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
#include "lsm6dsl.h"

// macro for converting a uint8_t low/high pair into a float
#define INT16_TO_FLOAT(h, l)                    \
    (float)( (int16_t)( (l) | ((h) << 8) ) )

// some useful macros to save on typing and text wrapping
#undef _SHIFT
#define _SHIFT(x) (_LSM6DSL_##x##_SHIFT)

#undef _MASK
#define _MASK(x) (_LSM6DSL_##x##_MASK)

#undef _SHIFTMASK
#define _SHIFTMASK(x) (_MASK(x) << _SHIFT(x))

// SPI CS on and off functions
static void _csOn(const lsm6dsl_context dev)
{
    assert(dev != NULL);

    if (dev->gpioCS)
        mraa_gpio_write(dev->gpioCS, 0);
}

static void _csOff(const lsm6dsl_context dev)
{
    assert(dev != NULL);

    if (dev->gpioCS)
        mraa_gpio_write(dev->gpioCS, 1);
}

// init
lsm6dsl_context lsm6dsl_init(int bus, int addr, int cs)
{
    lsm6dsl_context dev =
        (lsm6dsl_context)malloc(sizeof(struct _lsm6dsl_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _lsm6dsl_context));

    // make sure MRAA is initialized
    if (mraa_init() != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed.\n", __FUNCTION__);
        lsm6dsl_close(dev);
        return NULL;
    }

    if (addr < 0)
    {
        // SPI
        if (!(dev->spi = mraa_spi_init(bus)))
        {
            printf("%s: mraa_spi_init() failed.\n", __FUNCTION__);
            lsm6dsl_close(dev);
            return NULL;
        }

        // Only create cs context if we are actually using a valid pin.
        // A hardware controlled pin should specify cs as -1.
        if (cs >= 0)
        {
            if (!(dev->gpioCS = mraa_gpio_init(cs)))
            {
                printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
                lsm6dsl_close(dev);
                return NULL;
            }
            mraa_gpio_dir(dev->gpioCS, MRAA_GPIO_OUT);
        }

        mraa_spi_mode(dev->spi, MRAA_SPI_MODE0);
        if (mraa_spi_frequency(dev->spi, 5000000))
        {
            printf("%s: mraa_spi_frequency() failed.\n", __FUNCTION__);
            lsm6dsl_close(dev);
            return NULL;
        }
    }
    else
    {
        // I2C
        if (!(dev->i2c = mraa_i2c_init(bus)))
        {
            printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
            lsm6dsl_close(dev);
            return NULL;
        }

        if (mraa_i2c_address(dev->i2c, addr))
        {
            printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
            lsm6dsl_close(dev);
            return NULL;
        }
    }

    // check the chip id

    uint8_t chipID = lsm6dsl_get_chip_id(dev);
    if (chipID != LSM6DSL_CHIPID)
    {
        printf("%s: invalid chip id: %02x.  Expected %02x\n",
               __FUNCTION__, chipID, LSM6DSL_CHIPID);
        lsm6dsl_close(dev);
        return NULL;
    }

    // reset
    if (lsm6dsl_reset(dev))
    {
        printf("%s: lsm6dsl_reset() failed.\n", __FUNCTION__);
        lsm6dsl_close(dev);
        return NULL;
    }

    // call devinit with default options
    if (lsm6dsl_devinit(dev, LSM6DSL_XL_ODR_104HZ, LSM6DSL_XL_FS_2G,
                        LSM6DSL_G_ODR_104HZ, LSM6DSL_G_FS_245DPS))
    {
        printf("%s: lsm6dsl_devinit() failed.\n", __FUNCTION__);
        lsm6dsl_close(dev);
        return NULL;
    }

    return dev;
}

void lsm6dsl_close(lsm6dsl_context dev)
{
    assert(dev != NULL);

    lsm6dsl_uninstall_isr(dev, LSM6DSL_INTERRUPT_INT1);
    lsm6dsl_uninstall_isr(dev, LSM6DSL_INTERRUPT_INT2);

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);
    if (dev->spi)
        mraa_spi_stop(dev->spi);
    if (dev->gpioCS)
        mraa_gpio_close(dev->gpioCS);

    free(dev);
}

upm_result_t lsm6dsl_devinit(const lsm6dsl_context dev,
                             LSM6DSL_XL_ODR_T acc_odr,
                             LSM6DSL_XL_FS_T acc_fs,
                             LSM6DSL_G_ODR_T gyr_odr,
                             LSM6DSL_G_FS_T gyr_fs)
{
    assert(dev != NULL);

    // enable register auto-increment and BDU (block data update)
    uint8_t reg = lsm6dsl_read_reg(dev, LSM6DSL_REG_CTRL3_C);
    reg |= (LSM6DSL_CTRL3_IF_INC | LSM6DSL_CTRL3_BDU);

    if (lsm6dsl_write_reg(dev, LSM6DSL_REG_CTRL3_C, reg))
        return UPM_ERROR_OPERATION_FAILED;

    // set our ODR, FS, and HP mode
    if (lsm6dsl_set_acc_odr(dev, acc_odr)
        || lsm6dsl_set_acc_full_scale(dev, acc_fs)
        || lsm6dsl_set_gyr_odr(dev, gyr_odr)
        || lsm6dsl_set_gyr_full_scale(dev, gyr_fs)
        || lsm6dsl_high_performance(dev, true))
    {
        printf("%s: failed to set configuration parameters.\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // settle
    upm_delay_ms(50);

    return UPM_SUCCESS;
}

upm_result_t lsm6dsl_set_acc_odr(const lsm6dsl_context dev,
                                 LSM6DSL_XL_ODR_T odr)
{
    assert(dev != NULL);

    uint8_t reg = lsm6dsl_read_reg(dev, LSM6DSL_REG_CTRL1_XL);

    // mask out ODR bits, add our own
    reg &= ~_SHIFTMASK(CTRL1_XL_ODR);
    reg |= (odr << _SHIFT(CTRL1_XL_ODR));

    if (lsm6dsl_write_reg(dev, LSM6DSL_REG_CTRL1_XL, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t lsm6dsl_set_gyr_odr(const lsm6dsl_context dev,
                                 LSM6DSL_G_ODR_T odr)
{
    assert(dev != NULL);

    uint8_t reg = lsm6dsl_read_reg(dev, LSM6DSL_REG_CTRL2_G);

    // mask out ODR bits, add our own
    reg &= ~_SHIFTMASK(CTRL2_G_ODR);
    reg |= (odr << _SHIFT(CTRL2_G_ODR));

    if (lsm6dsl_write_reg(dev, LSM6DSL_REG_CTRL2_G, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t lsm6dsl_set_acc_full_scale(const lsm6dsl_context dev,
                                        LSM6DSL_XL_FS_T fs)
{
    assert(dev != NULL);

    uint8_t reg = lsm6dsl_read_reg(dev, LSM6DSL_REG_CTRL1_XL);

    // mask out FS bits, add our own
    reg &= ~_SHIFTMASK(CTRL1_XL_FS);
    reg |= (fs << _SHIFT(CTRL1_XL_FS));

    if (lsm6dsl_write_reg(dev, LSM6DSL_REG_CTRL1_XL, reg))
        return UPM_ERROR_OPERATION_FAILED;

    // 16b resolution
    switch(fs)
    {
    case LSM6DSL_XL_FS_2G:
        dev->accScale = 0.061;
        break;

    case LSM6DSL_XL_FS_4G:
        dev->accScale = 0.122;
        break;

    case LSM6DSL_XL_FS_8G:
        dev->accScale = 0.244;
        break;

    case LSM6DSL_XL_FS_16G:
        dev->accScale = 0.488;
        break;
    }

    return UPM_SUCCESS;
}

upm_result_t lsm6dsl_set_gyr_full_scale(const lsm6dsl_context dev,
                                        LSM6DSL_G_FS_T fs)
{
    assert(dev != NULL);

    uint8_t reg = lsm6dsl_read_reg(dev, LSM6DSL_REG_CTRL2_G);

    // mask out FS bits, add our own
    reg &= ~_SHIFTMASK(CTRL2_G_FS);

    if ((int)fs > (int)_MASK(CTRL2_G_FS))
        reg |= LSM6DSL_CTRL2_G_FS_125;
    else
        reg &= ~LSM6DSL_CTRL2_G_FS_125;

    // mask off the virtual bit.  The end result will be 245dps (0),
    // if it was tagged with the virtual bit.
    fs &= _MASK(CTRL2_G_FS);
    // add it to the register
    reg |= (fs << _SHIFT(CTRL2_G_FS));

    if (lsm6dsl_write_reg(dev, LSM6DSL_REG_CTRL2_G, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t lsm6dsl_update(const lsm6dsl_context dev)
{
    assert(dev != NULL);

    int bufLen = 14;
    uint8_t buf[bufLen];

    if (lsm6dsl_read_regs(dev, LSM6DSL_REG_OUT_TEMP_L,
                          buf, bufLen) != bufLen)
    {
        printf("%s: lsm6dsl_read_regs() failed to read %d bytes\n",
               __FUNCTION__, bufLen);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // temperature is first           msb     lsb
    dev->temperature = INT16_TO_FLOAT(buf[1], buf[0]);

    // gyroscope
    dev->gyrX = INT16_TO_FLOAT(buf[3], buf[2]);
    dev->gyrY = INT16_TO_FLOAT(buf[5], buf[4]);
    dev->gyrZ = INT16_TO_FLOAT(buf[7], buf[6]);

    // accelerometer
    dev->accX = INT16_TO_FLOAT(buf[9], buf[8]);
    dev->accY = INT16_TO_FLOAT(buf[11], buf[10]);
    dev->accZ = INT16_TO_FLOAT(buf[13], buf[12]);

    return UPM_SUCCESS;
}

uint8_t lsm6dsl_read_reg(const lsm6dsl_context dev, uint8_t reg)
{
    assert(dev != NULL);

    if (dev->spi)
    {
        reg |= 0x80; // needed for read
        uint8_t pkt[2] = {reg, 0};

        _csOn(dev);
        if (mraa_spi_transfer_buf(dev->spi, pkt, pkt, 2))
        {
            _csOff(dev);
            printf("%s: mraa_spi_transfer_buf() failed.\n", __FUNCTION__);
            return 0xff;
        }
        _csOff(dev);

        return pkt[1];
    }
    else
        return (uint8_t)mraa_i2c_read_byte_data(dev->i2c, reg);
}

int lsm6dsl_read_regs(const lsm6dsl_context dev, uint8_t reg,
                      uint8_t *buffer, int len)
{
    assert(dev != NULL);

    if (dev->spi)
    {
        reg |= 0x80; // needed for read

        uint8_t sbuf[len + 1];
        memset((char *)sbuf, 0, len + 1);
        sbuf[0] = reg;

        _csOn(dev);
        if (mraa_spi_transfer_buf(dev->spi, sbuf, sbuf, len + 1))
        {
            _csOff(dev);
            printf("%s: mraa_spi_transfer_buf() failed.\n", __FUNCTION__);
            return -1;
        }
        _csOff(dev);

        // now copy it into user buffer
        for (int i=0; i<len; i++)
            buffer[i] = sbuf[i + 1];
    }
    else
    {
        if (mraa_i2c_read_bytes_data(dev->i2c, reg, buffer, len) != len)
            return -1;
    }

    return len;
}

upm_result_t lsm6dsl_write_reg(const lsm6dsl_context dev,
                               uint8_t reg, uint8_t val)
{
    assert(dev != NULL);

    if (dev->spi)
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

uint8_t lsm6dsl_get_chip_id(const lsm6dsl_context dev)
{
    assert(dev != NULL);

    return lsm6dsl_read_reg(dev, LSM6DSL_REG_WHO_AM_I);
}

void lsm6dsl_get_accelerometer(const lsm6dsl_context dev,
                               float *x, float *y, float *z)
{
    assert(dev != NULL);

    if (x)
        *x = dev->accX * dev->accScale / 1000.00;

    if (y)
        *y = dev->accY * dev->accScale / 1000.00;

    if (z)
        *z = dev->accZ * dev->accScale / 1000.00;
}

void lsm6dsl_get_gyroscope(const lsm6dsl_context dev,
                           float *x, float *y, float *z)
{
    assert(dev != NULL);

    if (x)
        *x = dev->gyrX / 1000.00;

    if (y)
        *y = dev->gyrY / 1000.00;

    if (z)
        *z = dev->gyrZ / 1000.00;
}

float lsm6dsl_get_temperature(const lsm6dsl_context dev)
{
    assert(dev != NULL);

    // this seems to work, but I sure wish they would document this in
    // their DS's
    return (dev->temperature / 65536.0) + 25.0;
}

upm_result_t lsm6dsl_high_performance(const lsm6dsl_context dev,
                                      bool enable)
{
    assert(dev != NULL);

    uint8_t reg = lsm6dsl_read_reg(dev, LSM6DSL_REG_CTRL6_C);

    // acc
    if (enable)
        reg &= ~LSM6DSL_CTRL6_XL_HM_MODE;
    else
        reg |= LSM6DSL_CTRL6_XL_HM_MODE;

    if (lsm6dsl_write_reg(dev, LSM6DSL_REG_CTRL6_C, reg))
        return UPM_ERROR_OPERATION_FAILED;

    // gyr
    reg = lsm6dsl_read_reg(dev, LSM6DSL_REG_CTRL7_G);

    if (enable)
        reg &= ~LSM6DSL_CTRL7_G_HM_MODE;
    else
        reg |= LSM6DSL_CTRL7_G_HM_MODE;

    if (lsm6dsl_write_reg(dev, LSM6DSL_REG_CTRL7_G, reg))
        return UPM_ERROR_OPERATION_FAILED;

    upm_delay_ms(100);

    return UPM_SUCCESS;
}

upm_result_t lsm6dsl_reset(const lsm6dsl_context dev)
{
    assert(dev != NULL);

    uint8_t reg = lsm6dsl_read_reg(dev, LSM6DSL_REG_CTRL3_C);

    reg |= LSM6DSL_CTRL3_SW_RESET;

    if (lsm6dsl_write_reg(dev, LSM6DSL_REG_CTRL3_C, reg))
        return UPM_ERROR_OPERATION_FAILED;

    upm_delay_ms(100);

    return UPM_SUCCESS;
}

upm_result_t lsm6dsl_set_interrupt_active_high(const lsm6dsl_context dev,
                                               bool high)
{
    assert(dev != NULL);

    uint8_t reg = lsm6dsl_read_reg(dev, LSM6DSL_REG_CTRL3_C);

    if (high)
        reg &= ~LSM6DSL_CTRL3_H_LACTIVE;
    else
        reg |= LSM6DSL_CTRL3_H_LACTIVE;

    if (lsm6dsl_write_reg(dev, LSM6DSL_REG_CTRL3_C, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t lsm6dsl_set_interrupt_push_pull(const lsm6dsl_context dev,
                                             bool pp)
{
    assert(dev != NULL);

    uint8_t reg = lsm6dsl_read_reg(dev, LSM6DSL_REG_CTRL3_C);

    if (pp)
        reg &= ~LSM6DSL_CTRL3_PP_OD;
    else
        reg |= LSM6DSL_CTRL3_PP_OD;

    if (lsm6dsl_write_reg(dev, LSM6DSL_REG_CTRL3_C, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t lsm6dsl_get_status(const lsm6dsl_context dev)
{
    assert(dev != NULL);

    return lsm6dsl_read_reg(dev, LSM6DSL_REG_STATUS);
}

upm_result_t lsm6dsl_set_acc_offsets(const lsm6dsl_context dev,
                                     int x, int y, int z, bool weight)
{
    assert(dev != NULL);

    // narrowing conversion to int8_t
    int8_t ix = (int8_t)x;
    int8_t iy = (int8_t)y;
    int8_t iz = (int8_t)z;

    if (lsm6dsl_write_reg(dev, LSM6DSL_REG_X_OFS_USR, (uint8_t)ix))
        return UPM_ERROR_OPERATION_FAILED;
    if (lsm6dsl_write_reg(dev, LSM6DSL_REG_Y_OFS_USR, (uint8_t)iy))
        return UPM_ERROR_OPERATION_FAILED;
    if (lsm6dsl_write_reg(dev, LSM6DSL_REG_Z_OFS_USR, (uint8_t)iz))
        return UPM_ERROR_OPERATION_FAILED;

    // now set the weighting...
    uint8_t reg = lsm6dsl_read_reg(dev, LSM6DSL_REG_CTRL6_C);
    if (weight)
        reg |= LSM6DSL_CTRL6_USR_OFF_W;
    else
        reg &= ~LSM6DSL_CTRL6_USR_OFF_W;

    if (lsm6dsl_write_reg(dev, LSM6DSL_REG_CTRL6_C, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t lsm6dsl_install_isr(const lsm6dsl_context dev,
                                 LSM6DSL_INTERRUPT_PINS_T intr, int gpio,
                                 mraa_gpio_edge_t level,
                                 void (*isr)(void *), void *arg)
{
    assert(dev != NULL);

    // delete any existing ISR and GPIO context for this interrupt
    lsm6dsl_uninstall_isr(dev, intr);

    mraa_gpio_context gpio_isr = NULL;

    // create gpio context
    if (!(gpio_isr = mraa_gpio_init(gpio)))
    {
        printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    mraa_gpio_dir(gpio_isr, MRAA_GPIO_IN);

    if (mraa_gpio_isr(gpio_isr, level, isr, arg))
    {
        mraa_gpio_close(gpio_isr);
        printf("%s: mraa_gpio_isr() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    switch (intr)
    {
    case LSM6DSL_INTERRUPT_INT1:
        dev->gpioINT1 = gpio_isr;
        break;

    case LSM6DSL_INTERRUPT_INT2:
        dev->gpioINT2 = gpio_isr;
        break;
    }

    return UPM_SUCCESS;
}

void lsm6dsl_uninstall_isr(const lsm6dsl_context dev,
                           LSM6DSL_INTERRUPT_PINS_T intr)
{
    assert(dev != NULL);

    switch (intr)
    {
    case LSM6DSL_INTERRUPT_INT1:
        if (dev->gpioINT1)
        {
            mraa_gpio_isr_exit(dev->gpioINT1);
            mraa_gpio_close(dev->gpioINT1);
            dev->gpioINT1 = NULL;
        }
        break;

    case LSM6DSL_INTERRUPT_INT2:
        if (dev->gpioINT2)
        {
            mraa_gpio_isr_exit(dev->gpioINT2);
            mraa_gpio_close(dev->gpioINT2);
            dev->gpioINT2 = NULL;
        }
        break;
    }
}
