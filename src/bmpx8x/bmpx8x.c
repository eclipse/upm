/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This driver was rewritten based on the original driver written by:
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <assert.h>

#include "upm_utilities.h"
#include "upm_math.h"

#include "bmpx8x.h"

// UT is uncompensated temperature
static int32_t _bmpx8x_computeB5(const bmpx8x_context dev, int32_t UT)
{
    assert(dev != NULL);

    int32_t X1 = (UT - (int32_t)dev->ac6) * ((int32_t)dev->ac5) >> 15;
    int32_t X2 = ((int32_t)dev->mc << 11) / (X1+(int32_t)dev->md);

    return X1 + X2;
}

static upm_result_t _bmpx8x_read_calibration_data(const bmpx8x_context dev)
{
    assert(dev != NULL);

    const int dataLen = 22;
    uint8_t calData[dataLen];

    if (bmpx8x_read_regs(dev, BMPX8X_CAL_AC1, calData, dataLen) != dataLen)
        return UPM_ERROR_OPERATION_FAILED;

    dev->ac1 = (int16_t)( (calData[0] << 8) | calData[1] );
    dev->ac2 = (int16_t)( (calData[2] << 8) | calData[3] );
    dev->ac3 = (int16_t)( (calData[4] << 8) | calData[5] );
    dev->ac4 = (uint16_t)( (calData[6] << 8) | calData[7] );
    dev->ac5 = (uint16_t)( (calData[8] << 8) | calData[9] );
    dev->ac6 = (uint16_t)( (calData[10] << 8) | calData[11] );

    dev->b1 = (int16_t)( (calData[12] << 8) | calData[13] );
    dev->b2 = (int16_t)( (calData[14] << 8) | calData[15] );

    dev->mb = (int16_t)( (calData[16] << 8) | calData[17] );
    dev->mc = (int16_t)( (calData[18] << 8) | calData[19] );
    dev->md = (int16_t)( (calData[20] << 8) | calData[21] );

    return UPM_SUCCESS;
}

// init
bmpx8x_context bmpx8x_init(int bus, int addr)
{
    bmpx8x_context dev =
        (bmpx8x_context)malloc(sizeof(struct _bmpx8x_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _bmpx8x_context));

    // make sure MRAA is initialized
    if (mraa_init() != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed.\n", __FUNCTION__);
        bmpx8x_close(dev);
        return NULL;
    }

    if (!(dev->i2c = mraa_i2c_init(bus)))
    {
        printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
        bmpx8x_close(dev);
        return NULL;
    }

    if (mraa_i2c_address(dev->i2c, addr))
    {
        printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
        bmpx8x_close(dev);
        return NULL;
    }

    // check the chip id

    uint8_t chipID = bmpx8x_get_chip_id(dev);

    if (chipID != BMPX8X_DEFAULT_CHIPID)
    {
        printf("%s: invalid chip id: %02x.  Expected %02x\n",
               __FUNCTION__, chipID, BMPX8X_DEFAULT_CHIPID);
        bmpx8x_close(dev);
        return NULL;
    }

    // call devinit with a default ultrahigh resolution mode
    if (bmpx8x_devinit(dev, BMPX8X_OSS_ULTRAHIGHRES))
    {
        printf("%s: bmpx8x_devinit() failed.\n", __FUNCTION__);
        bmpx8x_close(dev);
        return NULL;
    }

    return dev;
}

void bmpx8x_close(bmpx8x_context dev)
{
    assert(dev != NULL);

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);

    free(dev);
}

upm_result_t bmpx8x_devinit(const bmpx8x_context dev,
                            BMPX8X_OSS_T oss)
{
    assert(dev != NULL);

    // first read calibration data
    if (_bmpx8x_read_calibration_data(dev))
    {
        printf("%s: _bmpx8x_read_calibration_data() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // now set our oversampling mode
    bmpx8x_set_oversampling(dev, oss);

    return UPM_SUCCESS;
}

uint8_t bmpx8x_get_chip_id(const bmpx8x_context dev)
{
    assert(dev != NULL);

    return bmpx8x_read_reg(dev, BMPX8X_CHIP_ID);
}

upm_result_t bmpx8x_reset(const bmpx8x_context dev)
{
    assert(dev != NULL);

    if (bmpx8x_write_reg(dev, BMPX8X_RESET, BMPX8X_RESET_BYTE))
        return UPM_ERROR_OPERATION_FAILED;

    upm_delay(1);

    return UPM_SUCCESS;
}

void bmpx8x_set_oversampling(const bmpx8x_context dev,
                             BMPX8X_OSS_T oss)
{
    assert(dev != NULL);

    dev->oversampling = oss;
}

upm_result_t bmpx8x_update(const bmpx8x_context dev)
{
    assert(dev != NULL);

    const int maxLen = 3; // maximum number of bytes we will read
    uint8_t buffer[maxLen];

    // first we need to read the temperature

    // send the measurement command, and sleep the required time
    // before reading it

    if (bmpx8x_write_reg(dev, BMPX8X_CTRL_MEAS, BMPX8X_CMD_READ_TEMP))
    {
        printf("%s: bmpx8x_write_reg(tempcmd) failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }
    upm_delay_ms(5);

    if (bmpx8x_read_regs(dev, BMPX8X_OUTDATA_MSB, buffer, maxLen) != maxLen)
    {
        printf("%s: bmpx8x_read_regs(temp) failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // we only need the first 2 bytes, uncompensated temperature
    int32_t UT = (int32_t)( (buffer[0] << 8) | buffer[1] );

    // now read in the uncompensated pressure - the delay time depends
    // on the oversampling value

    uint8_t reg = BMPX8X_CMD_READ_PRESSURE |
        (dev->oversampling << _BMPX8X_CTRL_MEAS_OSS_SHIFT);

    if (bmpx8x_write_reg(dev, BMPX8X_CTRL_MEAS, reg))
    {
        printf("%s: bmpx8x_write_reg(prescmd) failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    switch(dev->oversampling)
    {
    case BMPX8X_OSS_ULTRALOWPOWER:
        upm_delay_ms(5);
        break;

    case BMPX8X_OSS_STANDARD:
        upm_delay_ms(8);
        break;

    case BMPX8X_OSS_HIGHRES:
        upm_delay_ms(14);
        break;

    case BMPX8X_OSS_ULTRAHIGHRES:
        upm_delay_ms(26);
        break;
    }

    if (bmpx8x_read_regs(dev, BMPX8X_OUTDATA_MSB, buffer, maxLen) != maxLen)
    {
        printf("%s: bmpx8x_read_regs(pres) failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // uncompensated pressure

    int32_t UP = ( (buffer[0] << 16) | (buffer[1] << 8) | buffer[2] );
    UP >>= (8 - dev->oversampling);

    // now, compensate and store
    int32_t B3, B5, B6, X1, X2, X3, p;
    uint32_t B4, B7;

    // temperature
    B5 = _bmpx8x_computeB5(dev, UT);

    dev->temperature = (float)( (B5 + 8) >> 4 );
    dev->temperature /= 10.0;

    // pressure
    B6 = B5 - 4000;
    X1 = ((int32_t)dev->b2 * ( (B6 * B6)>>12 )) >> 11;
    X2 = ((int32_t)dev->ac2 * B6) >> 11;
    X3 = X1 + X2;
    B3 = ((((int32_t)dev->ac1*4 + X3) << dev->oversampling) + 2) / 4;

    X1 = ((int32_t)dev->ac3 * B6) >> 13;
    X2 = ((int32_t)dev->b1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = ((uint32_t)dev->ac4 * (uint32_t)(X3 + 32768)) >> 15;
    B7 = ((uint32_t)UP - B3) * (uint32_t)( 50000UL >> dev->oversampling );

    if (B7 < 0x80000000)
        p = (B7 * 2) / B4;
    else
        p = (B7 / B4) * 2;

    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;

    dev->pressure = p + ((X1 + X2 + (int32_t)3791)>>4);

    return UPM_SUCCESS;
}

int bmpx8x_get_pressure(const bmpx8x_context dev)
{
    assert(dev != NULL);

    return dev->pressure;
}

float bmpx8x_get_temperature(const bmpx8x_context dev)
{
    assert(dev != NULL);

    return dev->temperature;
}

int bmpx8x_get_sealevel_pressure(const bmpx8x_context dev,
                                   float altitude)
{
    assert(dev != NULL);

    return ((float)dev->pressure / powf(1.0-altitude/44330.0, 5.255));
}

float bmpx8x_get_altitude(const bmpx8x_context dev, int sealevel)
{
    assert(dev != NULL);

    // avoid potential divide-by-0, and set the default to 101325 Pa
    if (sealevel <= 0)
        sealevel = 101325;

    return 44307.69 * (1.0 - powf((float)dev->pressure / (float)sealevel,
                                 0.190284));
}

uint8_t bmpx8x_read_reg(const bmpx8x_context dev, uint8_t reg)
{
    assert(dev != NULL);

    return (uint8_t)mraa_i2c_read_byte_data(dev->i2c, reg);
}

int bmpx8x_read_regs(const bmpx8x_context dev, uint8_t reg,
                     uint8_t *buffer, int len)
{
    assert(dev != NULL);

    if (mraa_i2c_read_bytes_data(dev->i2c, reg, buffer, len) != len)
        return -1;

    return len;
}

upm_result_t bmpx8x_write_reg(const bmpx8x_context dev,
                              uint8_t reg, uint8_t val)
{
    assert(dev != NULL);

    if (mraa_i2c_write_byte_data(dev->i2c, val, reg))
    {
        printf("%s: mraa_i2c_write_byte_data() failed.\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}
