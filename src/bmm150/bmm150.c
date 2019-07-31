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

#include "bmm150.h"

// SPI CS on and off functions
static void _csOn(const bmm150_context dev)
{
    assert(dev != NULL);

    if (dev->gpioCS)
        mraa_gpio_write(dev->gpioCS, 0);
}

static void _csOff(const bmm150_context dev)
{
    assert(dev != NULL);

    if (dev->gpioCS)
        mraa_gpio_write(dev->gpioCS, 1);
}

// These trimming algorithms (bmm050_bosch_compensate_*()) are taken
// from the Bosch BMM050 reference driver code.  See license.txt.

// Bosch compensation functions
static float _bmm050_compensate_X_float(bmm150_context dev,
                                        int16_t mag_data_x)
{
    assert(dev != NULL);

    uint16_t data_r = dev->hall;
    float inter_retval = 0;

    if (mag_data_x != -4096	/* no overflow */
        ) {
        if ((data_r != 0)
            && (dev->dig_xyz1 != 0)) {
            inter_retval = ((((float)dev->dig_xyz1)
                             * 16384.0 / data_r) - 16384.0);
        } else {
            inter_retval = 0.0f;
            return inter_retval;
        }
        inter_retval = (((mag_data_x * ((((((float)dev->dig_xy2) *
                                           (inter_retval*inter_retval /
                                            268435456.0) +
                                           inter_retval * ((float)dev->dig_xy1)
                                           / 16384.0)) + 256.0) *
                                        (((float)dev->dig_x2) + 160.0)))
                         / 8192.0)
                        + (((float)dev->dig_x1) *
                           8.0)) / 16.0;
    } else {
        inter_retval = 0.0f;
    }
    return inter_retval;
}

static float _bmm050_compensate_Y_float(bmm150_context dev,
                                        int16_t mag_data_y)
{
    assert(dev != NULL);

    uint16_t data_r = dev->hall;
    float inter_retval = 0;

    if (mag_data_y != -4096 /* no overflow */
        ) {
        if ((data_r != 0)
            && (dev->dig_xyz1 != 0)) {
            inter_retval = ((((float)dev->dig_xyz1)
                             * 16384.0
                             /data_r) - 16384.0);
        } else {
            inter_retval = 0.0f;
            return inter_retval;
        }
        inter_retval = (((mag_data_y * ((((((float)dev->dig_xy2) *
                                           (inter_retval*inter_retval
                                            / 268435456.0) +
                                           inter_retval * ((float)dev->dig_xy1)
                                           / 16384.0)) +
                                         256.0) *
                                        (((float)dev->dig_y2) + 160.0)))
                         / 8192.0) +
                        (((float)dev->dig_y1) * 8.0))
            / 16.0;
    } else {
        /* overflow, set output to 0.0f */
        inter_retval = 0.0f;
    }
    return inter_retval;
}

static float _bmm050_compensate_Z_float(bmm150_context dev,
                                        int16_t mag_data_z)
{
    assert(dev != NULL);

    uint16_t data_r = dev->hall;
    float inter_retval = 0;
    /* no overflow */
    if (mag_data_z != -16384) {
        if ((dev->dig_z2 != 0)
            && (dev->dig_z1 != 0)
            && (dev->dig_xyz1 != 0)
            && (data_r != 0)) {
            inter_retval = ((((((float)mag_data_z)-
                               ((float)dev->dig_z4)) * 131072.0)-
                             (((float)dev->dig_z3)*(((float)data_r)
                                                    -((float)dev->dig_xyz1))))
                            /((((float)dev->dig_z2)+
                               ((float)dev->dig_z1)*((float)data_r) /
                               32768.0) * 4.0)) / 16.0;
        }
    } else {
        /* overflow, set output to 0.0f */
        inter_retval = 0.0f;
    }
    return inter_retval;
}

// read trim data
static upm_result_t _bmm150_read_trim_data(const bmm150_context dev)
{
    assert(dev != NULL);

    int bufLen = 10;
    uint8_t calibData[bufLen];

    // 2 bytes first
    if (bmm150_read_regs(dev, BMM150_REG_TRIM_DIG_X1, calibData, 2) != 2)
        return UPM_ERROR_OPERATION_FAILED;

    dev->dig_x1 = (int8_t)calibData[0];
    dev->dig_y1 = (int8_t)calibData[1];

    // next block of 4 bytes
    if (bmm150_read_regs(dev, BMM150_REG_TRIM_DIG_Z4_LSB, calibData, 4) != 4)
        return UPM_ERROR_OPERATION_FAILED;

    dev->dig_z4 = (int16_t)((calibData[1] << 8) | calibData[0]);
    dev->dig_x2 = (int8_t)calibData[2];
    dev->dig_y2 = (int8_t)calibData[3];

    // final block of 10 bytes
    if (bmm150_read_regs(dev, BMM150_REG_TRIM_DIG_Z2_LSB, calibData, 10) != 10)
        return UPM_ERROR_OPERATION_FAILED;

    dev->dig_z2 = (int16_t)((calibData[1] << 8) | calibData[0]);
    dev->dig_z1 = (uint16_t)((calibData[3] << 8) | calibData[2]);
    dev->dig_xyz1 = (uint16_t)((calibData[5] << 8) | calibData[4]);
    dev->dig_z3 = (int16_t)((calibData[7] << 8) | calibData[6]);
    dev->dig_xy2 = (int8_t)calibData[8];
    dev->dig_xy1 = calibData[9];

    return UPM_SUCCESS;
}


// init
bmm150_context bmm150_init(int bus, int addr, int cs)
{
    bmm150_context dev =
        (bmm150_context)malloc(sizeof(struct _bmm150_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _bmm150_context));

    // make sure MRAA is initialized
    if (mraa_init() != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed.\n", __FUNCTION__);
        bmm150_close(dev);
        return NULL;
    }

    if (addr < 0)
        dev->isSPI = true;

    if (dev->isSPI)
    {
        if (!(dev->spi = mraa_spi_init(bus)))
        {
            printf("%s: mraa_spi_init() failed.\n", __FUNCTION__);
            bmm150_close(dev);
            return NULL;
        }

        // Only create cs context if we are actually using a valid pin.
        // A hardware controlled pin should specify cs as -1.
        if (cs >= 0)
        {
            if (!(dev->gpioCS = mraa_gpio_init(cs)))
            {
                printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
                bmm150_close(dev);
                return NULL;
            }
            mraa_gpio_dir(dev->gpioCS, MRAA_GPIO_OUT);
        }

        mraa_spi_mode(dev->spi, MRAA_SPI_MODE0);
        if (mraa_spi_frequency(dev->spi, 5000000))
        {
            printf("%s: mraa_spi_frequency() failed.\n", __FUNCTION__);
            bmm150_close(dev);
            return NULL;
        }
    }
    else
    {
        // I2C

        if (!(dev->i2c = mraa_i2c_init(bus)))
        {
            printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
            bmm150_close(dev);
            return NULL;
        }

        if (mraa_i2c_address(dev->i2c, addr))
        {
            printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
            bmm150_close(dev);
            return NULL;
        }
    }

    // power bit must be on for chip ID to be accessible
    bmm150_set_power_bit(dev, true);

    // not really, just need to set it to something valid until
    // bmm150_set_opmode() is called in bmm150_devinit().
    dev->opmode = BMM150_OPERATION_MODE_SLEEP;

    upm_delay_ms(50);

    // check the chip id

    uint8_t chipID = bmm150_get_chip_id(dev);

    if (chipID != BMM150_DEFAULT_CHIPID)
    {
        printf("%s: invalid chip id: %02x.  Expected %02x\n",
               __FUNCTION__, chipID, BMM150_DEFAULT_CHIPID);
        bmm150_close(dev);
        return NULL;
    }

    // call devinit with a default high resolution mode
    if (bmm150_devinit(dev, BMM150_USAGE_HIGH_ACCURACY))
    {
        printf("%s: bmm150_devinit() failed.\n", __FUNCTION__);
        bmm150_close(dev);
        return NULL;
    }

    return dev;
}

void bmm150_close(bmm150_context dev)
{
    assert(dev != NULL);

    bmm150_uninstall_isr(dev, BMM150_INTERRUPT_INT);
    bmm150_uninstall_isr(dev, BMM150_INTERRUPT_DR);

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);
    if (dev->spi)
        mraa_spi_stop(dev->spi);
    if (dev->gpioCS)
        mraa_gpio_close(dev->gpioCS);

    free(dev);
}

upm_result_t bmm150_devinit(const bmm150_context dev,
                            BMM150_USAGE_PRESETS_T usage)
{
    assert(dev != NULL);

    // just in case...
    if (bmm150_set_power_bit(dev, true))
    {
        printf("%s: bmm150_set_power_bit() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // get trim data
    if (_bmm150_read_trim_data(dev))
    {
        printf("%s: _bmm150_read_trim_data() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    if (bmm150_set_opmode(dev, BMM150_OPERATION_MODE_NORMAL))
    {
        printf("%s: bmm150_set_opmode() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    upm_delay_ms(50); // 50ms, in case we are waking up

    if (bmm150_set_preset_mode(dev, usage))
    {
        printf("%s: bmm150_set_preset_mode() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // settle
    upm_delay_ms(50);

    return UPM_SUCCESS;
}

upm_result_t bmm150_update(const bmm150_context dev)
{
    assert(dev != NULL);

    // special care when in a forced mode - need to trigger a
    // measurement, and wait for the opmode to return to OPMODE_SLEEP,
    // then we can read the values.

    if (dev->opmode == BMM150_OPERATION_MODE_FORCED)
    {
        // trigger measurement
        if (bmm150_set_opmode(dev, BMM150_OPERATION_MODE_FORCED))
        {
            printf("%s: bmm150_set_opmode() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        // opmode will return to BMM150_OPERATION_MODE_SLEEP after
        // measurement is complete
        do {
            upm_delay_ms(5);
        } while (bmm150_get_opmode(dev) == BMM150_OPERATION_MODE_FORCED);
    }

    const int bufLen = 8;
    uint8_t buf[bufLen];

    if (bmm150_read_regs(dev, BMM150_REG_MAG_X_LSB, buf, bufLen) != bufLen)
    {
        printf("%s: bmm150_read_regs() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // we need to get the hall data first, since it's needed for the
    // bosch compensation functions for each of the xyz axes

    dev->hall = (uint16_t)(buf[7] << 8 | (buf[6] &
                                          (_BMM150_MAG_RHALL_LSB_LSB_MASK <<
                                           _BMM150_MAG_RHALL_LSB_LSB_SHIFT)));
    dev->hall /= 4;

    int16_t val;

    // x
    val = (int16_t)(buf[1] << 8 | (buf[0] & (_BMM150_MAG_XY_LSB_LSB_MASK <<
                                             _BMM150_MAG_XY_LSB_LSB_SHIFT)));
    val /= 8;
    dev->magX = _bmm050_compensate_X_float(dev, val);

    // y
    val = (int16_t)(buf[3] << 8 | (buf[2] & (_BMM150_MAG_XY_LSB_LSB_MASK <<
                                             _BMM150_MAG_XY_LSB_LSB_SHIFT)));
    val /= 8;
    dev->magY = _bmm050_compensate_Y_float(dev, val);

    // z
    val = (int16_t)(buf[5] << 8 | (buf[4] & (_BMM150_MAG_Z_LSB_LSB_MASK <<
                                             _BMM150_MAG_Z_LSB_LSB_SHIFT)));
    val /= 2;
    dev->magZ = _bmm050_compensate_Z_float(dev, val);

    return UPM_SUCCESS;
}

uint8_t bmm150_read_reg(const bmm150_context dev, uint8_t reg)
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

int bmm150_read_regs(const bmm150_context dev, uint8_t reg,
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

upm_result_t bmm150_write_reg(const bmm150_context dev,
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

uint8_t bmm150_get_chip_id(const bmm150_context dev)
{
    assert(dev != NULL);

    return bmm150_read_reg(dev, BMM150_REG_CHIP_ID);
}

void bmm150_get_magnetometer(const bmm150_context dev,
                             float *x, float *y, float *z)
{
    assert(dev != NULL);

    if (x)
        *x = dev->magX;

    if (y)
        *y = dev->magY;

    if (z)
        *z = dev->magZ;
}

upm_result_t bmm150_reset(const bmm150_context dev)
{
    assert(dev != NULL);

    // mask off reserved bits
    uint8_t reg =
        (bmm150_read_reg(dev, BMM150_REG_POWER_CTRL)
         & ~_BMM150_POWER_CTRL_RESERVED_BITS);

    reg |= BMM150_POWER_CTRL_SOFT_RESET0 | BMM150_POWER_CTRL_SOFT_RESET1;

    if (bmm150_write_reg(dev, BMM150_REG_POWER_CTRL, reg))
        return UPM_ERROR_OPERATION_FAILED;

    upm_delay(1);
    // device will return to SLEEP mode...

    return UPM_SUCCESS;
}

upm_result_t bmm150_set_output_data_rate(const bmm150_context dev,
                                         BMM150_DATA_RATE_T odr)
{
    assert(dev != NULL);

    uint8_t reg = bmm150_read_reg(dev, BMM150_REG_OPMODE);

    reg &= ~(_BMM150_OPMODE_DATA_RATE_MASK << _BMM150_OPMODE_DATA_RATE_SHIFT);
    reg |= (odr << _BMM150_OPMODE_DATA_RATE_SHIFT);

    if (bmm150_write_reg(dev, BMM150_REG_OPMODE, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bmm150_set_power_bit(const bmm150_context dev, bool power)
{
    assert(dev != NULL);

    // mask off reserved bits
    uint8_t reg =
        (bmm150_read_reg(dev, BMM150_REG_POWER_CTRL)
         & ~_BMM150_POWER_CTRL_RESERVED_BITS);

    if (power)
        reg |= BMM150_POWER_CTRL_POWER_CTRL_BIT;
    else
        reg &= ~BMM150_POWER_CTRL_POWER_CTRL_BIT;

    if (bmm150_write_reg(dev, BMM150_REG_POWER_CTRL, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bmm150_set_opmode(const bmm150_context dev,
                               BMM150_OPERATION_MODE_T opmode)
{
    assert(dev != NULL);

    uint8_t reg = bmm150_read_reg(dev, BMM150_REG_OPMODE);

    reg &= ~(_BMM150_OPMODE_OPERATION_MODE_MASK
             << _BMM150_OPMODE_OPERATION_MODE_SHIFT);
    reg |= (opmode << _BMM150_OPMODE_OPERATION_MODE_SHIFT);

    if (bmm150_write_reg(dev, BMM150_REG_OPMODE, reg))
        return UPM_ERROR_OPERATION_FAILED;

    dev->opmode = opmode;
    return UPM_SUCCESS;
}

BMM150_OPERATION_MODE_T bmm150_get_opmode(const bmm150_context dev)
{
    assert(dev != NULL);

    uint8_t reg = bmm150_read_reg(dev, BMM150_REG_OPMODE);

    reg &= (_BMM150_OPMODE_OPERATION_MODE_MASK
            << _BMM150_OPMODE_OPERATION_MODE_SHIFT);
    reg >>=  _BMM150_OPMODE_OPERATION_MODE_SHIFT;

    return (BMM150_OPERATION_MODE_T)reg;
}

uint8_t bmm150_get_interrupt_enable(const bmm150_context dev)
{
    assert(dev != NULL);

    return bmm150_read_reg(dev, BMM150_REG_INT_EN);
}

upm_result_t bmm150_set_interrupt_enable(const bmm150_context dev,
                                         uint8_t bits)
{
    assert(dev != NULL);

    if (bmm150_write_reg(dev, BMM150_REG_INT_EN, bits))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bmm150_get_interrupt_config(const bmm150_context dev)
{
    assert(dev != NULL);

    return bmm150_read_reg(dev, BMM150_REG_INT_CONFIG);
}

upm_result_t bmm150_set_interrupt_config(const bmm150_context dev,
                                         uint8_t bits)
{
    assert(dev != NULL);

    if (bmm150_write_reg(dev, BMM150_REG_INT_CONFIG, bits))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t bmm150_get_interrupt_status(const bmm150_context dev)
{
    assert(dev != NULL);

    return bmm150_read_reg(dev, BMM150_REG_INT_STATUS);
}

upm_result_t bmm150_set_repetitions_xy(const bmm150_context dev,
                                       uint8_t reps)
{
    assert(dev != NULL);

    if (bmm150_write_reg(dev, BMM150_REG_REP_XY, reps))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bmm150_set_repetitions_z(const bmm150_context dev,
                                      uint8_t reps)
{
    assert(dev != NULL);

    if (bmm150_write_reg(dev, BMM150_REG_REP_Z, reps))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bmm150_set_preset_mode(const bmm150_context dev,
                                    BMM150_USAGE_PRESETS_T usage)
{
    assert(dev != NULL);

    bool error = false;

    // these recommended presets come from the datasheet, Table 3,
    // Section 4.2
    switch (usage)
    {
    case BMM150_USAGE_LOW_POWER:
        if (bmm150_set_repetitions_xy(dev, 3)
            || bmm150_set_repetitions_z(dev, 3)
            || bmm150_set_output_data_rate(dev, BMM150_DATA_RATE_10HZ))
            error = true;

        break;

    case BMM150_USAGE_REGULAR:
        if (bmm150_set_repetitions_xy(dev, 9)
            || bmm150_set_repetitions_z(dev, 15)
            || bmm150_set_output_data_rate(dev, BMM150_DATA_RATE_10HZ))
            error = true;

        break;

    case BMM150_USAGE_ENHANCED_REGULAR:
        if (bmm150_set_repetitions_xy(dev, 15)
            || bmm150_set_repetitions_z(dev, 27)
            || bmm150_set_output_data_rate(dev, BMM150_DATA_RATE_10HZ))
            error = true;

        break;

    case BMM150_USAGE_HIGH_ACCURACY:
        if (bmm150_set_repetitions_xy(dev, 47)
            || bmm150_set_repetitions_z(dev, 83)
            || bmm150_set_output_data_rate(dev, BMM150_DATA_RATE_20HZ))
            error = true;

        break;

    default:
        printf("%s: Invalid usage value passed.\n", __FUNCTION__);
        error = true;
    }

    if (error)
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bmm150_install_isr(const bmm150_context dev,
                                BMM150_INTERRUPT_PINS_T intr, int gpio,
                                mraa_gpio_edge_t level,
                                void (*isr)(void *), void *arg)
{
    assert(dev != NULL);

    // delete any existing ISR and GPIO context for this interrupt
    bmm150_uninstall_isr(dev, intr);

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
    case BMM150_INTERRUPT_INT:
        dev->gpioINT = gpio_isr;
        break;

    case BMM150_INTERRUPT_DR:
        dev->gpioDR = gpio_isr;
        break;
    }

    return UPM_SUCCESS;
}

void bmm150_uninstall_isr(const bmm150_context dev,
                          BMM150_INTERRUPT_PINS_T intr)
{
    assert(dev != NULL);

    switch (intr)
    {
    case BMM150_INTERRUPT_INT:
        if (dev->gpioINT)
        {
            mraa_gpio_isr_exit(dev->gpioINT);
            mraa_gpio_close(dev->gpioINT);
            dev->gpioINT = NULL;
        }
        break;

    case BMM150_INTERRUPT_DR:
        if (dev->gpioDR)
        {
            mraa_gpio_isr_exit(dev->gpioDR);
            mraa_gpio_close(dev->gpioDR);
            dev->gpioDR = NULL;
        }
        break;
    }
}
