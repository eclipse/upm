/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
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

#include "lsm303d.h"


// some useful macros to save on typing and text wrapping
#undef _SHIFT
#define _SHIFT(x) (_LSM303D_##x##_SHIFT)

#undef _MASK
#define _MASK(x) (_LSM303D_##x##_MASK)

#undef _SHIFTMASK
#define _SHIFTMASK(x) (_MASK(x) << _SHIFT(x))


// init
lsm303d_context lsm303d_init(int bus, int addr)
{
    lsm303d_context dev =
        (lsm303d_context)malloc(sizeof(struct _lsm303d_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _lsm303d_context));

    // make sure MRAA is initialized
    if (mraa_init() != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed.\n", __FUNCTION__);
        lsm303d_close(dev);
        return NULL;
    }

    if (!(dev->i2c = mraa_i2c_init(bus)))
    {
        printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
        lsm303d_close(dev);
        return NULL;
    }

    if (mraa_i2c_address(dev->i2c, addr))
    {
        printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
        lsm303d_close(dev);
        return NULL;
    }

    // check the chip id
    uint8_t chipID = lsm303d_read_reg(dev, LSM303D_REG_WHO_AM_I);

    if (chipID != LSM303D_CHIPID)
    {
        printf("%s: invalid chip id: %02x.  Expected %02x\n",
               __FUNCTION__, chipID, LSM303D_CHIPID);
        lsm303d_close(dev);
        return NULL;
    }

    // call devinit with a default high resolution mode
    if (lsm303d_devinit(dev, LSM303D_M_RES_HIGH))
    {
        printf("%s: lsm303d_devinit() failed.\n", __FUNCTION__);
        lsm303d_close(dev);
        return NULL;
    }

    return dev;
}

void lsm303d_close(lsm303d_context dev)
{
    assert(dev != NULL);

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);

    free(dev);
}

upm_result_t lsm303d_devinit(const lsm303d_context dev,
                             LSM303D_M_RES_T res)
{
    assert(dev != NULL);

    // enable all axes and BDU
    uint8_t reg = lsm303d_read_reg(dev, LSM303D_REG_CTRL1);

    reg |= LSM303D_CTRL1_AXEN
        | LSM303D_CTRL1_AYEN
        | LSM303D_CTRL1_AZEN
        | LSM303D_CTRL1_BDU;

    if (lsm303d_write_reg(dev, LSM303D_REG_CTRL1, reg))
    {
        printf("%s: lsm303d_write_reg() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // enable temperature measurement and set mag resolution
    reg = lsm303d_read_reg(dev, LSM303D_REG_CTRL5);
    reg &= ~_SHIFTMASK(CTRL5_MRES);
    reg |= LSM303D_CTRL5_TEMP_EN
        | (res << _SHIFT(CTRL5_MRES));

    if (lsm303d_write_reg(dev, LSM303D_REG_CTRL5, reg))
    {
        printf("%s: lsm303d_write_reg() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // set magnetometer to continuous mode
    reg = lsm303d_read_reg(dev, LSM303D_REG_CTRL7);
    reg &= ~_SHIFTMASK(CTRL7_MD);
    reg |= (LSM303D_MD_CONTINUOUS << _SHIFT(CTRL7_MD));

    if (lsm303d_write_reg(dev, LSM303D_REG_CTRL7, reg))
    {
        printf("%s: lsm303d_write_reg() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // set ACC ODR to 100Hz by default
    if (lsm303d_set_acc_odr(dev, LSM303D_AODR_100HZ))
    {
        printf("%s: lsm303d_set_acc_odr() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // set MAG ODR to 12.5Hz by default
    if (lsm303d_set_mag_odr(dev, LSM303D_MODR_12_5HZ))
    {
        printf("%s: lsm303d_set_acc_odr() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // default to 2G acc sensitivity
    if (lsm303d_set_acc_full_scale(dev, LSM303D_AFS_2G))
    {
        printf("%s: lsm303d_set_acc_full_scale() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // default to 2 Gauss mag sensitivity
    if (lsm303d_set_mag_full_scale(dev, LSM303D_MFS_2))
    {
        printf("%s: lsm303d_set_acc_full_scale() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    upm_delay_ms(10);

    return UPM_SUCCESS;
}

upm_result_t lsm303d_set_acc_full_scale(const lsm303d_context dev,
                                        LSM303D_AFS_T fs)
{
    assert(dev != NULL);

    uint8_t reg = lsm303d_read_reg(dev, LSM303D_REG_CTRL2);

    reg &= ~_SHIFTMASK(CTRL2_AFS);
    reg |= (fs << _SHIFT(CTRL2_AFS));

    if (lsm303d_write_reg(dev, LSM303D_REG_CTRL2, reg))
        return UPM_ERROR_OPERATION_FAILED;

    upm_delay_ms(50);

    // set our scaling factor depending on current FS
    switch(fs)
    {
    case LSM303D_AFS_2G:
        dev->accScale = 0.061;
        break;

    case LSM303D_AFS_4G:
        dev->accScale = 0.122;
        break;

    case LSM303D_AFS_6G:
        dev->accScale = 0.183;
        break;

    case LSM303D_AFS_8G:
        dev->accScale = 0.320;
        break;

    case LSM303D_AFS_16G:
        dev->accScale = 0.732;
        break;
    }

    return UPM_SUCCESS;
}

upm_result_t lsm303d_set_mag_full_scale(const lsm303d_context dev,
                                        LSM303D_MFS_T fs)
{
    assert(dev != NULL);

    uint8_t reg = lsm303d_read_reg(dev, LSM303D_REG_CTRL6);

    reg &= ~_SHIFTMASK(CTRL6_MFS);
    reg |= (fs << _SHIFT(CTRL6_MFS));

    if (lsm303d_write_reg(dev, LSM303D_REG_CTRL6, reg))
        return UPM_ERROR_OPERATION_FAILED;

    upm_delay_ms(50);

    // set our scaling factor depending on current FS
    switch(fs)
    {
    case LSM303D_MFS_2:
        dev->magScale = 0.080;
        break;

    case LSM303D_MFS_4:
        dev->magScale = 0.160;
        break;

    case LSM303D_MFS_8:
        dev->magScale = 0.320;
        break;

    case LSM303D_MFS_12:
        dev->magScale = 0.479;
        break;
    }

    return UPM_SUCCESS;
}

upm_result_t lsm303d_update(const lsm303d_context dev)
{
    assert(dev != NULL);

    const int maxLen = 6;
    uint8_t buf[maxLen];

    // get the temperature first, only 2 bytes
    if (lsm303d_read_regs(dev, LSM303D_REG_TEMP_OUT_L, buf, 2) != 2)
    {
        printf("%s: lsm303d_read_regs(temp) failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    dev->temperature = (float)( (int16_t)(buf[0] | (buf[1] << 8)) << 4);

    // next, acc data
    if (lsm303d_read_regs(dev, LSM303D_REG_OUT_X_L_A, buf,
                          maxLen) != maxLen)
    {
        printf("%s: lsm303d_read_regs(acc) failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    dev->accX = (float)((int16_t)(buf[0] | (buf[1] << 8)));
    dev->accY = (float)((int16_t)(buf[2] | (buf[3] << 8)));
    dev->accZ = (float)((int16_t)(buf[4] | (buf[5] << 8)));

    // now mag data
    if (lsm303d_read_regs(dev, LSM303D_REG_OUT_X_L_M, buf,
                          maxLen) != maxLen)
    {
        printf("%s: lsm303d_read_regs(mag) failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    dev->magX = (float)((int16_t)(buf[0] | (buf[1] << 8)));
    dev->magY = (float)((int16_t)(buf[2] | (buf[3] << 8)));
    dev->magZ = (float)((int16_t)(buf[4] | (buf[5] << 8)));

    return UPM_SUCCESS;
}

uint8_t lsm303d_read_reg(const lsm303d_context dev, uint8_t reg)
{
    assert(dev != NULL);

    int rv = mraa_i2c_read_byte_data(dev->i2c, reg);
    if (rv < 0)
    {
        printf("%s: mraa_i2c_read_byte_data() failed\n", __FUNCTION__);
        return 0xff;
    }

    return (uint8_t)rv;
}

int lsm303d_read_regs(const lsm303d_context dev, uint8_t reg,
                      uint8_t *buffer, int len)
{
    assert(dev != NULL);

    reg |= 0x80; // enable auto-increment
    if (mraa_i2c_read_bytes_data(dev->i2c, reg, buffer, len) != len)
        return -1;

    return len;
}

upm_result_t lsm303d_write_reg(const lsm303d_context dev,
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

float lsm303d_get_temperature(const lsm303d_context dev)
{
    assert(dev != NULL);

    // It's not clear how to compute this from the datasheet, but this
    // seems to give a reasonably accurate result.
    return (dev->temperature / 128.0) + 25.0;
}

void lsm303d_get_magnetometer(const lsm303d_context dev,
                              float *x, float *y, float *z)
{
    assert(dev != NULL);

    // Output is in milli-Gauss - we convert and return it in uT (SI
    // micro-teslas) instead.
    if (x)
        *x = (dev->magX * dev->magScale) / 10.0;
    if (y)
        *y = (dev->magY * dev->magScale) / 10.0;
    if (z)
        *z = (dev->magZ * dev->magScale) / 10.0;
}

void lsm303d_get_accelerometer(const lsm303d_context dev,
                               float *x, float *y, float *z)
{
    assert(dev != NULL);

    if (x)
        *x = (dev->accX * dev->accScale) / 1000.0;
    if (y)
        *y = (dev->accY * dev->accScale) / 1000.0;
    if (z)
        *z = (dev->accZ * dev->accScale) / 1000.0;
}

upm_result_t lsm303d_set_acc_odr(const lsm303d_context dev,
                                 LSM303D_AODR_T odr)
{
    assert(dev != NULL);

    uint8_t reg = lsm303d_read_reg(dev, LSM303D_REG_CTRL1);
    reg &= ~_SHIFTMASK(CTRL1_AODR);
    reg |= (odr << _SHIFT(CTRL1_AODR));

    if (lsm303d_write_reg(dev, LSM303D_REG_CTRL1, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t lsm303d_set_mag_odr(const lsm303d_context dev,
                                 LSM303D_MODR_T odr)
{
    assert(dev != NULL);

    uint8_t reg = lsm303d_read_reg(dev, LSM303D_REG_CTRL5);
    reg &= ~_SHIFTMASK(CTRL5_MODR);
    reg |= (odr << _SHIFT(CTRL5_MODR));

    if (lsm303d_write_reg(dev, LSM303D_REG_CTRL5, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}
