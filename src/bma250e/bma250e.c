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
#include "bma250e.h"

// macro for converting a uint8_t low/high pair into a float
#define INT16_TO_FLOAT(h, l) \
    (float)( (int16_t)( (l) | ((h) << 8) ) )

// SPI CS on and off functions
static void _csOn(const bma250e_context dev)
{
    assert(dev != NULL);

    if (dev->gpioCS)
        mraa_gpio_write(dev->gpioCS, 0);
}

static void _csOff(const bma250e_context dev)
{
    assert(dev != NULL);

    if (dev->gpioCS)
        mraa_gpio_write(dev->gpioCS, 1);
}

// init
bma250e_context bma250e_init(int bus, int addr, int cs)
{
    bma250e_context dev =
        (bma250e_context)malloc(sizeof(struct _bma250e_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _bma250e_context));

    // make sure MRAA is initialized
    if (mraa_init() != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed.\n", __FUNCTION__);
        bma250e_close(dev);
        return NULL;
    }

    if (addr < 0)
        dev->isSPI = true;

    if (dev->isSPI)
    {
        if (!(dev->spi = mraa_spi_init(bus)))
        {
            printf("%s: mraa_spi_init() failed.\n", __FUNCTION__);
            bma250e_close(dev);
            return NULL;
        }

        // Only create cs context if we are actually using a valid pin.
        // A hardware controlled pin should specify cs as -1.
        if (cs >= 0)
        {
            if (!(dev->gpioCS = mraa_gpio_init(cs)))
            {
                printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
                bma250e_close(dev);
                return NULL;
            }
            mraa_gpio_dir(dev->gpioCS, MRAA_GPIO_OUT);
        }

        mraa_spi_mode(dev->spi, MRAA_SPI_MODE0);
        if (mraa_spi_frequency(dev->spi, 5000000))
        {
            printf("%s: mraa_spi_frequency() failed.\n", __FUNCTION__);
            bma250e_close(dev);
            return NULL;
        }
    }
    else
    {
        // I2C

        if (!(dev->i2c = mraa_i2c_init(bus)))
        {
            printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
            bma250e_close(dev);
            return NULL;
        }

        if (mraa_i2c_address(dev->i2c, addr))
        {
            printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
            bma250e_close(dev);
            return NULL;
        }
    }

    // check the chip id

    uint8_t chipID = bma250e_get_chip_id(dev);

    // check the various chips id's and set appropriate capabilities.
    // Bail if the chip id is unknown.
    switch (chipID)
    {
    case 0xf9: // standalone bma250e
        dev->resolution = BMA250E_RESOLUTION_10BITS;
        dev->fifoAvailable = true;

        break;

    case 0xfa: // bmx055, bmi055 variants, 12b resolution
        dev->resolution = BMA250E_RESOLUTION_12BITS;
        dev->fifoAvailable = true;

        break;

    case 0x03: // bmc050 variant, no FIFO, 12b resolution
        dev->resolution = BMA250E_RESOLUTION_12BITS;
        dev->fifoAvailable = false;

        break;

    default:
        printf("%s: invalid chip id: %02x.  Expected f9, fa, or 03\n",
               __FUNCTION__, chipID);
        bma250e_close(dev);
        return NULL;
    }

    // call devinit with default options
    if (bma250e_devinit(dev, BMA250E_POWER_MODE_NORMAL, BMA250E_RANGE_2G,
                        BMA250E_BW_250))
    {
        printf("%s: bma250e_devinit() failed.\n", __FUNCTION__);
        bma250e_close(dev);
        return NULL;
    }

    return dev;
}

void bma250e_close(bma250e_context dev)
{
    assert(dev != NULL);

    bma250e_uninstall_isr(dev, BMA250E_INTERRUPT_INT1);
    bma250e_uninstall_isr(dev, BMA250E_INTERRUPT_INT2);

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);
    if (dev->spi)
        mraa_spi_stop(dev->spi);
    if (dev->gpioCS)
        mraa_gpio_close(dev->gpioCS);

    free(dev);
}

upm_result_t bma250e_devinit(const bma250e_context dev,
                             BMA250E_POWER_MODE_T pwr,
                             BMA250E_RANGE_T range,
                             BMA250E_BW_T bw)
{
    assert(dev != NULL);

    if (bma250e_set_power_mode(dev, pwr))
    {
        printf("%s: bma250e_set_power_mode() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    upm_delay_ms(50); // 50ms, in case we are waking up

    // set our range and bandwidth, make sure register shadowing is
    // enabled, enable output filtering, and set our FIFO config

    if (bma250e_set_range(dev, range)
        || bma250e_set_bandwidth(dev, bw)
        || bma250e_enable_register_shadowing(dev, true)
        || bma250e_enable_output_filtering(dev, true)
        || bma250e_fifo_config(dev, BMA250E_FIFO_MODE_BYPASS,
                               BMA250E_FIFO_DATA_SEL_XYZ))
    {
        printf("%s: failed to set configuration parameters.\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    bma250e_enable_fifo(dev, true);

    // make sure low power mode LPM2 is enabled in case we go to low
    // power or suspend mode. LPM1 mode (the default) requires register
    // writes to be drastically slowed down when enabled, which we
    // cannot handle.
    bma250e_set_low_power_mode2(dev);

    // settle
    upm_delay_ms(50);

    return UPM_SUCCESS;
}

upm_result_t bma250e_update(const bma250e_context dev)
{
    assert(dev != NULL);

    int bufLen = 7; // max, non-FIFO
    uint8_t startReg = BMA250E_REG_ACCD_X_LSB;

    if (dev->useFIFO)
    {
        bufLen = 6;
        startReg = BMA250E_REG_FIFO_DATA;
    }

    uint8_t buf[bufLen];

    if (bma250e_read_regs(dev, startReg, buf, bufLen) != bufLen)
    {
        printf("%s: bma250e_read_regs() failed to read %d bytes\n",
               __FUNCTION__, bufLen);
        return UPM_ERROR_OPERATION_FAILED;
    }

    uint8_t mask = 0, shift = 0;
    float divisor = 1;

    switch (dev->resolution)
    {
    case BMA250E_RESOLUTION_10BITS:
        mask = _BMA250E_ACCD10_LSB_MASK;
        shift = _BMA250E_ACCD10_LSB_SHIFT;
        divisor = 64.0;

        break;

    case BMA250E_RESOLUTION_12BITS:
        mask = _BMA250E_ACCD12_LSB_MASK;
        shift = _BMA250E_ACCD12_LSB_SHIFT;
        divisor = 16.0;

        break;
    }

    // x                       msb     lsb
    dev->accX = INT16_TO_FLOAT(buf[1], (buf[0] & (mask << shift))) / divisor;

    // y
    dev->accY = INT16_TO_FLOAT(buf[3], (buf[2] & (mask << shift))) / divisor;

    // z
    dev->accZ = INT16_TO_FLOAT(buf[5], (buf[4] & (mask << shift))) / divisor;

    // get the temperature...

    int8_t temp = 0;
    if (dev->useFIFO)
    {
        // we have to read temperature separately...
        temp = (int8_t)bma250e_read_reg(dev, BMA250E_REG_TEMP);
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

void bma250e_enable_fifo(const bma250e_context dev, bool useFIFO)
{
    assert(dev != NULL);

    if (dev->fifoAvailable)
        dev->useFIFO = useFIFO;
}

uint8_t bma250e_read_reg(const bma250e_context dev, uint8_t reg)
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

int bma250e_read_regs(const bma250e_context dev, uint8_t reg,
                      uint8_t *buffer, int len)
{
    assert(dev != NULL);

    if (dev->isSPI)
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

upm_result_t bma250e_write_reg(const bma250e_context dev,
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
            printf("%s: mraa_spi_transfer_buf() failed.\n",
                   __FUNCTION__);

            return UPM_ERROR_OPERATION_FAILED;
        }
        _csOff(dev);
    }
    else
    {
        if (mraa_i2c_write_byte_data(dev->i2c, val, reg))
        {
            printf("%s: mraa_i2c_write_byte_data() failed.\n",
                   __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }
    }

    return UPM_SUCCESS;
}

uint8_t bma250e_get_chip_id(const bma250e_context dev)
{
    assert(dev != NULL);

    return bma250e_read_reg(dev, BMA250E_REG_CHIP_ID);
}

void bma250e_get_accelerometer(const bma250e_context dev,
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

float bma250e_get_temperature(const bma250e_context dev)
{
    assert(dev != NULL);

    return dev->temperature;
}

upm_result_t bma250e_reset(const bma250e_context dev)
{
    assert(dev != NULL);

    if (bma250e_write_reg(dev, BMA250E_REG_SOFTRESET, BMA250E_RESET_BYTE))
        return UPM_ERROR_OPERATION_FAILED;

    upm_delay(1);

    return UPM_SUCCESS;
}

upm_result_t bma250e_set_range(const bma250e_context dev,
                               BMA250E_RANGE_T range)
{
    assert(dev != NULL);

    if (bma250e_write_reg(dev, BMA250E_REG_PMU_RANGE, range))
        return UPM_ERROR_OPERATION_FAILED;

    switch (dev->resolution)
    {
    case BMA250E_RESOLUTION_10BITS:
        switch(range)
        {
        case BMA250E_RANGE_2G:
            dev->accScale = 3.91; // milli-gravities
            break;

        case BMA250E_RANGE_4G:
            dev->accScale = 7.81;
            break;

        case BMA250E_RANGE_8G:
            dev->accScale = 15.63;
            break;

        case BMA250E_RANGE_16G:
            dev->accScale = 31.25;
            break;
        }

        break;

    case BMA250E_RESOLUTION_12BITS:
        switch(range)
        {
        case BMA250E_RANGE_2G:
            dev->accScale = 0.98; // milli-gravities
            break;

        case BMA250E_RANGE_4G:
            dev->accScale = 1.95;
            break;

        case BMA250E_RANGE_8G:
            dev->accScale = 3.91;
            break;

        case BMA250E_RANGE_16G:
            dev->accScale = 7.81;
            break;
        }

        break;
    }

    return UPM_SUCCESS;
}

upm_result_t bma250e_set_bandwidth(const bma250e_context dev,
                                   BMA250E_BW_T bw)
{
    assert(dev != NULL);

    if (bma250e_write_reg(dev, BMA250E_REG_PMU_BW, bw))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bma250e_set_power_mode(const bma250e_context dev,
                                    BMA250E_POWER_MODE_T power)
{
    assert(dev != NULL);

    // mask off reserved bits first
    uint8_t reg =
        bma250e_read_reg(dev, BMA250E_REG_PMU_LPW)
        & ~_BMA250E_PMU_LPW_RESERVED_MASK;

    reg &= ~(_BMA250E_PMU_LPW_POWER_MODE_MASK
             << _BMA250E_PMU_LPW_POWER_MODE_SHIFT);
    reg |= (power << _BMA250E_PMU_LPW_POWER_MODE_SHIFT);

    if (bma250e_write_reg(dev, BMA250E_REG_PMU_LPW, power))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bma250e_fifo_set_watermark(const bma250e_context dev, int wm)
{
    assert(dev != NULL);

    if (!dev->fifoAvailable)
        return UPM_ERROR_NOT_SUPPORTED;

    // mask off illegal values
    uint8_t reg = ((uint8_t)wm) & _BMA250E_FIFO_CONFIG_0_WATER_MARK_MASK;

    if (bma250e_write_reg(dev, BMA250E_REG_FIFO_CONFIG_0, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bma250e_fifo_config(const bma250e_context dev,
                                 BMA250E_FIFO_MODE_T mode,
                                 BMA250E_FIFO_DATA_SEL_T axes)
{
    assert(dev != NULL);

    if (!dev->fifoAvailable)
        return UPM_ERROR_NOT_SUPPORTED;

    uint8_t reg = ( (mode << _BMA250E_FIFO_CONFIG_1_FIFO_MODE_SHIFT) |
                    (axes << _BMA250E_FIFO_CONFIG_1_FIFO_DATA_SHIFT) );

    if (bma250e_write_reg(dev, BMA250E_REG_FIFO_CONFIG_1, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bma250e_set_self_test(const bma250e_context dev,
                                   bool sign, bool amp,
                                   BMA250E_SELFTTEST_AXIS_T axis)
{
    assert(dev != NULL);

    uint8_t reg = (axis << _BMA250E_PMU_SELFTTEST_AXIS_SHIFT);

    if (amp)
        reg |= BMA250E_PMU_SELFTTEST_AMP;

    if (sign)
        reg |= BMA250E_PMU_SELFTTEST_SIGN;

    if (bma250e_write_reg(dev, BMA250E_REG_PMU_SELFTEST, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bma250e_get_interrupt_enable0(const bma250e_context dev)
{
    assert(dev != NULL);

    return (bma250e_read_reg(dev, BMA250E_REG_INT_EN_0)
            & ~_BMA250E_INT_EN_0_RESERVED_BITS);
}

upm_result_t bma250e_set_interrupt_enable0(const bma250e_context dev,
                                           uint8_t bits)
{
    assert(dev != NULL);

    uint8_t reg = bits & ~_BMA250E_INT_EN_0_RESERVED_BITS;

    if (bma250e_write_reg(dev, BMA250E_REG_INT_EN_0, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bma250e_get_interrupt_enable1(const bma250e_context dev)
{
    assert(dev != NULL);

    return (bma250e_read_reg(dev, BMA250E_REG_INT_EN_1)
            & ~_BMA250E_INT_EN_1_RESERVED_BITS);
}

upm_result_t bma250e_set_interrupt_enable1(const bma250e_context dev,
                                           uint8_t bits)
{
    assert(dev != NULL);

    uint8_t reg = bits & ~_BMA250E_INT_EN_1_RESERVED_BITS;

    if (bma250e_write_reg(dev, BMA250E_REG_INT_EN_1, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bma250e_get_interrupt_enable2(const bma250e_context dev)
{
    assert(dev != NULL);

    return (bma250e_read_reg(dev, BMA250E_REG_INT_EN_2)
            & ~_BMA250E_INT_EN_2_RESERVED_BITS);
}

upm_result_t bma250e_set_interrupt_enable2(const bma250e_context dev,
                                           uint8_t bits)
{
    assert(dev != NULL);

    uint8_t reg = bits & ~_BMA250E_INT_EN_2_RESERVED_BITS;

    if (bma250e_write_reg(dev, BMA250E_REG_INT_EN_2, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bma250e_get_interrupt_map0(const bma250e_context dev)
{
    assert(dev != NULL);

    return bma250e_read_reg(dev, BMA250E_REG_INT_MAP_0);
}

upm_result_t bma250e_set_interrupt_map0(const bma250e_context dev, uint8_t bits)
{
    assert(dev != NULL);

    if (bma250e_write_reg(dev, BMA250E_REG_INT_MAP_0, bits))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bma250e_get_interrupt_map1(const bma250e_context dev)
{
    assert(dev != NULL);

    return (bma250e_read_reg(dev, BMA250E_REG_INT_MAP_1)
            & ~_BMA250E_INT_MAP_1_INT1_RESERVED_BITS);
}

upm_result_t bma250e_set_interrupt_map1(const bma250e_context dev, uint8_t bits)
{
    assert(dev != NULL);

    uint8_t reg = bits & ~_BMA250E_INT_MAP_1_INT1_RESERVED_BITS;

    if (bma250e_write_reg(dev, BMA250E_REG_INT_MAP_1, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bma250e_get_interrupt_map2(const bma250e_context dev)
{
    assert(dev != NULL);

    return bma250e_read_reg(dev, BMA250E_REG_INT_MAP_2);
}

upm_result_t bma250e_set_interrupt_map2(const bma250e_context dev, uint8_t bits)
{
    assert(dev != NULL);

    if (bma250e_write_reg(dev, BMA250E_REG_INT_MAP_2, bits))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bma250e_get_interrupt_src(const bma250e_context dev)
{
    assert(dev != NULL);

    return (bma250e_read_reg(dev, BMA250E_REG_INT_SRC)
            & ~_BMA250E_INT_SRC_RESERVED_BITS);
}

upm_result_t bma250e_set_interrupt_src(const bma250e_context dev, uint8_t bits)
{
    assert(dev != NULL);

    uint8_t reg = bits & ~_BMA250E_INT_SRC_RESERVED_BITS;

    if (bma250e_write_reg(dev, BMA250E_REG_INT_SRC, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bma250e_get_interrupt_output_control(const bma250e_context dev)
{
    assert(dev != NULL);

    return (bma250e_read_reg(dev, BMA250E_REG_INT_OUT_CTRL)
            & ~_BMA250E_INT_OUT_CTRL_INT1_RESERVED_BITS);
}

upm_result_t bma250e_set_interrupt_output_control(const bma250e_context dev,
                                                  uint8_t bits)
{
    assert(dev != NULL);

    uint8_t reg = bits & ~_BMA250E_INT_OUT_CTRL_INT1_RESERVED_BITS;

    if (bma250e_write_reg(dev, BMA250E_REG_INT_OUT_CTRL, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bma250e_clear_interrupt_latches(const bma250e_context dev)
{
    assert(dev != NULL);

    uint8_t reg =
        (bma250e_read_reg(dev, BMA250E_REG_INT_RST_LATCH)
         & ~_BMA250E_INT_RST_LATCH_RESERVED_BITS);

    reg |= BMA250E_INT_RST_LATCH_RESET_INT;

    if (bma250e_write_reg(dev, BMA250E_REG_INT_RST_LATCH, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

BMA250E_RST_LATCH_T bma250e_get_interrupt_latch_behavior(
    const bma250e_context dev)
{
    assert(dev != NULL);

    uint8_t reg = (bma250e_read_reg(dev, BMA250E_REG_INT_RST_LATCH)
                   & ~_BMA250E_INT_RST_LATCH_RESERVED_BITS);

    reg &= (_BMA250E_INT_RST_LATCH_MASK << _BMA250E_INT_RST_LATCH_SHIFT);

    return (BMA250E_RST_LATCH_T)reg;
}

upm_result_t bma250e_set_interrupt_latch_behavior(const bma250e_context dev,
                                                  BMA250E_RST_LATCH_T latch)
{
    assert(dev != NULL);

    uint8_t reg =
        (bma250e_read_reg(dev, BMA250E_REG_INT_RST_LATCH)
         & ~_BMA250E_INT_RST_LATCH_RESERVED_BITS);

    reg &= ~(_BMA250E_INT_RST_LATCH_MASK << _BMA250E_INT_RST_LATCH_SHIFT);
    reg |= (latch << _BMA250E_INT_RST_LATCH_SHIFT);

    if (bma250e_write_reg(dev, BMA250E_REG_INT_RST_LATCH, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bma250e_enable_register_shadowing(const bma250e_context dev,
                                               bool shadow)
{
    assert(dev != NULL);

    uint8_t reg =
        (bma250e_read_reg(dev, BMA250E_REG_ACC_HBW)
         & ~_BMA250E_ACC_HBW_RESERVED_BITS);

    if (shadow)
        reg &= ~BMA250E_ACC_HBW_SHADOW_DIS;
    else
        reg |= BMA250E_ACC_HBW_SHADOW_DIS;

    if (bma250e_write_reg(dev, BMA250E_REG_ACC_HBW, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bma250e_enable_output_filtering(const bma250e_context dev,
                                             bool filter)
{
    assert(dev != NULL);

    uint8_t reg =
        (bma250e_read_reg(dev, BMA250E_REG_ACC_HBW)
         & ~_BMA250E_ACC_HBW_RESERVED_BITS);

    if (filter)
        reg &= ~BMA250E_ACC_HBW_DATA_HIGH_BW;
    else
        reg |= BMA250E_ACC_HBW_DATA_HIGH_BW;

    if (bma250e_write_reg(dev, BMA250E_REG_ACC_HBW, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bma250e_get_interrupt_status0(const bma250e_context dev)
{
    assert(dev != NULL);

    return bma250e_read_reg(dev, BMA250E_REG_INT_STATUS_0);
}

uint8_t bma250e_get_interrupt_status1(const bma250e_context dev)
{
    assert(dev != NULL);

    return (bma250e_read_reg(dev, BMA250E_REG_INT_STATUS_1)
            & ~_BMA250E_INT_STATUS_1_RESERVED_BITS);
}

uint8_t bma250e_get_interrupt_status2(const bma250e_context dev)
{
    assert(dev != NULL);

    return bma250e_read_reg(dev, BMA250E_REG_INT_STATUS_2);
}

uint8_t bma250e_get_interrupt_status3_bits(const bma250e_context dev)
{
    assert(dev != NULL);

    // filter out the orientation bitfield..
    return (bma250e_read_reg(dev, BMA250E_REG_INT_STATUS_3)
            & ~(_BMA250E_INT_STATUS_3_ORIENT_MASK
                << _BMA250E_INT_STATUS_3_ORIENT_SHIFT));
}

BMA250E_ORIENT_T bma250e_get_interrupt_status3_orientation(
    const bma250e_context dev)
{
    assert(dev != NULL);

    // grab just the orientation bitfield
    uint8_t reg = (bma250e_read_reg(dev, BMA250E_REG_INT_STATUS_3)
                   & (_BMA250E_INT_STATUS_3_ORIENT_MASK
                      << _BMA250E_INT_STATUS_3_ORIENT_SHIFT));

    reg >>= _BMA250E_INT_STATUS_3_ORIENT_SHIFT;

    return (BMA250E_ORIENT_T)reg;
}

upm_result_t bma250e_set_low_power_mode2(const bma250e_context dev)
{
    assert(dev != NULL);

    uint8_t reg = (bma250e_read_reg(dev, BMA250E_REG_PMU_LOW_POWER)
                   & ~_BMA250E_LOW_POWER_RESERVED_BITS);

    // we simply set the low power mode to 2.  Low power mode 1 slows
    // down register write accesses, and we can't handle that.  In the
    // words of the late Admiral Akbar: "We cannot handle firepower of
    // that magnitude!" :(

    reg |= BMA250E_LOW_POWER_LOWPOWER_MODE;

    if (bma250e_write_reg(dev, BMA250E_REG_PMU_LOW_POWER, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bma250e_install_isr(const bma250e_context dev,
                                 BMA250E_INTERRUPT_PINS_T intr, int gpio,
                                 mraa_gpio_edge_t level,
                                 void (*isr)(void *), void *arg)
{
    assert(dev != NULL);

    // delete any existing ISR and GPIO context for this interrupt
    bma250e_uninstall_isr(dev, intr);

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
    case BMA250E_INTERRUPT_INT1:
        dev->gpio1 = gpio_isr;
        break;

    case BMA250E_INTERRUPT_INT2:
        dev->gpio2 = gpio_isr;
        break;
    }

    return UPM_SUCCESS;
}

void bma250e_uninstall_isr(const bma250e_context dev,
                           BMA250E_INTERRUPT_PINS_T intr)
{
    assert(dev != NULL);

    switch (intr)
    {
    case BMA250E_INTERRUPT_INT1:
        if (dev->gpio1)
        {
            mraa_gpio_isr_exit(dev->gpio1);
            mraa_gpio_close(dev->gpio1);
            dev->gpio1 = NULL;
        }
        break;

    case BMA250E_INTERRUPT_INT2:
        if (dev->gpio2)
        {
            mraa_gpio_isr_exit(dev->gpio2);
            mraa_gpio_close(dev->gpio2);
            dev->gpio2 = NULL;
        }
        break;
    }
}
