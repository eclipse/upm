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
#include "bmg160.h"

// macro for converting a uint8_t low/high pair into a float
#define INT16_TO_FLOAT(h, l) \
    (float)( (int16_t)( (l) | ((h) << 8) ) )

// SPI CS on and off functions
static void _csOn(const bmg160_context dev)
{
    assert(dev != NULL);

    if (dev->gpioCS)
        mraa_gpio_write(dev->gpioCS, 0);
}

static void _csOff(const bmg160_context dev)
{
    assert(dev != NULL);

    if (dev->gpioCS)
        mraa_gpio_write(dev->gpioCS, 1);
}

// init
bmg160_context bmg160_init(int bus, int addr, int cs)
{
    bmg160_context dev =
        (bmg160_context)malloc(sizeof(struct _bmg160_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _bmg160_context));

    // make sure MRAA is initialized
    if (mraa_init() != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed.\n", __FUNCTION__);
        bmg160_close(dev);
        return NULL;
    }

    if (addr < 0)
        dev->isSPI = true;

    if (dev->isSPI)
    {
        if (!(dev->spi = mraa_spi_init(bus)))
        {
            printf("%s: mraa_spi_init() failed.\n", __FUNCTION__);
            bmg160_close(dev);
            return NULL;
        }

        // Only create cs context if we are actually using a valid pin.
        // A hardware controlled pin should specify cs as -1.
        if (cs >= 0)
        {
            if (!(dev->gpioCS = mraa_gpio_init(cs)))
            {
                printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
                bmg160_close(dev);
                return NULL;
            }
            mraa_gpio_dir(dev->gpioCS, MRAA_GPIO_OUT);
        }

        mraa_spi_mode(dev->spi, MRAA_SPI_MODE0);
        if (mraa_spi_frequency(dev->spi, 5000000))
        {
            printf("%s: mraa_spi_frequency() failed.\n", __FUNCTION__);
            bmg160_close(dev);
            return NULL;
        }
    }
    else
    {
        // I2C

        if (!(dev->i2c = mraa_i2c_init(bus)))
        {
            printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
            bmg160_close(dev);
            return NULL;
        }

        if (mraa_i2c_address(dev->i2c, addr))
        {
            printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
            bmg160_close(dev);
            return NULL;
        }
    }

    // check the chip id

    uint8_t chipID = bmg160_get_chip_id(dev);
    if (chipID != BMG160_CHIPID)
    {
        printf("%s: invalid chip id: %02x.  Expected %02x\n",
               __FUNCTION__, chipID, BMG160_CHIPID);
        bmg160_close(dev);
        return NULL;
    }

    // call devinit with default options
    if (bmg160_devinit(dev, BMG160_POWER_MODE_NORMAL, BMG160_RANGE_250,
                       BMG160_BW_400_47))
    {
        printf("%s: bmg160_devinit() failed.\n", __FUNCTION__);
        bmg160_close(dev);
        return NULL;
    }

    return dev;
}

void bmg160_close(bmg160_context dev)
{
    assert(dev != NULL);

    bmg160_uninstall_isr(dev, BMG160_INTERRUPT_INT1);
    bmg160_uninstall_isr(dev, BMG160_INTERRUPT_INT2);

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);
    if (dev->spi)
        mraa_spi_stop(dev->spi);
    if (dev->gpioCS)
        mraa_gpio_close(dev->gpioCS);

    free(dev);
}

upm_result_t bmg160_devinit(const bmg160_context dev,
                            BMG160_POWER_MODE_T pwr,
                            BMG160_RANGE_T range,
                            BMG160_BW_T bw)
{
    assert(dev != NULL);

    if (bmg160_set_power_mode(dev, pwr))
    {
        printf("%s: bmg160_set_power_mode() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    upm_delay_ms(50); // 50ms, in case we are waking up

    // set our range and bandwidth, make sure register shadowing is
    // enabled, enable output filtering, and set our FIFO config

    if (bmg160_set_range(dev, range)
        || bmg160_set_bandwidth(dev, bw)
        || bmg160_enable_register_shadowing(dev, true)
        || bmg160_enable_output_filtering(dev, true)
        || bmg160_fifo_config(dev, BMG160_FIFO_MODE_BYPASS,
                              BMG160_FIFO_DATA_SEL_XYZ))
    {
        printf("%s: failed to set configuration parameters.\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    bmg160_enable_fifo(dev, true);

    // settle
    upm_delay_ms(50);

    return UPM_SUCCESS;
}

upm_result_t bmg160_update(const bmg160_context dev)
{
    assert(dev != NULL);

    int bufLen = 7; // max, non-FIFO
    uint8_t startReg = BMG160_REG_RATE_X_LSB;

    if (dev->useFIFO)
    {
        bufLen = 6;
        startReg = BMG160_REG_FIFO_DATA;
    }

    uint8_t buf[bufLen];

    if (bmg160_read_regs(dev, startReg, buf, bufLen) != bufLen)
    {
        printf("%s: bmg160_read_regs() failed to read %d bytes\n",
               __FUNCTION__, bufLen);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // x                       msb     lsb
    dev->gyrX = INT16_TO_FLOAT(buf[1], buf[0]);

    // y
    dev->gyrY = INT16_TO_FLOAT(buf[3], buf[2]);

    // z
    dev->gyrZ = INT16_TO_FLOAT(buf[5], buf[4]);

    // get the temperature...

    int8_t temp = 0;
    if (dev->useFIFO)
    {
        // we have to read temperature separately...
        temp = (int8_t)bmg160_read_reg(dev, BMG160_REG_TEMP);
    }
    else
    {
        // we already got it
        temp = (int8_t)buf[6];
    }

    // .5K/LSB, 23C center point
    dev->temperature = ((float)temp / 2.0) + 23.0;

    return UPM_SUCCESS;
}

void bmg160_enable_fifo(const bmg160_context dev, bool useFIFO)
{
    assert(dev != NULL);

    dev->useFIFO = useFIFO;
}

uint8_t bmg160_read_reg(const bmg160_context dev, uint8_t reg)
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
            printf("%s: mraa_spi_transfer_buf() failed.\n", __FUNCTION__);
            return 0xff;
        }
        _csOff(dev);

        return pkt[1];
    }
    else
        return (uint8_t)mraa_i2c_read_byte_data(dev->i2c, reg);
}

int bmg160_read_regs(const bmg160_context dev, uint8_t reg,
                     uint8_t *buffer, int len)
{
    assert(dev != NULL);

    if (dev->isSPI)
    {
        reg |= 0x80; // needed for read

        uint8_t sbuf[len + 1];
        memset((char *)sbuf, 0, len + 1);
        sbuf[0] = reg;

        // We need to do it this way for edison - ie: use a single
        // transfer rather than breaking it up into two like we used to.
        // This means a buffer copy is now required, but that's the way
        // it goes.

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

upm_result_t bmg160_write_reg(const bmg160_context dev,
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

uint8_t bmg160_get_chip_id(const bmg160_context dev)
{
    assert(dev != NULL);

    return bmg160_read_reg(dev, BMG160_REG_CHIP_ID);
}

void bmg160_get_gyroscope(const bmg160_context dev,
                          float *x, float *y, float *z)
{
    assert(dev != NULL);

    if (x)
        *x = (dev->gyrX * dev->gyrScale) / 1000.0;

    if (y)
        *y = (dev->gyrY * dev->gyrScale) / 1000.0;

    if (z)
        *z = (dev->gyrZ * dev->gyrScale) / 1000.0;
}

float bmg160_get_temperature(const bmg160_context dev)
{
    assert(dev != NULL);

    return dev->temperature;
}

upm_result_t bmg160_reset(const bmg160_context dev)
{
    assert(dev != NULL);

    if (bmg160_write_reg(dev, BMG160_REG_SOFTRESET, BMG160_RESET_BYTE))
        return UPM_ERROR_OPERATION_FAILED;

    upm_delay(1);

    return UPM_SUCCESS;
}

upm_result_t bmg160_set_range(const bmg160_context dev,
                              BMG160_RANGE_T range)
{
    assert(dev != NULL);

    // we also have to write a fixed '0x10' to the high-order bits for
    // some reason (according to datasheet)
    uint8_t reg = range | (_BMG160_GYR_RANGE_FIXED_VALUE
                           << _BMG160_GYR_RANGE_FIXED_SHIFT);
    if (bmg160_write_reg(dev, BMG160_REG_GYR_RANGE, reg))
        return UPM_ERROR_OPERATION_FAILED;

    switch(range)
    {
    case BMG160_RANGE_125:
        dev->gyrScale = 3.8; // milli-degrees
        break;

    case BMG160_RANGE_250:
        dev->gyrScale = 7.6;
        break;

    case BMG160_RANGE_500:
        dev->gyrScale = 15.3;
        break;

    case BMG160_RANGE_1000:
        dev->gyrScale = 30.5;
        break;

    case BMG160_RANGE_2000:
        dev->gyrScale = 61.0;
        break;
    }

    return UPM_SUCCESS;
}

upm_result_t bmg160_set_bandwidth(const bmg160_context dev,
                                  BMG160_BW_T bw)
{
    assert(dev != NULL);

    if (bmg160_write_reg(dev, BMG160_REG_GYR_BW, bw))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bmg160_set_power_mode(const bmg160_context dev,
                                   BMG160_POWER_MODE_T power)
{
    assert(dev != NULL);

    // mask off reserved bits first
    uint8_t reg =
        bmg160_read_reg(dev, BMG160_REG_LPM1) & ~_BMG160_LPM1_RESERVED_BITS;

    reg &= ~(_BMG160_LPM1_POWER_MODE_MASK << _BMG160_LPM1_POWER_MODE_SHIFT);
    reg |= (power << _BMG160_LPM1_POWER_MODE_SHIFT);

    if (bmg160_write_reg(dev, BMG160_REG_LPM1, power))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bmg160_fifo_set_watermark(const bmg160_context dev, int wm)
{
    assert(dev != NULL);

    // mask off illegal values
    uint8_t reg = ((uint8_t)wm) & _BMG160_FIFO_CONFIG_0_WATER_MARK_MASK;

    if (bmg160_write_reg(dev, BMG160_REG_FIFO_CONFIG_0, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bmg160_fifo_config(const bmg160_context dev,
                                BMG160_FIFO_MODE_T mode,
                                BMG160_FIFO_DATA_SEL_T axes)
{
    assert(dev != NULL);

    uint8_t reg = ( (mode << _BMG160_FIFO_CONFIG_1_FIFO_MODE_SHIFT) |
                    (axes << _BMG160_FIFO_CONFIG_1_FIFO_DATA_SHIFT) );

    if (bmg160_write_reg(dev, BMG160_REG_FIFO_CONFIG_1, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bmg160_get_interrupt_enable0(const bmg160_context dev)
{
    assert(dev != NULL);

    return (bmg160_read_reg(dev, BMG160_REG_INT_EN_0)
            & ~_BMG160_INT_EN_0_RESERVED_BITS);
}

upm_result_t bmg160_set_interrupt_enable0(const bmg160_context dev,
                                          uint8_t bits)
{
    assert(dev != NULL);

    uint8_t reg = bits & ~_BMG160_INT_EN_0_RESERVED_BITS;

    if (bmg160_write_reg(dev, BMG160_REG_INT_EN_0, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bmg160_get_interrupt_map0(const bmg160_context dev)
{
    assert(dev != NULL);

    return bmg160_read_reg(dev, BMG160_REG_INT_MAP_0)
        & ~_BMG160_INT_MAP_0_RESERVED_BITS;
}

upm_result_t bmg160_set_interrupt_map0(const bmg160_context dev, uint8_t bits)
{
    assert(dev != NULL);

    uint8_t reg = bits & ~_BMG160_INT_MAP_0_RESERVED_BITS;

    if (bmg160_write_reg(dev, BMG160_REG_INT_MAP_0, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bmg160_get_interrupt_map1(const bmg160_context dev)
{
    assert(dev != NULL);

    return bmg160_read_reg(dev, BMG160_REG_INT_MAP_1);
}

upm_result_t bmg160_set_interrupt_map1(const bmg160_context dev, uint8_t bits)
{
    assert(dev != NULL);

    if (bmg160_write_reg(dev, BMG160_REG_INT_MAP_1, bits))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

// REG_INT_EN1, for some strange reason
uint8_t bmg160_get_interrupt_src(const bmg160_context dev)
{
    assert(dev != NULL);

    return (bmg160_read_reg(dev, BMG160_REG_INT_EN_1)
            & ~_BMG160_INT_EN_1_INT1_RESERVED_BITS);
}

upm_result_t bmg160_set_interrupt_src(const bmg160_context dev, uint8_t bits)
{
    assert(dev != NULL);

    uint8_t reg = bits & ~_BMG160_INT_EN_1_INT1_RESERVED_BITS;

    if (bmg160_write_reg(dev, BMG160_REG_INT_EN_1, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bmg160_get_interrupt_output_control(const bmg160_context dev)
{
    assert(dev != NULL);

    return (bmg160_read_reg(dev, BMG160_REG_INT_EN_1)
            & ~_BMG160_INT_EN_1_INT1_RESERVED_BITS);
}

upm_result_t bmg160_set_interrupt_output_control(const bmg160_context dev,
                                                 uint8_t bits)
{
    assert(dev != NULL);

    uint8_t reg = bits & ~_BMG160_INT_EN_1_INT1_RESERVED_BITS;

    if (bmg160_write_reg(dev, BMG160_REG_INT_EN_1, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bmg160_clear_interrupt_latches(const bmg160_context dev)
{
    assert(dev != NULL);

    uint8_t reg =
        bmg160_read_reg(dev, BMG160_REG_INT_RST_LATCH) & ~_BMG160_INT_RST_LATCH_RESERVED_BITS;

    reg |= BMG160_INT_RST_LATCH_RESET_INT;

    if (bmg160_write_reg(dev, BMG160_REG_INT_RST_LATCH, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

BMG160_RST_LATCH_T bmg160_get_interrupt_latch_behavior(const bmg160_context dev)
{
    assert(dev != NULL);

    uint8_t reg = bmg160_read_reg(dev, BMG160_REG_INT_RST_LATCH)
        & ~_BMG160_INT_RST_LATCH_RESERVED_BITS;

    reg &= (_BMG160_INT_RST_LATCH_MASK << _BMG160_INT_RST_LATCH_SHIFT);

    return (BMG160_RST_LATCH_T)reg;
}

upm_result_t bmg160_set_interrupt_latch_behavior(const bmg160_context dev,
                                                 BMG160_RST_LATCH_T latch)
{
    assert(dev != NULL);

    uint8_t reg =
        bmg160_read_reg(dev, BMG160_REG_INT_RST_LATCH) & ~_BMG160_INT_RST_LATCH_RESERVED_BITS;

    reg &= ~(_BMG160_INT_RST_LATCH_MASK << _BMG160_INT_RST_LATCH_SHIFT);
    reg |= (latch << _BMG160_INT_RST_LATCH_SHIFT);

    if (bmg160_write_reg(dev, BMG160_REG_INT_RST_LATCH, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bmg160_enable_register_shadowing(const bmg160_context dev,
                                              bool shadow)
{
    assert(dev != NULL);

    uint8_t reg =
        bmg160_read_reg(dev, BMG160_REG_RATE_HBW) & ~_BMG160_RATE_HBW_RESERVED_BITS;

    if (shadow)
        reg &= ~BMG160_RATE_HBW_SHADOW_DIS;
    else
        reg |= BMG160_RATE_HBW_SHADOW_DIS;

    if (bmg160_write_reg(dev, BMG160_REG_RATE_HBW, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bmg160_enable_output_filtering(const bmg160_context dev,
                                            bool filter)
{
    assert(dev != NULL);

    uint8_t reg =
        bmg160_read_reg(dev, BMG160_REG_RATE_HBW) & ~_BMG160_RATE_HBW_RESERVED_BITS;

    if (filter)
        reg &= ~BMG160_RATE_HBW_DATA_HIGH_BW;
    else
        reg |= BMG160_RATE_HBW_DATA_HIGH_BW;

    if (bmg160_write_reg(dev, BMG160_REG_RATE_HBW, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bmg160_get_interrupt_status0(const bmg160_context dev)
{
    assert(dev != NULL);

    return (bmg160_read_reg(dev, BMG160_REG_INT_STATUS_0)
            & ~_BMG160_INT_STATUS_0_RESERVED_BITS);
}

uint8_t bmg160_get_interrupt_status1(const bmg160_context dev)
{
    assert(dev != NULL);

    return (bmg160_read_reg(dev, BMG160_REG_INT_STATUS_1)
            & ~_BMG160_INT_STATUS_1_RESERVED_BITS);
}

uint8_t bmg160_get_interrupt_status2(const bmg160_context dev)
{
    assert(dev != NULL);

    return (bmg160_read_reg(dev, BMG160_REG_INT_STATUS_2)
            & ~_BMG160_INT_STATUS_2_RESERVED_BITS);
}

uint8_t bmg160_get_interrupt_status3(const bmg160_context dev)
{
    assert(dev != NULL);

    return (bmg160_read_reg(dev, BMG160_REG_INT_STATUS_3)
            & ~_BMG160_INT_STATUS_3_RESERVED_BITS);
}

upm_result_t bmg160_install_isr(const bmg160_context dev,
                                BMG160_INTERRUPT_PINS_T intr, int gpio,
                                mraa_gpio_edge_t level,
                                void (*isr)(void *), void *arg)
{
    assert(dev != NULL);

    // delete any existing ISR and GPIO context for this interrupt
    bmg160_uninstall_isr(dev, intr);

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
    case BMG160_INTERRUPT_INT1:
        dev->gpio1 = gpio_isr;
        break;

    case BMG160_INTERRUPT_INT2:
        dev->gpio2 = gpio_isr;
        break;
    }

    return UPM_SUCCESS;
}

void bmg160_uninstall_isr(const bmg160_context dev,
                          BMG160_INTERRUPT_PINS_T intr)
{
    assert(dev != NULL);

    switch (intr)
    {
    case BMG160_INTERRUPT_INT1:
        if (dev->gpio1)
        {
            mraa_gpio_isr_exit(dev->gpio1);
            mraa_gpio_close(dev->gpio1);
            dev->gpio1 = NULL;
        }
        break;

    case BMG160_INTERRUPT_INT2:
        if (dev->gpio2)
        {
            mraa_gpio_isr_exit(dev->gpio2);
            mraa_gpio_close(dev->gpio2);
            dev->gpio2 = NULL;
        }
        break;
    }
}
