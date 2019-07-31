/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "ms5803.h"

#include <upm_math.h>
#include <upm_platform.h>
#include <upm_utilities.h>

// quick binary power of 2 function
#define POWB(type, exp) ((type)1 << exp)

// For SPI, these are our CS on/off functions, if needed
static void ms5803_cs_on(const ms5803_context dev)
{
    assert(dev != NULL);

    if (dev->gpio)
        mraa_gpio_write(dev->gpio, 0);
}

static void ms5803_cs_off(const ms5803_context dev)
{
    assert(dev != NULL);

    if (dev->gpio)
        mraa_gpio_write(dev->gpio, 1);
}

static upm_result_t ms5803_get_adc_value(const ms5803_context dev,
                                         MS5803_CMD_T cmd,
                                         MS5803_OSR_T dly,
                                         uint32_t *value)
{
    assert(dev != NULL);

    uint8_t buf[3];

    if (ms5803_bus_write(dev, cmd, NULL, 0))
    {
        printf("%s: ms5802_bus_write() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // need to delay for the appropriate time
    upm_delay_ms(dly);

    // now, get the 3 byte sample
    if (ms5803_bus_read(dev, MS5803_CMD_ADC_READ, buf, 3))
    {
        printf("%s: ms5802_bus_read() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    *value = ((buf[0] << 16) | (buf[1] << 8) | buf[2]);

    return UPM_SUCCESS;
}

ms5803_context ms5803_init(unsigned int bus, int address, int cs_pin)
{
    ms5803_context dev =
        (ms5803_context)malloc(sizeof(struct _ms5803_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _ms5803_context));

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        ms5803_close(dev);
        return NULL;
    }

    if (address > 0)
    {
        // we are doing I2C
        dev->isSPI = false;

        if (!(dev->i2c = mraa_i2c_init(bus)))
        {
            printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
            ms5803_close(dev);
            return NULL;
        }

        if (mraa_i2c_address(dev->i2c, address) != MRAA_SUCCESS)
        {
            printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
            ms5803_close(dev);
            return NULL;
        }

#if defined(UPM_PLATFORM_ZEPHYR)
        // we seem to need to stick qith 100khz for some reason with
        // this device on Zephyr.  Even at 100Khz, while calibration
        // data is now read correctly, there are other issues yet to
        // be determined.  But as a first step, 100Khz is a
        // requirement for this driver on Zephyr.
        if (mraa_i2c_frequency(dev->i2c, MRAA_I2C_STD))
        {
            printf("%s: mraa_i2c_frequency() failed.\n", __FUNCTION__);
            ms5803_close(dev);
            return NULL;
        }
#endif // UPM_PLATFORM_ZEPHYR

    }
    else
    {
        // we are doing SPI
        dev->isSPI = true;

        if (!(dev->spi = mraa_spi_init(bus)))
        {
            printf("%s: mraa_spi_init() failed.\n", __FUNCTION__);
            ms5803_close(dev);
            return NULL;
        }

        // Only create cs context if we are actually using a valid pin.
        // A hardware controlled pin should specify cs as -1.
        if (cs_pin >= 0)
        {
            if (!(dev->gpio = mraa_gpio_init(cs_pin)))
            {
                printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
                ms5803_close(dev);
                return NULL;
            }

            mraa_gpio_dir(dev->gpio, MRAA_GPIO_OUT);
            ms5803_cs_off(dev);
        }

        if (mraa_spi_mode(dev->spi, MRAA_SPI_MODE0))
        {
            printf("%s: mraa_spi_mode() failed.\n", __FUNCTION__);
            ms5803_close(dev);
            return NULL;
        }

        if (mraa_spi_frequency(dev->spi, 10000000))
        {
            printf("%s: mraa_spi_frequency() failed.\n", __FUNCTION__);
            ms5803_close(dev);
            return NULL;
        }
    }

    // read factory coefficients
    if (ms5803_load_coefficients(dev))
    {
        printf("%s: ms5803_load_coefficients() failed.\n", __FUNCTION__);
        ms5803_close(dev);
        return NULL;
    }

    // set the default OSR to the highest resolution
    ms5803_set_temperature_osr(dev, MS5803_OSR_4096);
    ms5803_set_pressure_osr(dev, MS5803_OSR_4096);

    return dev;
}

void ms5803_close(ms5803_context dev)
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

upm_result_t ms5803_update(const ms5803_context dev)
{
    assert(dev != NULL);

    // start by getting temperature and then pressure

    uint32_t rawTemperature;
    uint32_t rawPressure;

    // temperature
    if (ms5803_get_adc_value(dev, dev->temperatureCmd, dev->temperatureDelay,
                             &rawTemperature))
    {
        printf("%s: ms5803_get_adc_value() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // pressure
    if (ms5803_get_adc_value(dev, dev->pressureCmd, dev->pressureDelay,
                             &rawPressure))
    {
        printf("%s: ms5803_get_adc_value() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

//    printf("raw T = %u P = %u\n", rawTemperature, rawPressure);

    // This algorithm comes from the datasheet.

    // calc 1st order compensated temperature
    int32_t dT = rawTemperature - dev->C[5] * POWB(int32_t, 8);
    int32_t TEMP = 2000 + (int64_t)dT * dev->C[6] / POWB(int32_t, 23);

    // calc compensated temp and pressure
    int64_t OFF, SENS;
    int32_t P = 0;

    // first order compensation
    OFF = dev->C[2] * POWB(int64_t, 16) + (dev->C[4] * dT)/POWB(int64_t, 7);
    SENS = dev->C[1] * POWB(int64_t, 15) + (dev->C[3] * dT)/POWB(int64_t, 8);

    // second order compensation
    int64_t T2 = 0, OFF2 = 0, SENS2 = 0;
    if (TEMP >= 2000)
    {
        // >=20C
        T2 = 7 * (((uint64_t)dT * dT) / POWB(int64_t, 37));
        OFF2 =  ((TEMP - 2000) * (TEMP - 2000)) / POWB(int64_t, 4);
        SENS2 = 0;
    }
    else
    {
        T2 = 3 * (((uint64_t)dT * dT) / POWB(int64_t, 33));
        OFF2 = 3 * ((TEMP - 2000) * (TEMP - 2000)) / POWB(int64_t, 1);
        SENS2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / POWB(int64_t, 3);

        // further compensation for very low temps
        if (TEMP < 1500)
        {
            // <15C
            OFF2 = OFF2 + 7 * ((TEMP + 1500) * (TEMP + 1500));
            SENS2 = SENS2 + 4 * ((TEMP + 1500) * (TEMP + 1500));
        }
    }

    // final caculation
    TEMP = TEMP - T2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;
    P = (rawPressure * SENS/POWB(int64_t, 21) - OFF)/POWB(int64_t, 15);

    dev->temperature = (float)TEMP / 100.0;
    dev->pressure = (float)P/10.0;

    return UPM_SUCCESS;
}

upm_result_t ms5803_load_coefficients(const ms5803_context dev)
{
    assert(dev != NULL);

    // we will load them all, even though only 6 of them are of use to us
    uint8_t buffer[2];

    for (int i=0; i<MS5803_MAX_COEFFICIENTS; i++)
    {
        uint8_t cmd = MS5803_CMD_PROM_READ | ((i & 7) << 1);
        if (ms5803_bus_read(dev, cmd, buffer, 2))
        {
            printf("%s: ms5802_bus_read() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        dev->C[i] = (buffer[0] << 8) | buffer[1];
//        printf("C[%d] = %u\n", i, dev->C[i]);
    }

    return UPM_SUCCESS;
}

// i2c bus read and write functions
upm_result_t ms5803_bus_read(const ms5803_context dev, uint8_t cmd,
                             uint8_t *data, uint8_t len)
{
    assert(dev != NULL);

    if (dev->isSPI)
    {
        // SPI

        uint8_t sbuf[len + 1];
        memset((void *)sbuf, 0, len + 1);
        sbuf[0] = cmd;

        ms5803_cs_on(dev);

        if (mraa_spi_transfer_buf(dev->spi, sbuf, sbuf, len + 1))
        {
            ms5803_cs_off(dev);
            printf("%s: mraa_spi_transfer_buf() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }
        ms5803_cs_off(dev);

        // now copy it into user buffer
        for (int i=0; i<len; i++)
            data[i] = sbuf[i + 1];
    }
    else
    {
        // I2C

        if (mraa_i2c_read_bytes_data(dev->i2c, cmd, data, len) < 0)
        {
            printf("%s: mraa_i2c_read_bytes() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }
    }

    return UPM_SUCCESS;
}

upm_result_t ms5803_bus_write(const ms5803_context dev, uint8_t cmd,
                              uint8_t *data, uint8_t len)
{
    assert(dev != NULL);

    if (dev->isSPI)
    {
        // SPI

        uint8_t sbuf[len + 1];
        memset((void *)sbuf, 0, len + 1);
        sbuf[0] = cmd;

        // copy in the data to write...
        if (data && len)
        {
            for (int i=0; i<len; i++)
                sbuf[i + 1] = data[i];
        }

        ms5803_cs_on(dev);

        if (mraa_spi_transfer_buf(dev->spi, sbuf, sbuf, len + 1))
        {
            ms5803_cs_off(dev);
            printf("%s: mraa_spi_transfer_buf() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }
        ms5803_cs_off(dev);
    }
    else
    {
        // I2C...

        uint8_t buffer[len + 1];

        buffer[0] = cmd;

        if (data && len)
        {
            for (int i=0; i<len; i++)
                buffer[i+1] = data[i];
        }

        mraa_result_t rv = mraa_i2c_write(dev->i2c, buffer, len+1);

        if (rv != MRAA_SUCCESS)
        {
            printf("%s: mraa_i2c_write() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }
    }

    return UPM_SUCCESS;
}

void ms5803_set_temperature_osr(const ms5803_context dev, MS5803_OSR_T osr)
{
    assert(dev != NULL);

    switch(osr)
    {
    case MS5803_OSR_256:
        dev->temperatureCmd = MS5803_CMD_CONVERT_D2_OSR_256;
        break;

    case MS5803_OSR_512:
        dev->temperatureCmd = MS5803_CMD_CONVERT_D2_OSR_512;
        break;

    case MS5803_OSR_1024:
        dev->temperatureCmd = MS5803_CMD_CONVERT_D2_OSR_1024;
        break;

    case MS5803_OSR_2048:
        dev->temperatureCmd = MS5803_CMD_CONVERT_D2_OSR_2048;
        break;

    case MS5803_OSR_4096:
        dev->temperatureCmd = MS5803_CMD_CONVERT_D2_OSR_4096;
        break;

    default:
        // can't happen in this universe
        printf("%s: Internal error, invalid osr value %d\n", __FUNCTION__,
               (int)osr);
        return;
    }

    dev->temperatureDelay = osr;
}

void ms5803_set_pressure_osr(const ms5803_context dev, MS5803_OSR_T osr)
{
    assert(dev != NULL);

    switch(osr)
    {
    case MS5803_OSR_256:
        dev->pressureCmd = MS5803_CMD_CONVERT_D1_OSR_256;
        break;

    case MS5803_OSR_512:
        dev->pressureCmd = MS5803_CMD_CONVERT_D1_OSR_512;
        break;

    case MS5803_OSR_1024:
        dev->pressureCmd = MS5803_CMD_CONVERT_D1_OSR_1024;
        break;

    case MS5803_OSR_2048:
        dev->pressureCmd = MS5803_CMD_CONVERT_D1_OSR_2048;
        break;

    case MS5803_OSR_4096:
        dev->pressureCmd = MS5803_CMD_CONVERT_D1_OSR_4096;
        break;

    default:
        // can't happen in this universe
        printf("%s: Internal error, invalid osr value %d\n", __FUNCTION__,
               (int)osr);
        return;
    }

    dev->pressureDelay = osr;
}

upm_result_t ms5803_reset(const ms5803_context dev)
{
    assert(dev != NULL);

    upm_result_t rv = ms5803_bus_write(dev, MS5803_CMD_RESET, NULL, 0);
    upm_delay_ms(5);

    return rv;
}

float ms5803_get_temperature(const ms5803_context dev)
{
    assert(dev != NULL);

    return dev->temperature;
}

float ms5803_get_pressure(const ms5803_context dev)
{
    assert(dev != NULL);

    return dev->pressure;
}
