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
#include "lis2ds12.h"

// macro for converting a uint8_t low/high pair into a float
#define INT16_TO_FLOAT(h, l) \
    (float)( (int16_t)( (l) | ((h) << 8) ) )

// some useful macros to save on typing and text wrapping
#undef _SHIFT
#define _SHIFT(x) (_LIS2DS12_##x##_SHIFT)

#undef _MASK
#define _MASK(x) (_LIS2DS12_##x##_MASK)

#undef _SHIFTMASK
#define _SHIFTMASK(x) (_MASK(x) << _SHIFT(x))

// SPI CS on and off functions
static void _csOn(const lis2ds12_context dev)
{
    assert(dev != NULL);

    if (dev->gpioCS)
        mraa_gpio_write(dev->gpioCS, 0);
}

static void _csOff(const lis2ds12_context dev)
{
    assert(dev != NULL);

    if (dev->gpioCS)
        mraa_gpio_write(dev->gpioCS, 1);
}

// init
lis2ds12_context lis2ds12_init(int bus, int addr, int cs)
{
    lis2ds12_context dev =
        (lis2ds12_context)malloc(sizeof(struct _lis2ds12_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _lis2ds12_context));

    // make sure MRAA is initialized
    if (mraa_init() != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed.\n", __FUNCTION__);
        lis2ds12_close(dev);
        return NULL;
    }

    if (addr < 0)
    {
        // SPI
        if (!(dev->spi = mraa_spi_init(bus)))
        {
            printf("%s: mraa_spi_init() failed.\n", __FUNCTION__);
            lis2ds12_close(dev);
            return NULL;
        }

        // Only create cs context if we are actually using a valid pin.
        // A hardware controlled pin should specify cs as -1.
        if (cs >= 0)
        {
            if (!(dev->gpioCS = mraa_gpio_init(cs)))
            {
                printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
                lis2ds12_close(dev);
                return NULL;
            }
            mraa_gpio_dir(dev->gpioCS, MRAA_GPIO_OUT);
        }

        mraa_spi_mode(dev->spi, MRAA_SPI_MODE0);
        if (mraa_spi_frequency(dev->spi, 5000000))
        {
            printf("%s: mraa_spi_frequency() failed.\n", __FUNCTION__);
            lis2ds12_close(dev);
            return NULL;
        }
    }
    else
    {
        // I2C
        if (!(dev->i2c = mraa_i2c_init(bus)))
        {
            printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
            lis2ds12_close(dev);
            return NULL;
        }

        if (mraa_i2c_address(dev->i2c, addr))
        {
            printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
            lis2ds12_close(dev);
            return NULL;
        }
    }

    // check the chip id

    uint8_t chipID = lis2ds12_get_chip_id(dev);
    if (chipID != LIS2DS12_CHIPID)
    {
        printf("%s: invalid chip id: %02x.  Expected %02x\n",
               __FUNCTION__, chipID, LIS2DS12_CHIPID);
        lis2ds12_close(dev);
        return NULL;
    }

    // reset
    if (lis2ds12_reset(dev))
    {
        printf("%s: lis2ds12_reset() failed.\n", __FUNCTION__);
        lis2ds12_close(dev);
        return NULL;
    }

    // call devinit with default options
    if (lis2ds12_devinit(dev, LIS2DS12_ODR_100HZ, LIS2DS12_FS_2G))
    {
        printf("%s: lis2ds12_devinit() failed.\n", __FUNCTION__);
        lis2ds12_close(dev);
        return NULL;
    }

    return dev;
}

void lis2ds12_close(lis2ds12_context dev)
{
    assert(dev != NULL);

    lis2ds12_uninstall_isr(dev, LIS2DS12_INTERRUPT_INT1);
    lis2ds12_uninstall_isr(dev, LIS2DS12_INTERRUPT_INT2);

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);
    if (dev->spi)
        mraa_spi_stop(dev->spi);
    if (dev->gpioCS)
        mraa_gpio_close(dev->gpioCS);

    free(dev);
}

upm_result_t lis2ds12_devinit(const lis2ds12_context dev,
                              LIS2DS12_ODR_T odr,
                              LIS2DS12_FS_T fs)
{
    assert(dev != NULL);

    // enable register auto-increment
    uint8_t reg = lis2ds12_read_reg(dev, LIS2DS12_REG_CTRL2);
    reg |= LIS2DS12_CTRL2_IF_ADD_INC;

    if (lis2ds12_write_reg(dev, LIS2DS12_REG_CTRL2, reg))
        return UPM_ERROR_OPERATION_FAILED;

    // set our ODR and FS.  Disable output high-pass filter.
    if (lis2ds12_set_odr(dev, odr)
        || lis2ds12_set_full_scale(dev, fs)
        || lis2ds12_enable_hp_filtering(dev, false))
    {
        printf("%s: failed to set configuration parameters.\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // settle
    upm_delay_ms(50);

    return UPM_SUCCESS;
}

upm_result_t lis2ds12_set_odr(const lis2ds12_context dev,
                              LIS2DS12_ODR_T odr)
{
    assert(dev != NULL);

    bool hf_mode = false;
    uint8_t reg = lis2ds12_read_reg(dev, LIS2DS12_REG_CTRL1);

    // mask out ODR bits
    reg &= ~_SHIFTMASK(CTRL1_ODR);

    // we encoded an extra bit in LIS2DS12_ODR_T indicating an HF
    // mode.  Check for it here.
    if ((int)odr > (int)_MASK(CTRL1_ODR))
        hf_mode = true;

    // mask it off and set it
    odr &= _MASK(CTRL1_ODR);
    reg |= (odr << _SHIFT(CTRL1_ODR));

    // set the HF bit appropriately
    if (hf_mode)
        reg |= LIS2DS12_CTRL1_HF_ODR;
    else
        reg &= ~LIS2DS12_CTRL1_HF_ODR;

    // we always set the BDU (Block Data Update) bit
    reg |= LIS2DS12_CTRL1_BDU;

    if (lis2ds12_write_reg(dev, LIS2DS12_REG_CTRL1, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t lis2ds12_set_full_scale(const lis2ds12_context dev,
                                     LIS2DS12_FS_T fs)
{
    assert(dev != NULL);

    uint8_t reg = lis2ds12_read_reg(dev, LIS2DS12_REG_CTRL1);

    // mask out FS bits, add our own
    reg &= ~_SHIFTMASK(CTRL1_FS);
    reg |= (fs << _SHIFT(CTRL1_FS));

    if (lis2ds12_write_reg(dev, LIS2DS12_REG_CTRL1, reg))
        return UPM_ERROR_OPERATION_FAILED;

    // assumes a 16b resolution
    switch(fs)
    {
    case LIS2DS12_FS_2G:
        dev->accScale = 0.061;
        break;

    case LIS2DS12_FS_4G:
        dev->accScale = 0.122;
        break;

    case LIS2DS12_FS_8G:
        dev->accScale = 0.244;
        break;

    case LIS2DS12_FS_16G:
        dev->accScale = 0.488;
        break;

    }

    return UPM_SUCCESS;
}

upm_result_t lis2ds12_update(const lis2ds12_context dev)
{
    assert(dev != NULL);

    int bufLen = 6; // max
    uint8_t buf[bufLen];

    if (lis2ds12_read_regs(dev, LIS2DS12_REG_OUT_X_L, buf, bufLen) != bufLen)
    {
        printf("%s: lis2ds12_read_regs() failed to read %d bytes\n",
               __FUNCTION__, bufLen);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // x                       msb     lsb
    dev->accX = INT16_TO_FLOAT(buf[1], buf[0]);

    // y
    dev->accY = INT16_TO_FLOAT(buf[3], buf[2]);

    // z
    dev->accZ = INT16_TO_FLOAT(buf[5], buf[4]);

    // get the temperature...

    int8_t temp = 0;
    temp = (int8_t)lis2ds12_read_reg(dev, LIS2DS12_REG_OUT_T);

    // 1K/LSB, 25C center point
    dev->temperature = ((float)temp / 256.0) + 25.0;

    return UPM_SUCCESS;
}

uint8_t lis2ds12_read_reg(const lis2ds12_context dev, uint8_t reg)
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

int lis2ds12_read_regs(const lis2ds12_context dev, uint8_t reg,
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

upm_result_t lis2ds12_write_reg(const lis2ds12_context dev,
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

uint8_t lis2ds12_get_chip_id(const lis2ds12_context dev)
{
    assert(dev != NULL);

    return lis2ds12_read_reg(dev, LIS2DS12_REG_WHO_AM_I);
}

void lis2ds12_get_accelerometer(const lis2ds12_context dev,
                                float *x, float *y, float *z)
{
    assert(dev != NULL);

    float scale = dev->accScale / 1000.0;

    if (x)
        *x = dev->accX * scale;

    if (y)
        *y = dev->accY * scale;

    if (z)
        *z = dev->accZ * scale;
}

float lis2ds12_get_temperature(const lis2ds12_context dev)
{
    assert(dev != NULL);

    return dev->temperature;
}

upm_result_t lis2ds12_reset(const lis2ds12_context dev)
{
    assert(dev != NULL);

    uint8_t reg = lis2ds12_read_reg(dev, LIS2DS12_REG_CTRL2);

    reg |= LIS2DS12_CTRL2_SOFT_RESET;

    if (lis2ds12_write_reg(dev, LIS2DS12_REG_CTRL2, reg))
        return UPM_ERROR_OPERATION_FAILED;

    upm_delay_ms(100);

    return UPM_SUCCESS;
}

upm_result_t lis2ds12_enable_hp_filtering(const lis2ds12_context dev,
                                          bool filter)
{
    assert(dev != NULL);

    uint8_t reg = lis2ds12_read_reg(dev, LIS2DS12_REG_CTRL2);

    if (filter)
        reg |= LIS2DS12_CTRL2_FDS_SLOPE;
    else
        reg &= ~LIS2DS12_CTRL2_FDS_SLOPE;

    if (lis2ds12_write_reg(dev, LIS2DS12_REG_CTRL2, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t lis2ds12_enable_interrupt_latching(const lis2ds12_context dev,
                                                bool latch)
{
    assert(dev != NULL);

    uint8_t reg = lis2ds12_read_reg(dev, LIS2DS12_REG_CTRL3);

    if (latch)
        reg |= LIS2DS12_CTRL3_LIR;
    else
        reg &= ~LIS2DS12_CTRL3_LIR;

    if (lis2ds12_write_reg(dev, LIS2DS12_REG_CTRL3, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t lis2ds12_set_interrupt_active_high(const lis2ds12_context dev,
                                                bool high)
{
    assert(dev != NULL);

    uint8_t reg = lis2ds12_read_reg(dev, LIS2DS12_REG_CTRL3);

    if (high)
        reg &= ~LIS2DS12_CTRL3_H_LACTIVE;
    else
        reg |= LIS2DS12_CTRL3_H_LACTIVE;

    if (lis2ds12_write_reg(dev, LIS2DS12_REG_CTRL3, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t lis2ds12_set_interrupt_push_pull(const lis2ds12_context dev,
                                              bool pp)
{
    assert(dev != NULL);

    uint8_t reg = lis2ds12_read_reg(dev, LIS2DS12_REG_CTRL3);

    if (pp)
        reg &= ~LIS2DS12_CTRL3_PP_OD;
    else
        reg |= LIS2DS12_CTRL3_PP_OD;

    if (lis2ds12_write_reg(dev, LIS2DS12_REG_CTRL3, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t lis2ds12_set_int1_config(const lis2ds12_context dev,
                                      uint8_t cfg)
{
    assert(dev != NULL);

    if (lis2ds12_write_reg(dev, LIS2DS12_REG_CTRL4, cfg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t lis2ds12_set_int2_config(const lis2ds12_context dev,
                                      uint8_t cfg)
{
    assert(dev != NULL);

    if (lis2ds12_write_reg(dev, LIS2DS12_REG_CTRL5, cfg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t lis2ds12_get_status(const lis2ds12_context dev)
{
    assert(dev != NULL);

    return lis2ds12_read_reg(dev, LIS2DS12_REG_STATUS);
}

upm_result_t lis2ds12_install_isr(const lis2ds12_context dev,
                                  LIS2DS12_INTERRUPT_PINS_T intr, int gpio,
                                  mraa_gpio_edge_t level,
                                  void (*isr)(void *), void *arg)
{
    assert(dev != NULL);

    // delete any existing ISR and GPIO context for this interrupt
    lis2ds12_uninstall_isr(dev, intr);

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
    case LIS2DS12_INTERRUPT_INT1:
        dev->gpioINT1 = gpio_isr;
        break;

    case LIS2DS12_INTERRUPT_INT2:
        dev->gpioINT2 = gpio_isr;
        break;
    }

    return UPM_SUCCESS;
}

void lis2ds12_uninstall_isr(const lis2ds12_context dev,
                            LIS2DS12_INTERRUPT_PINS_T intr)
{
    assert(dev != NULL);

    switch (intr)
    {
    case LIS2DS12_INTERRUPT_INT1:
        if (dev->gpioINT1)
        {
            mraa_gpio_isr_exit(dev->gpioINT1);
            mraa_gpio_close(dev->gpioINT1);
            dev->gpioINT1 = NULL;
        }
        break;

    case LIS2DS12_INTERRUPT_INT2:
        if (dev->gpioINT2)
        {
            mraa_gpio_isr_exit(dev->gpioINT2);
            mraa_gpio_close(dev->gpioINT2);
            dev->gpioINT2 = NULL;
        }
        break;
    }
}
