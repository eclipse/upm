/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <string.h>
#include <assert.h>
#include <upm_utilities.h>

#include "bmp280.h"

// number of bytes of stored calibration data (bmp280)
#define BMP280_CALIBRATION_BYTES (26)
// BME280 has these additional calibration regs
#define BME280_CALIBRATION_BYTES (7)

// Number of bytes to get bmp280 data
#define BMP280_DATA_LEN (6)
// Number of bytes to get bme280 data
#define BME280_DATA_LEN (2)

// Uncomment the following to use test data as specified in the
// datasheet, section 3.12.  This really only tests the compensation
// algorithm, and only for the bmp280 parts (temperature/pressure).

// #define BMP280_USE_TEST_DATA

// SPI CS on and off functions
void _csOn(const bmp280_context dev)
{
    assert(dev != NULL);

    if (dev->gpio)
        mraa_gpio_write(dev->gpio, 0);
}

void _csOff(const bmp280_context dev)
{
    assert(dev != NULL);

    if (dev->gpio)
        mraa_gpio_write(dev->gpio, 1);
}

// These functions come from the BMP280 datasheet, section 3.11.3

// Returns temperature in DegC, resolution is 0.01 DegC. Output value
// of "5123" equals 51.23 DegC.  t_fine carries fine temperature as
// global value
static int32_t _bmp280_compensate_T_int32(const bmp280_context dev,
                                          int32_t adc_T)
{
    assert(dev != NULL);

    int32_t var1, var2, T;
    var1 = ((((adc_T>>3) - ((int32_t)dev->dig_T1<<1))) * ((int32_t)dev->dig_T2)) >> 11;
    var2 = (((((adc_T>>4) - ((int32_t)dev->dig_T1)) * ((adc_T>>4) - ((int32_t)dev->dig_T1))) >> 12) *
            ((int32_t)dev->dig_T3)) >> 14;
    dev->t_fine = var1 + var2;
    T = (dev->t_fine * 5 + 128) >> 8;
    return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format
// (24 integer bits and 8 fractional bits).  Output value of
// "24674867" represents 24674867/256 = 96386.2 Pa = 963.862 hPa
static uint32_t _bmp280_compensate_P_int64(const bmp280_context dev,
                                           int32_t adc_P)
{
    assert(dev != NULL);

    int64_t var1, var2, p;
    var1 = ((int64_t)dev->t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dev->dig_P6;
    var2 = var2 + ((var1*(int64_t)dev->dig_P5)<<17);
    var2 = var2 + (((int64_t)dev->dig_P4)<<35);
    var1 = ((var1 * var1 * (int64_t)dev->dig_P3)>>8) + ((var1 * (int64_t)dev->dig_P2)<<12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)dev->dig_P1)>>33;
    if (var1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576-adc_P;
    p = (((p<<31)-var2)*3125)/var1;
    var1 = (((int64_t)dev->dig_P9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((int64_t)dev->dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)dev->dig_P7)<<4);
    return (uint32_t)p;
}

// This function comes from the BME280 datasheet, section 4.2.3
// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format
// (22 integer and 10 fractional bits).  Output value of "47445"
// represents 47445/1024 = 46.333 %RH
// BME280 only
static uint32_t _bme280_compensate_H_int32(const bmp280_context dev,
                                           int32_t adc_H)
{
    assert(dev != NULL);

    int32_t v_x1_u32r;
    v_x1_u32r = (dev->t_fine - ((int32_t)76800));
    v_x1_u32r = (((((adc_H << 14) - (((int32_t)dev->dig_H4) << 20) - (((int32_t)dev->dig_H5) * v_x1_u32r)) +
                   ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)dev->dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)dev->dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)dev->dig_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)dev->dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    return (int32_t)(v_x1_u32r>>12);
}

// read the calibration data
upm_result_t _read_calibration_data(const bmp280_context dev)
{
    assert(dev != NULL);

#if defined(BMP280_USE_TEST_DATA)
    printf("%s: WARNING: Test data is being used\n", __FUNCTION__);

    // This data is taken from the datasheet, section 3.12
    dev->dig_T1 = 27504;
    dev->dig_T2 = 26435;
    dev->dig_T3 = -1000;

    dev->dig_P1 = 36477;
    dev->dig_P2 = -10685;
    dev->dig_P3 = 3024;
    dev->dig_P4 = 2855;
    dev->dig_P5 = 140;
    dev->dig_P6 = -7;
    dev->dig_P7 = 15500;
    dev->dig_P8 = -14600;
    dev->dig_P9 = 6000;

    // BME280 does not have any test data....
    dev->dig_H1 = 0;
    dev->dig_H2 = 0;
    dev->dig_H3 = 0;
    dev->dig_H4 = 0;
    dev->dig_H5 = 0;
    dev->dig_H6 = 0;

#else

    // bmp280...
    uint8_t calibData[BMP280_CALIBRATION_BYTES];
    int rv;
    if ( (rv = bmp280_read_regs(dev, BMP280_REG_CALIB00, calibData,
                                BMP280_CALIBRATION_BYTES))
         != BMP280_CALIBRATION_BYTES)
    {
        printf("%s: bmp280_read_regs(BMP280) failed.",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    dev->dig_T1 = (uint16_t)((calibData[1] << 8) | calibData[0]);
    dev->dig_T2 = (int16_t)((calibData[3] << 8) | calibData[2]);
    dev->dig_T3 = (int16_t)((calibData[5] << 8) | calibData[4]);

    dev->dig_P1 = (uint16_t)((calibData[7] << 8) | calibData[6]);
    dev->dig_P2 = (int16_t)((calibData[9] << 8) | calibData[8]);
    dev->dig_P3 = (int16_t)((calibData[11] << 8) | calibData[10]);
    dev->dig_P4 = (int16_t)((calibData[13] << 8) | calibData[12]);
    dev->dig_P5 = (int16_t)((calibData[15] << 8) | calibData[14]);
    dev->dig_P6 = (int16_t)((calibData[17] << 8) | calibData[16]);
    dev->dig_P7 = (int16_t)((calibData[19] << 8) | calibData[18]);
    dev->dig_P8 = (int16_t)((calibData[21] << 8) | calibData[20]);
    dev->dig_P9 = (int16_t)((calibData[23] << 8) | calibData[22]);

    // bme280... (humidity calibration data)
    if (dev->isBME)
    {
        dev->dig_H1 = bmp280_read_reg(dev, BME280_REG_CALIB_DIG_H1);

        uint8_t hCalibData[BME280_CALIBRATION_BYTES];
        if ( (rv = bmp280_read_regs(dev, BME280_REG_CALIB_DIG_H2_LSB,
                                    hCalibData,
                                    BME280_CALIBRATION_BYTES))
             != BME280_CALIBRATION_BYTES)
        {
            printf("%s: bmp280_read_regs(BME280) failed.",
                   __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        dev->dig_H2 = (int16_t)((hCalibData[1] << 8) | hCalibData[0]);

        dev->dig_H3 = hCalibData[2];

        dev->dig_H4 = (int16_t)( (hCalibData[3] << 4) |
                               (hCalibData[4] & 0x0f) );

        dev->dig_H5 = (int16_t)( ((hCalibData[4] & 0xf0) >> 4) |
                               (hCalibData[5] << 4) );

        dev->dig_H6 = (int8_t)(hCalibData[6]);
    }
#endif // defined(BMP280_USE_TEST_DATA)

    return UPM_SUCCESS;
}

bmp280_context bmp280_init(int bus, int addr, int cs)
{
    bmp280_context dev =
        (bmp280_context)malloc(sizeof(struct _bmp280_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _bmp280_context));

    // make sure MRAA is initialized
    if (mraa_init() != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed.\n", __FUNCTION__);
        bmp280_close(dev);
        return NULL;
    }

    if (addr < 0)
        dev->isSPI = true;

    if (dev->isSPI)
    {
        if (!(dev->spi = mraa_spi_init(bus)))
        {
            printf("%s: mraa_spi_init() failed.\n", __FUNCTION__);
            bmp280_close(dev);
            return NULL;
        }

        // Only create cs context if we are actually using a valid pin.
        // A hardware controlled pin should specify cs as -1.
        if (cs >= 0)
        {
            if (!(dev->gpio = mraa_gpio_init(cs)))
            {
                printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
                bmp280_close(dev);
                return NULL;
            }
            mraa_gpio_dir(dev->gpio, MRAA_GPIO_OUT);
        }

        mraa_spi_mode(dev->spi, MRAA_SPI_MODE0);
        if (mraa_spi_frequency(dev->spi, 5000000))
        {
            printf("%s: mraa_spi_frequency() failed.\n", __FUNCTION__);
            bmp280_close(dev);
            return NULL;
        }

        // toggle CS on/off so chip switches into SPI mode.  For a hw
        // CS pin, the first SPI transaction should accomplish this.
        _csOn(dev);
        upm_delay_ms(10);
        _csOff(dev);
    }
    else
    {
        // I2C

        if (!(dev->i2c = mraa_i2c_init(bus)))
        {
            printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
            bmp280_close(dev);
            return NULL;
        }

        if (mraa_i2c_address(dev->i2c, addr))
        {
            printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
            bmp280_close(dev);
            return NULL;
        }
    }

    // check the chip id

    uint8_t chipID = bmp280_read_reg(dev, BMP280_REG_CHIPID);

    switch(chipID)
    {
    case BMP280_CHIPID: // BMP280
        dev->isBME = false;
        break;

    case BME280_CHIPID: // BME280
        dev->isBME = true;
        break;

    default: // ??
        printf("%s: invalid chip id: %02x.  Expected either %02x "
               "(bmp280) or %02x (bme280)\n",
               __FUNCTION__, chipID, BMP280_CHIPID, BME280_CHIPID);
        bmp280_close(dev);
        return NULL;
    }

    // set sleep mode for now
    bmp280_set_measure_mode(dev, BMP280_MODE_SLEEP);

    // read calibration data
    if (_read_calibration_data(dev))
    {
        printf("%s: _read_calibration_data() failed.", __FUNCTION__);
        bmp280_close(dev);
        return NULL;
    }

    // set the default mode to the highest resolution mode
    bmp280_set_usage_mode(dev, BMP280_USAGE_MODE_INDOOR_NAV);

    // set the default sea level pressure in hPA
    dev->sea_level_hPA = BMP280_SEA_LEVEL_HPA;

    return dev;
}

void bmp280_close(bmp280_context dev)
{
    assert(dev != NULL);

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);

    if (dev->spi)
        mraa_spi_stop(dev->spi);

    if (dev->gpio)
        mraa_gpio_close(dev->gpio);

    free(dev);
}

upm_result_t bmp280_update(const bmp280_context dev)
{
    assert(dev != NULL);

    int32_t temp = 0;
    int32_t pres = 0;

    uint8_t bmp_data[BMP280_DATA_LEN];
    memset(bmp_data, 0, BMP280_DATA_LEN);

    // If we are using a forced mode, then we need to manually trigger
    // the measurement and wait for it to complete.

    if (dev->mode == BMP280_MODE_FORCED)
    {
        // bmp280 measure mode will return to sleep after completion...
        bmp280_set_measure_mode(dev, BMP280_MODE_FORCED);

        uint8_t stat;
        do
        {
            upm_delay_ms(10);
            stat = bmp280_get_status(dev);
        } while (stat & BMP280_STATUS_MEASURING);
    }

    int rv;
    if ((rv = bmp280_read_regs(dev, BMP280_REG_PRESSURE_MSB,
                               bmp_data, BMP280_DATA_LEN))
        != BMP280_DATA_LEN)
    {
        printf("%s: bmp280_read_regs() failed.", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // 20 bits unsigned stored in a 32bit signed quantity

#if defined(BMP280_USE_TEST_DATA)
    // taken from datasheet, section 3.12
    temp = 519888;
    pres = 415148;
#else
    temp = ( (bmp_data[5] >> 4) | (bmp_data[4] << 4) | (bmp_data[3] << 12) );
    pres = ( (bmp_data[2] >> 4) | (bmp_data[1] << 4) | (bmp_data[0] << 12) );
#endif

    dev->temperature = (float)_bmp280_compensate_T_int32(dev, temp);
    dev->temperature /= 100.0;

    dev->pressure = (float)_bmp280_compensate_P_int64(dev, pres);
    dev->pressure /= 256.0;

    // BME280?
    if (dev->isBME)
    {
        uint8_t bme_data[BME280_DATA_LEN];

        memset(bme_data, 0, BME280_DATA_LEN);

        if ((rv = bmp280_read_regs(dev, BME280_REG_HUMIDITY_MSB,
                                   bme_data, BME280_DATA_LEN))
            != BME280_DATA_LEN)
        {
            printf("%s: bmp280_read_regs(BME280) failed.",
                   __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        // 20 bits unsigned stored in a 32bit signed quantity
        int32_t hum = ( (bme_data[0] << 8) | bme_data[1] );

        dev->humidity = (float)_bme280_compensate_H_int32(dev, hum);
        dev->humidity /= 1024.0;
    }

    return UPM_SUCCESS;
}

void bmp280_set_sea_level_pressure(const bmp280_context dev,
                                   float seaLevelhPA)
{
    assert(dev != NULL);

    dev->sea_level_hPA = seaLevelhPA;
}

float bmp280_get_altitude(const bmp280_context dev)
{
    assert(dev != NULL);

    // adapted from the US NOAA pdf: pressureAltitude.pdf
    return 44307.69 * (1.0 - pow((dev->pressure/100.0)
                                 / dev->sea_level_hPA, 0.190284));
}

uint8_t bmp280_read_reg(const bmp280_context dev, uint8_t reg)
{
    assert(dev != NULL);

    if (dev->isSPI)
    {
        reg |= 0x80; // needed for read
        uint8_t pkt[2] = {reg, 0};

        _csOn(dev);
        if (mraa_spi_transfer_buf(dev->spi, pkt, pkt, 2))
        {
            _csOff(dev);
            printf("%s: mraa_spi_transfer_buf() failed.",
                   __FUNCTION__);

            return 0xff;
        }
        _csOff(dev);

        return pkt[1];
    }
    else
    {
        return (uint8_t)mraa_i2c_read_byte_data(dev->i2c, reg);
    }
}

int bmp280_read_regs(const bmp280_context dev,
                     uint8_t reg, uint8_t *buffer, int len)
{
    assert(dev != NULL);

    if (dev->isSPI)
    {
        reg |= 0x80; // needed for read

        uint8_t sbuf[len + 1];
        memset((char *)sbuf, 0, len + 1);
        sbuf[0] = reg;

        // We need to do it this way for edison - ie: use a single
        // transfer rather than breaking it up into two like we used
        // to, since we have no control over CS.  This means a buffer
        // copy is now required, but that's the way it goes.

        _csOn(dev);
        if (mraa_spi_transfer_buf(dev->spi, sbuf, sbuf, len + 1))
        {
            _csOff(dev);
            printf("%s: mraa_spi_transfer_buf() failed.",
                   __FUNCTION__);

            return 0;
        }
        _csOff(dev);

        // now copy it into user buffer
        for (int i=0; i<len; i++)
            buffer[i] = sbuf[i + 1];
    }
    else
    {
        if (mraa_i2c_read_bytes_data(dev->i2c, reg, buffer, len) != len)
            return UPM_ERROR_OPERATION_FAILED;
    }

    return len;
}

upm_result_t bmp280_write_reg(const bmp280_context dev,
                              uint8_t reg, uint8_t val)
{
    assert(dev != NULL);

    if (dev->isSPI)
    {
        reg &= 0x7f; // mask off 0x80 for writing
        uint8_t pkt[2] = {reg, val};

        _csOn(dev);
        if (mraa_spi_transfer_buf(dev->spi, pkt, NULL, 2))
        {
            _csOff(dev);
            printf("%s: mraa_spi_transfer_buf() failed.",
                   __FUNCTION__);

            return UPM_ERROR_OPERATION_FAILED;
        }
        _csOff(dev);
    }
    else
    {
        if (mraa_i2c_write_byte_data(dev->i2c, val, reg))
        {
            printf("%s: mraa_i2c_write_byte_data() failed.",
                   __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }
    }

    return UPM_SUCCESS;
}

uint8_t bmp280_get_chip_id(const bmp280_context dev)
{
    assert(dev != NULL);

    return bmp280_read_reg(dev, BMP280_REG_CHIPID);
}

void bmp280_reset(const bmp280_context dev)
{
    assert(dev != NULL);

    bmp280_write_reg(dev, BMP280_REG_RESET, BMP280_RESET_BYTE);
    upm_delay(1);
}

float bmp280_get_temperature(const bmp280_context dev)
{
    assert(dev != NULL);

    return dev->temperature;
}

float bmp280_get_pressure(const bmp280_context dev)
{
    assert(dev != NULL);

    return dev->pressure;
}

// BME280 only
float bmp280_get_humidity(const bmp280_context dev)
{
    assert(dev != NULL);

    if (dev->isBME)
        return dev->humidity;
    else
        return 0.0;
}

void bmp280_set_filter(const bmp280_context dev, BMP280_FILTER_T filter)
{
    assert(dev != NULL);

    uint8_t reg = bmp280_read_reg(dev, BMP280_REG_CONFIG);

    reg &= ~(_BMP280_CONFIG_FILTER_MASK << _BMP280_CONFIG_FILTER_SHIFT);
    reg |= (filter << _BMP280_CONFIG_FILTER_SHIFT);

    bmp280_write_reg(dev, BMP280_REG_CONFIG, reg);
}

void bmp280_set_timer_standby(const bmp280_context dev,
                              BMP280_T_SB_T tsb)
{
    assert(dev != NULL);

    uint8_t reg = bmp280_read_reg(dev, BMP280_REG_CONFIG);

    reg &= ~(_BMP280_CONFIG_T_SB_MASK << _BMP280_CONFIG_T_SB_SHIFT);
    reg |= (tsb << _BMP280_CONFIG_T_SB_SHIFT);

    bmp280_write_reg(dev, BMP280_REG_CONFIG, reg);
}

void bmp280_set_measure_mode(const bmp280_context dev,
                             BMP280_MODES_T mode)
{
    assert(dev != NULL);

    uint8_t reg = bmp280_read_reg(dev, BMP280_REG_CTRL_MEAS);

    reg &= ~(_BMP280_CTRL_MEAS_MODE_MASK << _BMP280_CTRL_MEAS_MODE_SHIFT);
    reg |= (mode << _BMP280_CTRL_MEAS_MODE_SHIFT);

    bmp280_write_reg(dev, BMP280_REG_CTRL_MEAS, reg);
    dev->mode = mode;
}

void bmp280_set_oversample_rate_pressure(const bmp280_context dev,
                                         BMP280_OSRS_P_T rate)
{
    assert(dev != NULL);

    uint8_t reg = bmp280_read_reg(dev, BMP280_REG_CTRL_MEAS);

    reg &= ~(_BMP280_CTRL_MEAS_OSRS_P_MASK << _BMP280_CTRL_MEAS_OSRS_P_SHIFT);
    reg |= (rate << _BMP280_CTRL_MEAS_OSRS_P_SHIFT);

    bmp280_write_reg(dev, BMP280_REG_CTRL_MEAS, reg);
}

void bmp280_set_oversample_rate_temperature(const bmp280_context dev,
                                            BMP280_OSRS_T_T rate)
{
    assert(dev != NULL);

    uint8_t reg = bmp280_read_reg(dev, BMP280_REG_CTRL_MEAS);

    reg &= ~(_BMP280_CTRL_MEAS_OSRS_T_MASK << _BMP280_CTRL_MEAS_OSRS_T_SHIFT);
    reg |= (rate << _BMP280_CTRL_MEAS_OSRS_T_SHIFT);

    bmp280_write_reg(dev, BMP280_REG_CTRL_MEAS, reg);
}

// bme280 only
void bmp280_set_oversample_rate_humidity(const bmp280_context dev,
                                         BME280_OSRS_H_T rate)
{
    assert(dev != NULL);

    if (dev->isBME)
    {
        uint8_t reg = bmp280_read_reg(dev, BME280_REG_CTRL_HUM);

        reg &= ~(_BME280_CTRL_HUM_OSRS_H_MASK << _BME280_CTRL_HUM_OSRS_H_SHIFT);
        reg |= (rate << _BME280_CTRL_HUM_OSRS_H_SHIFT);

        bmp280_write_reg(dev, BME280_REG_CTRL_HUM, reg);
    }
}

uint8_t bmp280_get_status(const bmp280_context dev)
{
    assert(dev != NULL);

    return bmp280_read_reg(dev, BMP280_REG_STATUS);
}

void bmp280_set_usage_mode(const bmp280_context dev, BMP280_USAGE_MODE_T mode)
{
    assert(dev != NULL);

    // set up the regs for the given usage mode.  These settings come
    // from the recomendations in the BMP280 datasheet, section 3.4
    // Filter Selection.

    dev->temperature = 0;
    dev->pressure = 0;
    dev->humidity = 0;

    // set sleep mode first
    bmp280_set_measure_mode(dev, BMP280_MODE_SLEEP);

    switch (mode)
    {
    case BMP280_USAGE_MODE_HANDHELD_LOW_POWER:
        bmp280_set_oversample_rate_pressure(dev,
                                            BMP280_OSRS_P_OVERSAMPLING_16);
        bmp280_set_oversample_rate_temperature(dev,
                                               BMP280_OSRS_T_OVERSAMPLING_2);
        bmp280_set_oversample_rate_humidity(dev,
                                            BME280_OSRS_H_OVERSAMPLING_1);
        bmp280_set_filter(dev, BMP280_FILTER_4);
        bmp280_set_measure_mode(dev, BMP280_MODE_NORMAL);

        break;

    case BMP280_USAGE_MODE_HANDHELD_DYNAMIC:
        bmp280_set_oversample_rate_pressure(dev,
                                            BMP280_OSRS_P_OVERSAMPLING_4);
        bmp280_set_oversample_rate_temperature(dev,
                                               BMP280_OSRS_T_OVERSAMPLING_1);
        bmp280_set_oversample_rate_humidity(dev,
                                            BME280_OSRS_H_OVERSAMPLING_1);
        bmp280_set_filter(dev, BMP280_FILTER_16);
        bmp280_set_measure_mode(dev, BMP280_MODE_NORMAL);

        break;

    case BMP280_USAGE_MODE_WEATHER_MONITOR:
        bmp280_set_oversample_rate_pressure(dev,
                                            BMP280_OSRS_P_OVERSAMPLING_1);
        bmp280_set_oversample_rate_temperature(dev,
                                               BMP280_OSRS_T_OVERSAMPLING_1);
        bmp280_set_oversample_rate_humidity(dev,
                                            BME280_OSRS_H_OVERSAMPLING_1);
        bmp280_set_filter(dev, BMP280_FILTER_OFF);
        bmp280_set_measure_mode(dev, BMP280_MODE_FORCED);

        break;

    case BMP280_USAGE_MODE_FLOOR_CHG_DETECT:
        bmp280_set_oversample_rate_pressure(dev,
                                            BMP280_OSRS_P_OVERSAMPLING_4);
        bmp280_set_oversample_rate_temperature(dev,
                                               BMP280_OSRS_T_OVERSAMPLING_1);
        bmp280_set_oversample_rate_humidity(dev,
                                            BME280_OSRS_H_OVERSAMPLING_1);
        bmp280_set_filter(dev, BMP280_FILTER_4);
        bmp280_set_measure_mode(dev, BMP280_MODE_NORMAL);

        break;

    case BMP280_USAGE_MODE_DROP_DETECT:
        bmp280_set_oversample_rate_pressure(dev,
                                            BMP280_OSRS_P_OVERSAMPLING_2);
        bmp280_set_oversample_rate_temperature(dev,
                                               BMP280_OSRS_T_OVERSAMPLING_1);
        bmp280_set_oversample_rate_humidity(dev,
                                            BME280_OSRS_H_OVERSAMPLING_1);
        bmp280_set_filter(dev, BMP280_FILTER_OFF);
        bmp280_set_measure_mode(dev, BMP280_MODE_NORMAL);

        break;

    case BMP280_USAGE_MODE_INDOOR_NAV:
        bmp280_set_oversample_rate_pressure(dev,
                                            BMP280_OSRS_P_OVERSAMPLING_16);
        bmp280_set_oversample_rate_temperature(dev,
                                               BMP280_OSRS_T_OVERSAMPLING_2);
        bmp280_set_oversample_rate_humidity(dev,
                                            BME280_OSRS_H_OVERSAMPLING_1);
        bmp280_set_filter(dev, BMP280_FILTER_16);
        bmp280_set_measure_mode(dev, BMP280_MODE_NORMAL);

        break;

    default:
        printf("%s: invalid usage mode specified, mode unchanged.",
               __FUNCTION__);
    }
}
