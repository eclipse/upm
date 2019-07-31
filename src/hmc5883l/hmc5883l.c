/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * Ported based on original C++ code by:
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "math.h"
#include "hmc5883l.h"

#define MAX_BUFFER_LENGTH 6
#define HMC5883L_I2C_ADDR 0x1E

// configuration registers
#define HMC5883L_CONF_REG_A 0x00
#define HMC5883L_CONF_REG_B 0x01

// mode register
#define HMC5883L_MODE_REG 0x02

// data register
#define HMC5883L_X_MSB_REG 0
#define HMC5883L_X_LSB_REG 1
#define HMC5883L_Z_MSB_REG 2
#define HMC5883L_Z_LSB_REG 3
#define HMC5883L_Y_MSB_REG 4
#define HMC5883L_Y_LSB_REG 5
#define DATA_REG_SIZE 6

// status register
#define HMC5883L_STATUS_REG 0x09

// ID registers
#define HMC5883L_ID_A_REG 0x0A
#define HMC5883L_ID_B_REG 0x0B
#define HMC5883L_ID_C_REG 0x0C

// mode
#define HMC5883L_CONT_MODE 0x00
// start of data registers (MSB/LSB for X, Z, and Y axis)
#define HMC5883L_DATA_REG 0x03

// scales
#define GA_0_88_REG 0x00 << 5
#define GA_1_3_REG 0x01 << 5
#define GA_1_9_REG 0x02 << 5
#define GA_2_5_REG 0x03 << 5
#define GA_4_0_REG 0x04 << 5
#define GA_4_7_REG 0x05 << 5
#define GA_5_6_REG 0x06 << 5
#define GA_8_1_REG 0x07 << 5

// scale factors
#define SCALE_0_73_MG 0.73
#define SCALE_0_92_MG 0.92
#define SCALE_1_22_MG 1.22
#define SCALE_1_52_MG 1.52
#define SCALE_2_27_MG 2.27
#define SCALE_2_56_MG 2.56
#define SCALE_3_03_MG 3.03
#define SCALE_4_35_MG 4.35

hmc5883l_context hmc5883l_init(int bus)
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    hmc5883l_context dev =
        (hmc5883l_context)malloc(sizeof(struct _hmc5883l_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _hmc5883l_context));

    if (!(dev->i2c = mraa_i2c_init(bus)))
    {
        printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
        hmc5883l_close(dev);
        return NULL;
    }

    // apparently only one address supported...
    if (mraa_i2c_address(dev->i2c, HMC5883L_I2C_ADDR))
    {
        printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
        hmc5883l_close(dev);
        return NULL;
    }

    // I guess we only support a single configuration...
    uint8_t buf[2];

    buf[0] = HMC5883L_CONF_REG_B;
    buf[1] = GA_1_3_REG; // should be 1.2 according to DS
    if (mraa_i2c_write(dev->i2c, buf, 2))
    {
        printf("%s: mraa_i2c_write(HMC5883L_CONF_REG_B) failed.\n",
               __FUNCTION__);
        hmc5883l_close(dev);
        return NULL;
    }

    // ... and we only support continuous mode.
    buf[0] = HMC5883L_MODE_REG;
    buf[1] = HMC5883L_CONT_MODE;
    if (mraa_i2c_write(dev->i2c, buf, 2))
    {
        printf("%s: mraa_i2c_write(HMC5883L_MODE_REG) failed.\n",
               __FUNCTION__);
        hmc5883l_close(dev);
        return NULL;
    }

    // do an initial update
    if (hmc5883l_update(dev))
    {
        printf("%s: hmc5883l_update() failed.\n", __FUNCTION__);
        hmc5883l_close(dev);
        return NULL;
    }

    return dev;
}

void hmc5883l_close(hmc5883l_context dev)
{
    assert(dev != NULL);

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);

    free(dev);
}

upm_result_t hmc5883l_update(const hmc5883l_context dev)
{
    assert(dev != NULL);

    // set device reg address
    if (mraa_i2c_write_byte(dev->i2c, HMC5883L_DATA_REG))
    {
        printf("%s: mraa_i2c_write_byte(HMC5883L_DATA_REG) failed.\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    uint8_t buf[DATA_REG_SIZE];

    // now read the data (6 bytes)
    int rb;
    if ((rb = mraa_i2c_read(dev->i2c, buf, DATA_REG_SIZE)) != DATA_REG_SIZE)
    {
        printf("%s: mraa_i2c_read() failed.  Got %d bytes, expected %d.\n",
               __FUNCTION__, rb, DATA_REG_SIZE);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // The device stores the data in X, Z, Y order, however, we
    // store them in X, Y, Z order.

    // x
    dev->coords[0] = (buf[HMC5883L_X_MSB_REG] << 8 )
        | buf[HMC5883L_X_LSB_REG];
    // z
    dev->coords[2] = (buf[HMC5883L_Z_MSB_REG] << 8 )
        | buf[HMC5883L_Z_LSB_REG];
    // y
    dev->coords[1] = (buf[HMC5883L_Y_MSB_REG] << 8 )
        | buf[HMC5883L_Y_LSB_REG];

    return UPM_SUCCESS;
}

float hmc5883l_direction(const hmc5883l_context dev)
{
    assert(dev != NULL);

    // we seem to only support a single scale :(
    return atan2(dev->coords[1] * SCALE_0_92_MG,
                 dev->coords[0] * SCALE_0_92_MG) + dev->declination;
}

float hmc5883l_heading(const hmc5883l_context dev)
{
    assert(dev != NULL);

    float dir = hmc5883l_direction(dev) * 180/M_PI;
    if (dir < 0)
        dir += 360.0;

    return dir;
}

const int16_t *hmc5883l_coordinates(const hmc5883l_context dev)
{
    assert(dev != NULL);

    return dev->coords;
}

void hmc5883l_set_declination(const hmc5883l_context dev, float dec)
{
    assert(dev != NULL);

    dev->declination = dec;
}

float hmc5883l_get_declination(const hmc5883l_context dev)
{
    assert(dev != NULL);

    return dev->declination;
}
