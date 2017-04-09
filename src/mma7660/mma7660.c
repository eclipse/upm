/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string.h>
#include <assert.h>
#include "mma7660.h"

mma7660_context mma7660_init(int bus, uint8_t address)
{
    mma7660_context dev =
        (mma7660_context)malloc(sizeof(struct _mma7660_context));

    if (!dev)
        return NULL;

    memset((void *)dev, 0, sizeof(struct _mma7660_context));

    dev->i2c = NULL;
    dev->gpio = NULL;
    dev->isrInstalled = false;

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        mma7660_close(dev);
        return NULL;
    }

    // setup our i2c
    if ( !(dev->i2c = mraa_i2c_init(bus)) )
    {
        printf("%s: mraa_i2c_init() failed\n", __FUNCTION__);
        mma7660_close(dev);
        return NULL;
    }

    if (mraa_i2c_address(dev->i2c, address))
    {
        printf("%s: mraa_i2c_address() failed\n", __FUNCTION__);
        mma7660_close(dev);
        return NULL;
    }

    return dev;
}

void mma7660_close(mma7660_context dev)
{
    assert(dev != NULL);

    mma7660_uninstall_isr(dev);

    if (dev->i2c)
    {
        mma7660_set_mode_standby(dev);
        mraa_i2c_stop(dev->i2c);
    }

    free(dev);
}

upm_result_t mma7660_write_byte(const mma7660_context dev,
                                uint8_t reg, uint8_t byte)
{
    assert(dev != NULL);

    if (mraa_i2c_write_byte_data(dev->i2c, byte, reg))
    {
        printf("%s: mraa_i2c_write_byte_data() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t mma7660_read_byte(const mma7660_context dev, uint8_t reg,
                               uint8_t *byte)
{
    assert(dev != NULL);

    int x = mraa_i2c_read_byte_data(dev->i2c, reg);

    if (x < 0)
    {
        printf("%s: mraa_i2c_read_byte_data() failed.\n", __FUNCTION__);
        if (byte)
            *byte = 0;

        return UPM_ERROR_OPERATION_FAILED;
    }

    if (byte)
        *byte = (uint8_t)(x & 0xff);

  return UPM_SUCCESS;
}

upm_result_t mma7660_get_raw_values(const mma7660_context dev,
                                    int *x, int *y, int *z)
{
    assert(dev != NULL);

    int rv;
    if (x)
    {
        if ( (rv = mma7660_get_verified_axis(dev, MMA7660_REG_XOUT, x)) )
            return rv;
    }
    if (y)
    {
        if ( (rv = mma7660_get_verified_axis(dev, MMA7660_REG_YOUT, y)) )
            return rv;
    }
    if (z)
    {
        if ( (rv = mma7660_get_verified_axis(dev, MMA7660_REG_ZOUT, z)) )
            return rv;
    }

    return UPM_SUCCESS;
}

upm_result_t mma7660_set_mode_active(const mma7660_context dev)
{
    assert(dev != NULL);

    uint8_t modeReg;
    if (mma7660_read_byte(dev, MMA7660_REG_MODE, &modeReg))
        return UPM_ERROR_OPERATION_FAILED;

    // The D2 (TON bit) should be cleared, and the MODE bit set

    modeReg &= ~MMA7660_MODE_TON;
    modeReg |= MMA7660_MODE_MODE;

    return mma7660_write_byte(dev, MMA7660_REG_MODE, modeReg);
}

upm_result_t mma7660_set_mode_standby(const mma7660_context dev)
{
    assert(dev != NULL);

    uint8_t modeReg;
    if (mma7660_read_byte(dev, MMA7660_REG_MODE, &modeReg))
        return UPM_ERROR_OPERATION_FAILED;

    // the D0 (mode bit) and D2 (TON bit) should be cleared.

    modeReg &= ~MMA7660_MODE_TON;
    modeReg &= ~MMA7660_MODE_MODE;

    return mma7660_write_byte(dev, MMA7660_REG_MODE, modeReg);
}

// read an axis value, verifying it's validity
upm_result_t mma7660_get_verified_axis(const mma7660_context dev,
                                       MMA7660_REG_T axis, int *val)
{
    assert(dev != NULL);

    *val = 0;
    // We only want one of the 3 axes

    if (axis > 2)
    {
        printf("%s: axis must be 0, 1, or 2.\n", __FUNCTION__);
        return UPM_ERROR_OUT_OF_RANGE;
    }

    // we need to check the alert bit and sign bits if the alert bit is
    // set, this means that the register was being updated when the
    // register was read, so re-read until it's clear.

    uint8_t value = 0;
    do {
        if (mma7660_read_byte(dev, axis, &value))
            return UPM_ERROR_OPERATION_FAILED;

        // check alert bit
    } while (value & 0x40);

    // shift the sign bit over, and compensate
    *val = ((int8_t)(value << 2) / 4);

    return UPM_SUCCESS;
}

// read the tilt register, verifying it's validity
upm_result_t mma7660_get_verified_tilt(const mma7660_context dev,
                                       uint8_t *val)
{
    assert(dev != NULL);

    // we need to check the alert bit and sign bits if the alert bit is
    // set, this means that the register was being updated when the
    // register was read, so re-read until it's clear.

    do {
        if (mma7660_read_byte(dev, MMA7660_REG_TILT, val))
            return UPM_ERROR_OPERATION_FAILED;

        // check alert bit
    } while (*val & 0x40);

    return UPM_SUCCESS;
}

upm_result_t mma7660_tilt_back_front(const mma7660_context dev,
                                     uint8_t *bits)
{
    assert(dev != NULL);

    if (mma7660_get_verified_tilt(dev, bits))
        return UPM_ERROR_OPERATION_FAILED;

    // mask off the bits we don't care about
    *bits &= 0x03;

    return UPM_SUCCESS;
}

upm_result_t mma7660_tilt_landscape_portrait(const mma7660_context dev,
                                             uint8_t *bits)
{
    assert(dev != NULL);

    if (mma7660_get_verified_tilt(dev, bits))
        return UPM_ERROR_OPERATION_FAILED;

    // mask off the bits we don't care about
    *bits >>= 2;
    *bits &= 0x07;

    return UPM_SUCCESS;
}

upm_result_t mma7660_tilt_tap(const mma7660_context dev, bool *tap)
{
    assert(dev != NULL);

    uint8_t val = 0;
    if (mma7660_get_verified_tilt(dev, &val))
        return UPM_ERROR_OPERATION_FAILED;

    if (val & 0x20)
        *tap = true;
    else
        *tap = false;

    return UPM_SUCCESS;
}

upm_result_t mma7660_tilt_shake(const mma7660_context dev, bool *shake)
{
    assert(dev != NULL);

    uint8_t val = 0;
    if (mma7660_get_verified_tilt(dev, &val))
        return UPM_ERROR_OPERATION_FAILED;

    if (val & 0x80)
        *shake = true;
    else
        *shake = false;

    return UPM_SUCCESS;
}

upm_result_t mma7660_install_isr(const mma7660_context dev, int pin,
                                 void (*isr)(void *), void *arg)
{
    assert(dev != NULL);

    mma7660_uninstall_isr(dev);

    if ( !(dev->gpio = mraa_gpio_init(pin)) )
    {
        printf("%s: mraa_gpio_init failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN);

    // install our interrupt handler
    mraa_gpio_isr(dev->gpio, MRAA_GPIO_EDGE_RISING,
                  isr, arg);
    dev->isrInstalled = true;

    return UPM_SUCCESS;
}

void mma7660_uninstall_isr(const mma7660_context dev)
{
    assert(dev != NULL);

    if (!dev->isrInstalled)
        return;

    mraa_gpio_isr_exit(dev->gpio);
    dev->isrInstalled = false;
    mraa_gpio_close(dev->gpio);
    dev->gpio = NULL;
}

upm_result_t mma7660_set_interrupt_bits(const mma7660_context dev,
                                        uint8_t ibits)
{
    assert(dev != NULL);

    return mma7660_write_byte(dev, MMA7660_REG_INTSU, ibits);
}

upm_result_t mma7660_set_sample_rate(const mma7660_context dev,
                                     MMA7660_AUTOSLEEP_T sr)
{
    assert(dev != NULL);

    return mma7660_write_byte(dev, MMA7660_REG_SR, sr);
}

upm_result_t mma7660_get_acceleration(const mma7660_context dev,
                                      float *ax, float *ay, float *az)
{
    assert(dev != NULL);

    int x, y, z;

    if (mma7660_get_raw_values(dev, &x, &y, &z))
    {
        printf("%s: mma7660_get_raw_values() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // 21.33, typical counts/g

    if (ax)
        *ax = x/21.33;
    if (ay)
        *ay = y/21.33;
    if (az)
        *az = z/21.33;

    return UPM_SUCCESS;
}
