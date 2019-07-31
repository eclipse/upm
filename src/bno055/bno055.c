/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <string.h>
#include <assert.h>

#include <upm_utilities.h>

#include "bno055.h"

// macro for converting a uint8_t low/high pair into a float
#define INT16_TO_FLOAT(l, h) \
    (float)( (int16_t)( (l) | ((h) << 8) ) )


// clear internal data items
static void _clear_data(const bno055_context dev)
{
    assert(dev != NULL);

    dev->magX = dev->magY = dev->magZ = 0;
    dev->accX = dev->accY = dev->accZ = 0;
    dev->gyrX = dev->gyrY = dev->gyrZ = 0;
    dev->eulHeading = dev->eulRoll = dev->eulPitch = 0;
    dev->quaW = dev->quaX = dev->quaY = dev->quaZ = 0;
    dev->liaX = dev->liaY = dev->liaZ = 0;
    dev->grvX = dev->grvY = dev->grvZ = 0;
}

// load fusion data
static upm_result_t _update_fusion_data(const bno055_context dev)
{
    assert(dev != NULL);

    // bail (with success code) if we are in config mode, or aren't in
    // a fusion mode...
    if (dev->currentMode == BNO055_OPERATION_MODE_CONFIGMODE ||
        dev->currentMode < BNO055_OPERATION_MODE_IMU)
        return UPM_SUCCESS;

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    // FIXME/MAYBE? - abort early if SYS calibration is == 0?

    const int fusionBytes = 26;
    uint8_t buf[fusionBytes];

    if (bno055_read_regs(dev, BNO055_REG_EUL_HEADING_LSB, buf, fusionBytes))
        return UPM_ERROR_OPERATION_FAILED;

    dev->eulHeading = INT16_TO_FLOAT(buf[0], buf[1]);
    dev->eulRoll    = INT16_TO_FLOAT(buf[2], buf[3]);
    dev->eulPitch   = INT16_TO_FLOAT(buf[4], buf[5]);

    dev->quaW       = INT16_TO_FLOAT(buf[6], buf[7]);
    dev->quaX       = INT16_TO_FLOAT(buf[8], buf[9]);
    dev->quaY       = INT16_TO_FLOAT(buf[10], buf[11]);
    dev->quaZ       = INT16_TO_FLOAT(buf[12], buf[13]);

    dev->liaX       = INT16_TO_FLOAT(buf[14], buf[15]);
    dev->liaY       = INT16_TO_FLOAT(buf[16], buf[17]);
    dev->liaZ       = INT16_TO_FLOAT(buf[18], buf[19]);

    dev->grvX       = INT16_TO_FLOAT(buf[20], buf[21]);
    dev->grvY       = INT16_TO_FLOAT(buf[22], buf[23]);
    dev->grvZ       = INT16_TO_FLOAT(buf[24], buf[25]);

    return UPM_SUCCESS;
}

// update non-fusion data
static upm_result_t _update_non_fusion_data(const bno055_context dev)
{
    assert(dev != NULL);

    // bail (with success code) if we are in config mode...
    if (dev->currentMode == BNO055_OPERATION_MODE_CONFIGMODE)
        return UPM_SUCCESS;

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    const int nonFusionBytes = 18;
    uint8_t buf[nonFusionBytes];

    if (bno055_read_regs(dev, BNO055_REG_ACC_DATA_X_LSB, buf, nonFusionBytes))
        return UPM_ERROR_OPERATION_FAILED;

    dev->accX = INT16_TO_FLOAT(buf[0], buf[1]);
    dev->accY = INT16_TO_FLOAT(buf[2], buf[3]);
    dev->accZ = INT16_TO_FLOAT(buf[4], buf[5]);

    dev->magX = INT16_TO_FLOAT(buf[6], buf[7]);
    dev->magY = INT16_TO_FLOAT(buf[8], buf[9]);
    dev->magZ = INT16_TO_FLOAT(buf[10], buf[11]);

    dev->gyrX = INT16_TO_FLOAT(buf[12], buf[13]);
    dev->gyrY = INT16_TO_FLOAT(buf[14], buf[15]);
    dev->gyrZ = INT16_TO_FLOAT(buf[16], buf[17]);

    return UPM_SUCCESS;
}

// init
bno055_context bno055_init(int bus, uint8_t addr, mraa_io_descriptor* descs)
{
    bno055_context dev =
        (bno055_context)malloc(sizeof(struct _bno055_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _bno055_context));

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        bno055_close(dev);
        return NULL;
    }

    if(descs)
    {
        if(!descs->i2cs)
        {
            bno055_close(dev);
            printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
            return NULL;
        }
        else
        {
            if( !(dev->i2c = descs->i2cs[0]) )
            {
                bno055_close(dev);
                printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
                return NULL;
            }
        }
    }
    else
    {
        if (!(dev->i2c = mraa_i2c_init(bus)))
        {
            printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
            bno055_close(dev);
            return NULL;
        }

        if (mraa_i2c_address(dev->i2c, addr) != MRAA_SUCCESS)
        {
            printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
            bno055_close(dev);
            return NULL;
        }
    }

    _clear_data(dev);

    // forcibly set page 0, so we are synced with the device
    if (bno055_set_page(dev, 0, true))
    {
        printf("%s: bno055_set_page() failed.\n", __FUNCTION__);
        bno055_close(dev);
        return NULL;
    }

    // check the chip id.  This has to be done after forcibly setting
    // page 0, as that is the only page where the chip id is present.
    uint8_t chipID = 0;
    if (bno055_get_chip_id(dev, &chipID))
    {
        printf("%s: Could not read chip id\n", __FUNCTION__);
        bno055_close(dev);
        return NULL;
    }

    if (chipID != BNO055_CHIPID)
    {
        printf("%s: Invalid chip ID. Expected 0x%02x, got 0x%02x\n",
               __FUNCTION__, BNO055_CHIPID, chipID);
        bno055_close(dev);
        return NULL;
    }

    upm_result_t urv = UPM_SUCCESS;
    // set config mode
    urv += bno055_set_operation_mode(dev, BNO055_OPERATION_MODE_CONFIGMODE);

    // default to internal clock
    urv += bno055_set_clock_external(dev, false);

    // we specifically avoid doing a reset so that if the device is
    // already calibrated, it will remain so.

    // we always use C for temperature
    urv += bno055_set_temperature_units_celsius(dev);

    // default to accelerometer temp
    urv += bno055_set_temperature_source(dev, BNO055_TEMP_SOURCE_ACC);

    // set accel units to m/s^2
    urv += bno055_set_accelerometer_units(dev, false);

    // set gyro units to degrees
    urv += bno055_set_gyroscope_units(dev, false);

    // set Euler units to degrees
    urv += bno055_set_euler_units(dev, false);

    // by default, we set the operating mode to the NDOF fusion mode
    urv += bno055_set_operation_mode(dev, BNO055_OPERATION_MODE_NDOF);

    // if any of those failed, bail
    if (urv != UPM_SUCCESS)
    {
        printf("%s: Initial device configuration failed\n", __FUNCTION__);
        bno055_close(dev);
        return NULL;
    }

    return dev;
}

void bno055_close(bno055_context dev)
{
    assert(dev != NULL);

    bno055_uninstall_isr(dev);

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);

    free(dev);
}

upm_result_t bno055_update(const bno055_context dev)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    // temperature first, always in Celsius
    uint8_t tempreg = 0;
    if (bno055_read_reg(dev, BNO055_REG_TEMPERATURE, &tempreg))
        return UPM_ERROR_OPERATION_FAILED;

    dev->temperature = (float)((int8_t)tempreg);

    if (_update_fusion_data(dev))
        return UPM_ERROR_OPERATION_FAILED;
    if (_update_non_fusion_data(dev))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bno055_read_reg(const bno055_context dev, uint8_t reg,
                             uint8_t *retval)
{
    assert(dev != NULL);

    int rv = mraa_i2c_read_byte_data(dev->i2c, reg);
    if (rv < 0)
    {
        printf("%s: mraa_i2c_read_byte_data() failed\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    if (retval)
        *retval = (uint8_t)(rv & 0xff);

    return UPM_SUCCESS;
}

upm_result_t bno055_read_regs(const bno055_context dev, uint8_t reg,
                              uint8_t *buffer, size_t len)
{
    assert(dev != NULL);

    if (mraa_i2c_read_bytes_data(dev->i2c, reg, buffer, len) < 0)
    {
        printf("%s: mraa_i2c_read_bytes() failed\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t bno055_write_reg(const bno055_context dev,
                              uint8_t reg, uint8_t val)
{
    assert(dev != NULL);

    if (mraa_i2c_write_byte_data(dev->i2c, val, reg))
    {
        printf("%s: mraa_i2c_write_byte_data() failed\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t bno055_write_regs(const bno055_context dev, uint8_t reg,
                               uint8_t *buffer, size_t len)
{
    assert(dev != NULL);

    uint8_t buf[len + 1];

    buf[0] = reg;
    for (size_t i=0; i<len; i++)
        buf[i+1] = buffer[i];

    if (mraa_i2c_write(dev->i2c, buf, len + 1))
    {
        printf("%s: mraa_i2c_write() failed\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t bno055_get_chip_id(const bno055_context dev, uint8_t *chip_id)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    return bno055_read_reg(dev, BNO055_REG_CHIP_ID, chip_id);
}

upm_result_t bno055_get_acc_id(const bno055_context dev, uint8_t *chip_id)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    return bno055_read_reg(dev, BNO055_REG_ACC_ID, chip_id);
}

upm_result_t bno055_get_mag_id(const bno055_context dev, uint8_t *chip_id)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    return bno055_read_reg(dev, BNO055_REG_MAG_ID, chip_id);
}

upm_result_t bno055_get_gyr_id(const bno055_context dev, uint8_t *chip_id)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    return bno055_read_reg(dev, BNO055_REG_GYR_ID, chip_id);
}

upm_result_t bno055_get_sw_revision(const bno055_context dev, uint16_t *sw_rev)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t lsb = 0, msb = 0;
    if (bno055_read_reg(dev, BNO055_REG_SW_REV_ID_LSB, &lsb))
        return UPM_ERROR_OPERATION_FAILED;
    if (bno055_read_reg(dev, BNO055_REG_SW_REV_ID_MSB, &msb))
        return UPM_ERROR_OPERATION_FAILED;

    if (sw_rev)
        *sw_rev = (uint16_t)(lsb | (msb << 8));

    return UPM_SUCCESS;
}

upm_result_t bno055_get_bootloader_id(const bno055_context dev, uint8_t *bl_id)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    return bno055_read_reg(dev, BNO055_REG_BL_REV_ID, bl_id);
}

upm_result_t bno055_set_page(const bno055_context dev, uint8_t page,
                             bool force)
{
    assert(dev != NULL);

    // page can only be 0 or 1
    if (!(page == 0 || page == 1))
    {
        printf("%s: page number can only be 0 or 1.\n",
               __FUNCTION__);
        return UPM_ERROR_INVALID_PARAMETER;
    }

    if (force || page != dev->currentPage)
    {
        if (bno055_write_reg(dev, BNO055_REG_PAGE_ID, page))
            return UPM_ERROR_OPERATION_FAILED;
    }

    dev->currentPage = page;
    return UPM_SUCCESS;
}

upm_result_t bno055_set_clock_external(const bno055_context dev,
                                       bool extClock)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    // first we need to be in config mode
    BNO055_OPERATION_MODES_T currentMode = dev->currentMode;
    if (bno055_set_operation_mode(dev, BNO055_OPERATION_MODE_CONFIGMODE))
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg = 0;
    if (bno055_read_reg(dev, BNO055_REG_SYS_TRIGGER, &reg))
        return UPM_ERROR_OPERATION_FAILED;

    if (extClock)
        reg |= BNO055_SYS_TRIGGER_CLK_SEL;
    else
        reg &= ~BNO055_SYS_TRIGGER_CLK_SEL;

    if (bno055_write_reg(dev, BNO055_REG_SYS_TRIGGER, reg))
        return UPM_ERROR_OPERATION_FAILED;

    // now reset our operating mode
    if (bno055_set_operation_mode(dev, currentMode))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bno055_set_temperature_source(const bno055_context dev,
                                           BNO055_TEMP_SOURCES_T src)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    return bno055_write_reg(dev, BNO055_REG_TEMP_SOURCE, src);
}

upm_result_t bno055_set_temperature_units_celsius(const bno055_context dev)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg = 0;
    if (bno055_read_reg(dev, BNO055_REG_UNIT_SEL, &reg))
        return UPM_ERROR_OPERATION_FAILED;

    reg &= ~BNO055_UNIT_SEL_TEMP_UNIT;

    return bno055_write_reg(dev, BNO055_REG_UNIT_SEL, reg);
}

upm_result_t bno055_set_accelerometer_units(const bno055_context dev, bool mg)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg = 0;
    if (bno055_read_reg(dev, BNO055_REG_UNIT_SEL, &reg))
        return UPM_ERROR_OPERATION_FAILED;

    if (mg)
    {
        reg |= BNO055_UNIT_SEL_ACC_UNIT;
        dev->accUnitScale = 1.0;
    }
    else
    {
        reg &= ~BNO055_UNIT_SEL_ACC_UNIT;
        dev->accUnitScale = 100.0;
    }

    return bno055_write_reg(dev, BNO055_REG_UNIT_SEL, reg);
}

upm_result_t bno055_set_gyroscope_units(const bno055_context dev, bool radians)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg = 0;
    if (bno055_read_reg(dev, BNO055_REG_UNIT_SEL, &reg))
        return UPM_ERROR_OPERATION_FAILED;

    if (radians)
    {
        reg |= BNO055_UNIT_SEL_GYR_UNIT;
        dev->gyrUnitScale = 900.0;
    }
    else
    {
        reg &= ~BNO055_UNIT_SEL_GYR_UNIT;
        dev->gyrUnitScale = 16.0;
    }

    return bno055_write_reg(dev, BNO055_REG_UNIT_SEL, reg);
}

upm_result_t bno055_set_euler_units(const bno055_context dev, bool radians)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg = 0;
    if (bno055_read_reg(dev, BNO055_REG_UNIT_SEL, &reg))
        return UPM_ERROR_OPERATION_FAILED;

    if (radians)
    {
        reg |= BNO055_UNIT_SEL_EUL_UNIT;
        dev->eulUnitScale = 900.0;
    }
    else
    {
        reg &= ~BNO055_UNIT_SEL_EUL_UNIT;
        dev->eulUnitScale = 16.0;
    }

    return bno055_write_reg(dev, BNO055_REG_UNIT_SEL, reg);
}

upm_result_t bno055_set_operation_mode(const bno055_context dev,
                                       BNO055_OPERATION_MODES_T mode)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    // we clear all of our loaded data on mode changes
    _clear_data(dev);

    uint8_t reg = 0;
    if (bno055_read_reg(dev, BNO055_REG_OPER_MODE, &reg))
        return UPM_ERROR_OPERATION_FAILED;

    reg &= ~(_BNO055_OPR_MODE_OPERATION_MODE_MASK
             << _BNO055_OPR_MODE_OPERATION_MODE_SHIFT);

    reg |= (mode << _BNO055_OPR_MODE_OPERATION_MODE_SHIFT);

    if (bno055_write_reg(dev, BNO055_REG_OPER_MODE, reg))
        return UPM_ERROR_OPERATION_FAILED;

    dev->currentMode = mode;

    upm_delay_us(30);

    return UPM_SUCCESS;
}

upm_result_t bno055_get_calibration_status(const bno055_context dev,
                                           int *mag, int *acc,
                                           int *gyr, int *sys)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg = 0;
    if (bno055_read_reg(dev, BNO055_REG_CALIB_STAT, &reg))
        return UPM_ERROR_OPERATION_FAILED;

    if (mag)
        *mag = (reg >> _BNO055_CALIB_STAT_MAG_SHIFT)
            & _BNO055_CALIB_STAT_MAG_MASK;

    if (acc)
        *acc = (reg >> _BNO055_CALIB_STAT_ACC_SHIFT)
            & _BNO055_CALIB_STAT_ACC_MASK;

    if (gyr)
        *gyr = (reg >> _BNO055_CALIB_STAT_GYR_SHIFT)
            & _BNO055_CALIB_STAT_GYR_MASK;

    if (sys)
        *sys = (reg >> _BNO055_CALIB_STAT_SYS_SHIFT)
            & _BNO055_CALIB_STAT_SYS_MASK;

    return UPM_SUCCESS;
}

bool bno055_is_fully_calibrated(const bno055_context dev)
{
    assert(dev != NULL);

    int mag, acc, gyr, sys;

    // fail on error
    if (bno055_get_calibration_status(dev, &mag, &acc, &gyr, &sys))
        return false;

    // all of them equal to 3 means fully calibrated
    if (mag == 3 && acc == 3 && gyr == 3 && sys == 3)
        return true;
    else
        return false;
}

upm_result_t bno055_reset_system(const bno055_context dev)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg = 0;
    if (bno055_read_reg(dev, BNO055_REG_SYS_TRIGGER, &reg))
        return UPM_ERROR_OPERATION_FAILED;

    reg |= BNO055_SYS_TRIGGER_RST_SYS;

    if (bno055_write_reg(dev, BNO055_REG_SYS_TRIGGER, reg))
        return UPM_ERROR_OPERATION_FAILED;

    upm_delay(1);

    return UPM_SUCCESS;
}

upm_result_t bno055_reset_interrupt_status(const bno055_context dev)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg = 0;
    if (bno055_read_reg(dev, BNO055_REG_SYS_TRIGGER, &reg))
        return UPM_ERROR_OPERATION_FAILED;

    reg |= BNO055_SYS_TRIGGER_RST_INT;

    return bno055_write_reg(dev, BNO055_REG_SYS_TRIGGER, reg);
}

upm_result_t bno055_get_interrupt_status(const bno055_context dev,
                                         uint8_t *istat)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    return bno055_read_reg(dev, BNO055_REG_INT_STA, istat);
}

upm_result_t bno055_get_interrupt_enable(const bno055_context dev,
                                         uint8_t *ienable)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 1, false))
        return UPM_ERROR_OPERATION_FAILED;

    return bno055_read_reg(dev, BNO055_REG_INT_EN, ienable);
}

upm_result_t bno055_set_interrupt_enable(const bno055_context dev,
                                         uint8_t enables)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 1, false))
        return UPM_ERROR_OPERATION_FAILED;

    return bno055_write_reg(dev, BNO055_REG_INT_EN, enables);
}

upm_result_t bno055_get_interrupt_mask(const bno055_context dev,
                                       uint8_t *imask)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 1, false))
        return UPM_ERROR_OPERATION_FAILED;

    return bno055_read_reg(dev, BNO055_REG_INT_MSK, imask);
}

upm_result_t bno055_set_interrupt_mask(const bno055_context dev, uint8_t mask)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 1, false))
        return UPM_ERROR_OPERATION_FAILED;

    return bno055_write_reg(dev, BNO055_REG_INT_MSK, mask);
}

upm_result_t bno055_get_system_status(const bno055_context dev,
    BNO055_SYS_STATUS_T *sys_stat)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg = 0;
    if (bno055_read_reg(dev, BNO055_REG_SYS_STATUS, &reg))
        return UPM_ERROR_OPERATION_FAILED;

    if (sys_stat)
        *sys_stat = (BNO055_SYS_STATUS_T)reg;

    return UPM_SUCCESS;
}

upm_result_t bno055_get_system_error(const bno055_context dev,
                                     BNO055_SYS_ERR_T *sys_err)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg = 0;
    if (bno055_read_reg(dev, BNO055_REG_SYS_ERROR, &reg))
        return UPM_ERROR_OPERATION_FAILED;

    if (sys_err)
        *sys_err = (BNO055_SYS_ERR_T)reg;

    return UPM_SUCCESS;
}

upm_result_t bno055_read_calibration_data(const bno055_context dev,
                                          uint8_t *data, size_t len)
{
    assert(dev != NULL);
    assert(data != NULL);

    if (!bno055_is_fully_calibrated(dev))
    {
        printf("%s: Sensor must be fully calibrated first.\n",
               __FUNCTION__);
        return UPM_ERROR_NO_DATA;
    }

    if (len != BNO055_CALIBRATION_DATA_SIZE)
    {
        printf("%s: len must equal BNO055_CALIBRATION_DATA_SIZE (%d).\n",
               __FUNCTION__, BNO055_CALIBRATION_DATA_SIZE);
        return UPM_ERROR_INVALID_SIZE;
    }

    // should be at page 0, but lets make sure
    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    // first we need to go back into config mode
    BNO055_OPERATION_MODES_T currentMode = dev->currentMode;
    if (bno055_set_operation_mode(dev, BNO055_OPERATION_MODE_CONFIGMODE))
        return UPM_ERROR_OPERATION_FAILED;

    if (bno055_read_regs(dev, BNO055_REG_ACC_OFFSET_X_LSB, data,
                         BNO055_CALIBRATION_DATA_SIZE))
        return UPM_ERROR_OPERATION_FAILED;

    // now reset our operating mode
    if (bno055_set_operation_mode(dev, currentMode))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t bno055_write_calibration_data(const bno055_context dev,
                                           uint8_t *data,
                                           size_t len)
{
    assert(dev != NULL);
    assert(data != NULL);

    if (len != BNO055_CALIBRATION_DATA_SIZE)
    {
        printf("%s: len must equal BNO055_CALIBRATION_DATA_SIZE "
               "(expected %d, got %d).\n",
               __FUNCTION__, BNO055_CALIBRATION_DATA_SIZE, (int)len);
        return UPM_ERROR_INVALID_SIZE;
    }

    // should be at page 0, but lets make sure
    if (bno055_set_page(dev, 0, false))
        return UPM_ERROR_OPERATION_FAILED;

    // first we need to go back into config mode
    BNO055_OPERATION_MODES_T currentMode = dev->currentMode;
    if (bno055_set_operation_mode(dev, BNO055_OPERATION_MODE_CONFIGMODE))
        return UPM_ERROR_OPERATION_FAILED;

    // write the data
    if (bno055_write_regs(dev, BNO055_REG_ACC_OFFSET_X_LSB, data,
                          BNO055_CALIBRATION_DATA_SIZE))
        return UPM_ERROR_OPERATION_FAILED;

    // now reset our operating mode
    if (bno055_set_operation_mode(dev, currentMode))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

float bno055_get_temperature(const bno055_context dev)
{
    assert(dev != NULL);

    return dev->temperature;
}

void bno055_get_euler_angles(const bno055_context dev, float *heading,
                             float *roll, float *pitch)
{
    assert(dev != NULL);

    if (heading)
        *heading = dev->eulHeading / dev->eulUnitScale;

    if (roll)
        *roll = dev->eulRoll / dev->eulUnitScale;

    if (pitch)
        *pitch = dev->eulPitch / dev->eulUnitScale;
}

void bno055_get_quaternions(const bno055_context dev, float *w, float *x,
                            float *y, float *z)
{
    assert(dev != NULL);

    // from the datasheet
    const float scale = (float)(1.0 / (float)(1 << 14));

    if (w)
        *w = dev->quaW * scale;

    if (x)
        *x = dev->quaX * scale;

    if (y)
        *y = dev->quaY * scale;

    if (z)
        *z = dev->quaZ * scale;
}

void bno055_get_linear_acceleration(const bno055_context dev, float *x,
                                    float *y, float *z)
{
    assert(dev != NULL);

    if (x)
        *x = dev->liaX / dev->accUnitScale;

    if (y)
        *y = dev->liaY / dev->accUnitScale;

    if (z)
        *z = dev->liaZ / dev->accUnitScale;
}

void bno055_get_gravity_vectors(const bno055_context dev,
                                float *x, float *y, float *z)
{
    assert(dev != NULL);

    if (x)
        *x = dev->grvX / dev->accUnitScale;

    if (y)
        *y = dev->grvY / dev->accUnitScale;

    if (z)
        *z = dev->grvZ / dev->accUnitScale;
}

void bno055_get_accelerometer(const bno055_context dev, float *x, float *y,
                              float *z)
{
    assert(dev != NULL);

    if (x)
        *x = dev->accX / dev->accUnitScale;

    if (y)
        *y = dev->accY / dev->accUnitScale;

    if (z)
        *z = dev->accZ / dev->accUnitScale;
}

void bno055_get_magnetometer(const bno055_context dev, float *x, float *y,
                             float *z)
{
    assert(dev != NULL);

    // from the datasheet - 16 uT's per LSB
    const float scale = 16.0;

    if (x)
        *x = dev->magX / scale;

    if (y)
        *y = dev->magY / scale;

    if (z)
        *z = dev->magZ / scale;
}

void bno055_get_gyroscope(const bno055_context dev,
                          float *x, float *y, float *z)
{
    assert(dev != NULL);

    if (x)
        *x = dev->gyrX / dev->gyrUnitScale;

    if (y)
        *y = dev->gyrY / dev->gyrUnitScale;

    if (z)
        *z = dev->gyrZ / dev->gyrUnitScale;
}

upm_result_t bno055_set_acceleration_config(const bno055_context dev,
                                            BNO055_ACC_RANGE_T range,
                                            BNO055_ACC_BW_T bw,
                                            BNO055_ACC_PWR_MODE_T pwr)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 1, false))
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg = ((range << _BNO055_ACC_CONFIG_ACC_RANGE_SHIFT)
                   | (bw << _BNO055_ACC_CONFIG_ACC_BW_SHIFT)
                   | (pwr << _BNO055_ACC_CONFIG_ACC_PWR_MODE_SHIFT));

    return bno055_write_reg(dev, BNO055_REG_ACC_CONFIG, reg);
}

upm_result_t bno055_set_magnetometer_config(const bno055_context dev,
                                            BNO055_MAG_ODR_T odr,
                                            BNO055_MAG_OPR_T opr,
                                            BNO055_MAG_POWER_T pwr)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 1, false))
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg = ((odr << _BNO055_MAG_CONFIG_MAG_ODR_SHIFT)
                   | (opr << _BNO055_MAG_CONFIG_MAG_OPR_MODE_SHIFT)
                   | (pwr << _BNO055_MAG_CONFIG_MAG_POWER_MODE_SHIFT));

    return bno055_write_reg(dev, BNO055_REG_MAG_CONFIG, reg);
}

upm_result_t bno055_set_gyroscope_config(const bno055_context dev,
                                         BNO055_GYR_RANGE_T range,
                                         BNO055_GYR_BW_T bw,
                                         BNO055_GYR_POWER_MODE_T pwr)
{
    assert(dev != NULL);

    if (bno055_set_page(dev, 1, false))
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg = ((range << _BNO055_GYR_CONFIG0_GYR_RANGE_SHIFT)
                   | (bw << _BNO055_GYR_CONFIG0_GYR_BW_SHIFT));

    if (bno055_write_reg(dev, BNO055_REG_GYR_CONFIG0, reg))
        return UPM_ERROR_OPERATION_FAILED;

    reg = (pwr << _BNO055_GYR_CONFIG1_GYR_POWER_MODE_SHIFT);

    return bno055_write_reg(dev, BNO055_REG_GYR_CONFIG1, reg);
}

upm_result_t bno055_install_isr(const bno055_context dev,
                                int gpio, mraa_gpio_edge_t level,
                                void (*isr)(void *), void *arg)
{
    assert(dev != NULL);

    // delete any existing ISR and GPIO context
    bno055_uninstall_isr(dev);

    // create gpio context
    if (!(dev->gpio = mraa_gpio_init(gpio)))
    {
        printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN);

    if (mraa_gpio_isr(dev->gpio, level, isr, arg))
    {
        mraa_gpio_close(dev->gpio);
        dev->gpio = NULL;
        printf("%s: mraa_gpio_isr() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

void bno055_uninstall_isr(const bno055_context dev)
{
    assert(dev != NULL);

    if (dev->gpio)
    {
        mraa_gpio_isr_exit(dev->gpio);
        mraa_gpio_close(dev->gpio);
        dev->gpio = NULL;
    }
}
