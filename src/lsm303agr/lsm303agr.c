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

#include "lsm303agr.h"

// some useful macros to save on typing and text wrapping
#undef _SHIFT
#define _SHIFT(x) (_LSM303AGR_##x##_SHIFT)

#undef _MASK
#define _MASK(x) (_LSM303AGR_##x##_MASK)

#undef _SHIFTMASK
#define _SHIFTMASK(x) (_MASK(x) << _SHIFT(x))


// init
lsm303agr_context lsm303agr_init(int bus, int acc_addr, int mag_addr)
{
    if (acc_addr <= 0 && mag_addr <= 0)
    {
        printf("%s: At least one device must be enabled\n", __FUNCTION__);
        return NULL;
    }

    lsm303agr_context dev =
        (lsm303agr_context)malloc(sizeof(struct _lsm303agr_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _lsm303agr_context));

    // make sure MRAA is initialized
    if (mraa_init() != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed.\n", __FUNCTION__);
        lsm303agr_close(dev);
        return NULL;
    }

    if (acc_addr > 0)
    {
        if (!(dev->i2cACC = mraa_i2c_init(bus)))
        {
            printf("%s: mraa_i2c_init(acc) failed.\n", __FUNCTION__);
            lsm303agr_close(dev);
            return NULL;
        }

        if (mraa_i2c_address(dev->i2cACC, acc_addr))
        {
            printf("%s: mraa_i2c_address(acc) failed.\n", __FUNCTION__);
            lsm303agr_close(dev);
            return NULL;
        }

        // check the chip id
        uint8_t chipID = lsm303agr_read_reg(dev, LSM303AGR_REG_WHO_AM_I_A);

        if (chipID != LSM303AGR_CHIPID_ACC)
        {
            printf("%s: invalid accelerometer chip id: %02x.  Expected %02x\n",
                   __FUNCTION__, chipID, LSM303AGR_CHIPID_ACC);
            lsm303agr_close(dev);
            return NULL;
        }
    }

    // technically we could use a single i2c context since it is bus
    // specific, but then we would need to call i2c_address() every
    // time we wanted to talk to a specific device.  In addition, we
    // can use the i2c context pointer to determine if a subsystem
    // (acc or mag) is actually enabled throughout this driver.
    if (mag_addr > 0)
    {
        if (!(dev->i2cMAG = mraa_i2c_init(bus)))
        {
            printf("%s: mraa_i2c_init(mag) failed.\n", __FUNCTION__);
            lsm303agr_close(dev);
            return NULL;
        }

        if (mraa_i2c_address(dev->i2cMAG, mag_addr))
        {
            printf("%s: mraa_i2c_address(mag) failed.\n", __FUNCTION__);
            lsm303agr_close(dev);
            return NULL;
        }

        // check the chip id
        uint8_t chipID = lsm303agr_read_reg(dev, LSM303AGR_REG_WHO_AM_I_M);

        if (chipID != LSM303AGR_CHIPID_MAG)
        {
            printf("%s: invalid magnetometer chip id: %02x.  Expected %02x\n",
                   __FUNCTION__, chipID, LSM303AGR_CHIPID_MAG);
            lsm303agr_close(dev);
            return NULL;
        }
    }

    // call devinit with a default high resolution mode
    if (lsm303agr_devinit(dev, LSM303AGR_POWER_HIGH_RESOLUTION))
    {
        printf("%s: lsm303agr_devinit() failed.\n", __FUNCTION__);
        lsm303agr_close(dev);
        return NULL;
    }

    return dev;
}

void lsm303agr_close(lsm303agr_context dev)
{
    assert(dev != NULL);

    lsm303agr_uninstall_isr(dev, LSM303AGR_INTERRUPT_ACC_1);
    lsm303agr_uninstall_isr(dev, LSM303AGR_INTERRUPT_ACC_2);
    lsm303agr_uninstall_isr(dev, LSM303AGR_INTERRUPT_MAG);

    if (dev->i2cACC)
        mraa_i2c_stop(dev->i2cACC);
    if (dev->i2cMAG)
        mraa_i2c_stop(dev->i2cMAG);

    free(dev);
}

upm_result_t lsm303agr_devinit(const lsm303agr_context dev,
                               LSM303AGR_POWER_MODE_T mode)
{
    assert(dev != NULL);

    // magnetometer
    if (dev->i2cMAG)
    {
        // enable temp compensation and continuous mode
        uint8_t reg = lsm303agr_read_reg(dev, LSM303AGR_REG_CFG_REG_A_M);

        reg &= ~_SHIFTMASK(CFG_REG_A_M_MD);
        reg |= LSM303AGR_CFG_REG_A_M_COMP_TEMP_EN;
        reg |= (LSM303AGR_CFG_A_M_MD_CONTINUOUS
                << _SHIFT(CFG_REG_A_M_MD));

        if (lsm303agr_write_reg(dev, LSM303AGR_REG_CFG_REG_A_M, reg))
        {
            printf("%s: lsm303agr_write_reg() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        // set MAG ODR to 10Hz by default
        if (lsm303agr_set_mag_odr(dev, LSM303AGR_CFG_A_M_ODR_10HZ))
        {
            printf("%s: lsm303agr_set_mag_odr() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        upm_delay_ms(10);
    }

    // accelerometer
    if (dev->i2cACC)
    {
        // enable all axes
        uint8_t reg = lsm303agr_read_reg(dev, LSM303AGR_REG_CTRL_REG1_A);

        reg |= LSM303AGR_CTRL_REG1_A_XEN
            | LSM303AGR_CTRL_REG1_A_YEN
            | LSM303AGR_CTRL_REG1_A_ZEN;

        if (lsm303agr_write_reg(dev, LSM303AGR_REG_CTRL_REG1_A, reg))
        {
            printf("%s: lsm303agr_write_reg() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        // enable BDU
        reg = lsm303agr_read_reg(dev, LSM303AGR_REG_CTRL_REG4_A);
        reg |= LSM303AGR_CTRL_REG4_A_BDU;

        if (lsm303agr_write_reg(dev, LSM303AGR_REG_CTRL_REG4_A, reg))
        {
            printf("%s: lsm303agr_write_reg() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        // enable temperature measurement
        reg = lsm303agr_read_reg(dev, LSM303AGR_REG_TEMP_CFG_REG_A);
        reg &= ~_SHIFTMASK(TEMP_CFG_REG_A_TEMP_EN);
        reg |= (LSM303AGR_TEMP_EN_ON
                << _SHIFT(TEMP_CFG_REG_A_TEMP_EN));

        if (lsm303agr_write_reg(dev, LSM303AGR_REG_TEMP_CFG_REG_A, reg))
        {
            printf("%s: lsm303agr_write_reg() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        // set ACC ODR to 100Hz by default
        if (lsm303agr_set_acc_odr(dev, LSM303AGR_A_ODR_100HZ))
        {
            printf("%s: lsm303agr_set_acc_odr() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        // default to 2G sensitivity
        if (lsm303agr_set_full_scale(dev, LSM303AGR_A_FS_2G))
        {
            printf("%s: lsm303agr_set_full_scale() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        upm_delay_ms(10);
    }

    if (lsm303agr_set_power_mode(dev, mode))
    {
        printf("%s: lsm303agr_set_power_mode() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t lsm303agr_set_power_mode(const lsm303agr_context dev,
                                      LSM303AGR_POWER_MODE_T mode)
{
    assert(dev != NULL);

    // magnetometer
    if (dev->i2cMAG)
    {
        uint8_t reg = lsm303agr_read_reg(dev, LSM303AGR_REG_CFG_REG_A_M);

        // only low power or hires supported here
        if (mode == LSM303AGR_POWER_LOW_POWER)
            reg |= LSM303AGR_CFG_REG_A_M_LP;
        else
            reg &= ~LSM303AGR_CFG_REG_A_M_LP;

        if (lsm303agr_write_reg(dev, LSM303AGR_REG_CFG_REG_A_M, reg))
            return UPM_ERROR_OPERATION_FAILED;
    }

    // accelerometer
    if (dev->i2cACC)
    {
        uint8_t reg1 = lsm303agr_read_reg(dev, LSM303AGR_REG_CTRL_REG1_A);
        uint8_t reg4 = lsm303agr_read_reg(dev, LSM303AGR_REG_CTRL_REG4_A);

        switch (mode)
        {
        case LSM303AGR_POWER_LOW_POWER:
            reg1 |= LSM303AGR_CTRL_REG1_A_LPEN;
            reg4 &= ~LSM303AGR_CTRL_REG4_A_HR;
            break;

        case LSM303AGR_POWER_NORMAL:
            reg1 &= ~LSM303AGR_CTRL_REG1_A_LPEN;
            reg4 &= ~LSM303AGR_CTRL_REG4_A_HR;
            break;

        case LSM303AGR_POWER_HIGH_RESOLUTION:
            reg1 &= ~LSM303AGR_CTRL_REG1_A_LPEN;
            reg4 |= LSM303AGR_CTRL_REG4_A_HR;
            break;
        }

        if (lsm303agr_write_reg(dev, LSM303AGR_REG_CTRL_REG1_A, reg1))
            return UPM_ERROR_OPERATION_FAILED;

        if (lsm303agr_write_reg(dev, LSM303AGR_REG_CTRL_REG4_A, reg4))
            return UPM_ERROR_OPERATION_FAILED;
    }

    // settle
    upm_delay_ms(10);

    dev->powerMode = mode;

    return UPM_SUCCESS;
}

upm_result_t lsm303agr_set_full_scale(const lsm303agr_context dev,
                                      LSM303AGR_A_FS_T fs)
{
    assert(dev != NULL);

    // this only affects the accelerometer
    if (dev->i2cACC)
    {
        uint8_t reg = lsm303agr_read_reg(dev, LSM303AGR_REG_CTRL_REG4_A);

        reg &= ~_SHIFTMASK(CTRL_REG4_A_FS);
        reg |= (fs << _SHIFT(CTRL_REG4_A_FS));

        if (lsm303agr_write_reg(dev, LSM303AGR_REG_CTRL_REG4_A, reg))
            return UPM_ERROR_OPERATION_FAILED;

        upm_delay_ms(50);

        // set our scaling factor depending on current power mode and
        // FS
        switch(dev->powerMode)
        {
        case LSM303AGR_POWER_LOW_POWER:
            // 8b resolution
            dev->accDivisor = 256.0;

            switch (fs)
            {
            case LSM303AGR_A_FS_2G:
                dev->accScale = 15.63;
                break;

            case LSM303AGR_A_FS_4G:
                dev->accScale = 31.26;
                break;

            case LSM303AGR_A_FS_8G:
                dev->accScale = 62.52;
                break;

            case LSM303AGR_A_FS_16G:
                dev->accScale = 187.58;
                break;
            }
            break;

        case LSM303AGR_POWER_NORMAL:
            // 10b resolution
            dev->accDivisor = 64.0;

            switch (fs)
            {
            case LSM303AGR_A_FS_2G:
                dev->accScale = 3.9;
                break;

            case LSM303AGR_A_FS_4G:
                dev->accScale = 7.82;
                break;

            case LSM303AGR_A_FS_8G:
                dev->accScale = 15.63;
                break;

            case LSM303AGR_A_FS_16G:
                dev->accScale = 46.9;
                break;
            }
            break;

        case LSM303AGR_POWER_HIGH_RESOLUTION:
            // 12b resolution
            dev->accDivisor = 16.0;

            switch (fs)
            {
            case LSM303AGR_A_FS_2G:
                dev->accScale = 0.98;
                break;

            case LSM303AGR_A_FS_4G:
                dev->accScale = 1.95;
                break;

            case LSM303AGR_A_FS_8G:
                dev->accScale = 3.9;
                break;

            case LSM303AGR_A_FS_16G:
                dev->accScale = 11.72;
                break;
            }
            break;
        }
    }

    return UPM_SUCCESS;
}

upm_result_t lsm303agr_update(const lsm303agr_context dev)
{
    assert(dev != NULL);

    const int maxLen = 6;
    uint8_t buf[maxLen];

    if (dev->i2cACC)
    {
        // get the temperature first, only 2 bytes
        if (lsm303agr_read_regs(dev, LSM303AGR_REG_OUT_TEMP_L_A, buf, 2) != 2)
        {
            printf("%s: lsm303agr_read_regs(temp) failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        dev->temperature = (float)((int16_t)(buf[0] | (buf[1] << 8)));

        // next, acc data
        if (lsm303agr_read_regs(dev, LSM303AGR_REG_OUT_X_L_A, buf,
                                maxLen) != maxLen)
        {
            printf("%s: lsm303agr_read_regs(acc) failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        dev->accX = (float)((int16_t)(buf[0] | (buf[1] << 8)));
        dev->accY = (float)((int16_t)(buf[2] | (buf[3] << 8)));
        dev->accZ = (float)((int16_t)(buf[4] | (buf[5] << 8)));
    }

    if (dev->i2cMAG)
    {
        // now mag data
        if (lsm303agr_read_regs(dev,LSM303AGR_REG_OUTX_L_REG_M, buf,
                                maxLen) != maxLen)
        {
            printf("%s: lsm303agr_read_regs(mag) failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        dev->magX = (float)((int16_t)(buf[0] | (buf[1] << 8)));
        dev->magY = (float)((int16_t)(buf[2] | (buf[3] << 8)));
        dev->magZ = (float)((int16_t)(buf[4] | (buf[5] << 8)));
    }

    return UPM_SUCCESS;
}

uint8_t lsm303agr_read_reg(const lsm303agr_context dev, uint8_t reg)
{
    assert(dev != NULL);

    mraa_i2c_context i2c = NULL;
    if (reg <= LSM303AGR_MAX_ACC_ADDR)
        i2c = dev->i2cACC;
    else
        i2c = dev->i2cMAG;

    if (i2c)
    {
        int rv = mraa_i2c_read_byte_data(i2c, reg);
        if (rv < 0)
        {
            printf("%s: mraa_i2c_read_byte_data() failed\n", __FUNCTION__);
            return 0xff;
        }
        return (uint8_t)rv;
    }
    else // shouldn't happen, but...
        return 0xff;
}

int lsm303agr_read_regs(const lsm303agr_context dev, uint8_t reg,
                        uint8_t *buffer, int len)
{
    assert(dev != NULL);

    mraa_i2c_context i2c = NULL;
    if (reg <= LSM303AGR_MAX_ACC_ADDR)
        i2c = dev->i2cACC;
    else
        i2c = dev->i2cMAG;

    if (i2c)
    {
        reg |= 0x80; // enable auto-increment
        if (mraa_i2c_read_bytes_data(i2c, reg, buffer, len) != len)
            return -1;
    }
    else
        return -1;

    return len;
}

upm_result_t lsm303agr_write_reg(const lsm303agr_context dev,
                                 uint8_t reg, uint8_t val)
{
    assert(dev != NULL);

    mraa_i2c_context i2c = NULL;
    if (reg <= LSM303AGR_MAX_ACC_ADDR)
        i2c = dev->i2cACC;
    else
        i2c = dev->i2cMAG;

    if (i2c)
    {
        if (mraa_i2c_write_byte_data(i2c, val, reg))
        {
            printf("%s: mraa_i2c_write_byte_data() failed.\n",
                   __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }
    }
    else
        return UPM_ERROR_NO_RESOURCES;

    return UPM_SUCCESS;
}

float lsm303agr_get_temperature(const lsm303agr_context dev)
{
    assert(dev != NULL);

    // DS says 8 bits.  It is not clear in the DS how to compute this,
    // but the following seems to produce a reasonably correct
    // temperature.
    return (dev->temperature / 256.0) + 25.0;
}

void lsm303agr_get_magnetometer(const lsm303agr_context dev,
                                float *x, float *y, float *z)
{
    assert(dev != NULL);

    // 1.5 comes from the datasheet.  Output is in milli-Gauss - we
    // convert and return it in uT (SI micro-teslas) instead.
    if (x)
        *x = (dev->magX * 1.5) / 10.0;
    if (y)
        *y = (dev->magY * 1.5) / 10.0;
    if (z)
        *z = (dev->magZ * 1.5) / 10.0;
}

void lsm303agr_get_accelerometer(const lsm303agr_context dev,
                                 float *x, float *y, float *z)
{
    assert(dev != NULL);

    if (x)
        *x = ((dev->accX / dev->accDivisor) * dev->accScale) / 1000.0;
    if (y)
        *y = ((dev->accY / dev->accDivisor) * dev->accScale) / 1000.0;
    if (z)
        *z = ((dev->accZ / dev->accDivisor) * dev->accScale) / 1000.0;
}

upm_result_t lsm303agr_set_acc_odr(const lsm303agr_context dev,
                                   LSM303AGR_A_ODR_T odr)
{
    assert(dev != NULL);

    if (!dev->i2cACC)
        return UPM_ERROR_NO_RESOURCES;

    uint8_t reg = lsm303agr_read_reg(dev, LSM303AGR_REG_CTRL_REG1_A);
    reg &= ~_SHIFTMASK(CTRL_REG1_A_ODR);
    reg |= (odr << _SHIFT(CTRL_REG1_A_ODR));

    if (lsm303agr_write_reg(dev, LSM303AGR_REG_CTRL_REG1_A, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t lsm303agr_set_mag_odr(const lsm303agr_context dev,
                                   LSM303AGR_CFG_A_M_ODR_T odr)
{
    assert(dev != NULL);

    if (!dev->i2cMAG)
        return UPM_ERROR_NO_RESOURCES;

    uint8_t reg = lsm303agr_read_reg(dev, LSM303AGR_REG_CFG_REG_A_M);
    reg &= ~_SHIFTMASK(CFG_REG_A_M_ODR);
    reg |= (odr << _SHIFT(CFG_REG_A_M_ODR));

    if (lsm303agr_write_reg(dev, LSM303AGR_REG_CFG_REG_A_M, reg))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t lsm303agr_get_acc_int1_config(const lsm303agr_context dev)
{
    assert(dev != NULL);

    if (!dev->i2cACC)
        return 0;

    return lsm303agr_read_reg(dev, LSM303AGR_REG_INT1_CFG_A);
}

upm_result_t lsm303agr_set_acc_int1_config(const lsm303agr_context dev,
                                                 uint8_t bits)
{
    assert(dev != NULL);

    if (!dev->i2cACC)
        return UPM_ERROR_NO_RESOURCES;

    if (lsm303agr_write_reg(dev, LSM303AGR_REG_INT1_CFG_A, bits))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t lsm303agr_get_acc_int2_config(const lsm303agr_context dev)
{
    assert(dev != NULL);

    if (!dev->i2cACC)
        return 0;

    return lsm303agr_read_reg(dev, LSM303AGR_REG_INT2_CFG_A);
}

upm_result_t lsm303agr_set_acc_int2_config(const lsm303agr_context dev,
                                           uint8_t bits)
{
    assert(dev != NULL);

    if (!dev->i2cACC)
        return UPM_ERROR_NO_RESOURCES;

    if (lsm303agr_write_reg(dev, LSM303AGR_REG_INT2_CFG_A, bits))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t lsm303agr_get_mag_int_config(const lsm303agr_context dev)
{
    assert(dev != NULL);

    if (!dev->i2cMAG)
        return 0;

    return lsm303agr_read_reg(dev, LSM303AGR_REG_INT_CTRL_REG_M);
}

upm_result_t lsm303agr_set_mag_int_config(const lsm303agr_context dev,
                                          uint8_t bits)
{
    assert(dev != NULL);

    if (!dev->i2cMAG)
        return UPM_ERROR_NO_RESOURCES;

    if (lsm303agr_write_reg(dev, LSM303AGR_REG_INT_CTRL_REG_M, bits))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

uint8_t lsm303agr_get_acc_int1_src(const lsm303agr_context dev)
{
    assert(dev != NULL);

    if (!dev->i2cACC)
        return 0;

    return lsm303agr_read_reg(dev, LSM303AGR_REG_INT1_SRC_A);
}

uint8_t lsm303agr_get_acc_int2_src(const lsm303agr_context dev)
{
    assert(dev != NULL);

    if (!dev->i2cACC)
        return 0;

    return lsm303agr_read_reg(dev, LSM303AGR_REG_INT2_SRC_A);
}

uint8_t lsm303agr_get_mag_int_src(const lsm303agr_context dev)
{
    assert(dev != NULL);

    if (!dev->i2cMAG)
        return 0;

    return lsm303agr_read_reg(dev, LSM303AGR_REG_INT_SRC_REG_M);
}

upm_result_t lsm303agr_install_isr(const lsm303agr_context dev,
                                   LSM303AGR_INTERRUPT_PINS_T intr, int gpio,
                                   mraa_gpio_edge_t level,
                                   void (*isr)(void *), void *arg)
{
    assert(dev != NULL);

    // delete any existing ISR and GPIO context for this interrupt
    lsm303agr_uninstall_isr(dev, intr);

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
    case LSM303AGR_INTERRUPT_ACC_1:
        dev->gpioACC1 = gpio_isr;
        break;

    case LSM303AGR_INTERRUPT_ACC_2:
        dev->gpioACC2 = gpio_isr;
        break;

    case LSM303AGR_INTERRUPT_MAG:
        dev->gpioMAG = gpio_isr;
        break;
    }

    return UPM_SUCCESS;
}

void lsm303agr_uninstall_isr(const lsm303agr_context dev,
                             LSM303AGR_INTERRUPT_PINS_T intr)
{
    assert(dev != NULL);

    switch (intr)
    {
    case LSM303AGR_INTERRUPT_ACC_1:
        if (dev->gpioACC1)
        {
            mraa_gpio_isr_exit(dev->gpioACC1);
            mraa_gpio_close(dev->gpioACC1);
            dev->gpioACC1 = NULL;
        }
        break;

    case LSM303AGR_INTERRUPT_ACC_2:
        if (dev->gpioACC2)
        {
            mraa_gpio_isr_exit(dev->gpioACC2);
            mraa_gpio_close(dev->gpioACC2);
            dev->gpioACC2 = NULL;
        }
        break;

    case LSM303AGR_INTERRUPT_MAG:
        if (dev->gpioMAG)
        {
            mraa_gpio_isr_exit(dev->gpioMAG);
            mraa_gpio_close(dev->gpioMAG);
            dev->gpioMAG = NULL;
        }
        break;
    }
}
