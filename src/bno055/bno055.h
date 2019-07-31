/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <upm.h>

#include <mraa/i2c.h>
#include <mraa/gpio.h>
#include <mraa/initio.h>

#include "bno055_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file bno055.h
     * @library bno055
     * @brief C API for the bno055 driver
     *
     * @include bno055.c
     */

    /**
     * Device context
     */
    typedef struct _bno055_context {
        mraa_i2c_context i2c;
        mraa_gpio_context gpio;

        // always stored in C
        float temperature;

        int currentPage;
        BNO055_OPERATION_MODES_T currentMode;

        // uncompensated data

        // mag data
        float magX;
        float magY;
        float magZ;

        // acc data
        float accX;
        float accY;
        float accZ;

        // acc units
        float accUnitScale;

        // gyr data
        float gyrX;
        float gyrY;
        float gyrZ;

        // gyr units
        float gyrUnitScale;

        // eul (euler angle) data
        float eulHeading;
        float eulRoll;
        float eulPitch;

        // eul units
        float eulUnitScale;

        // qua (quaternion) data
        float quaW;
        float quaX;
        float quaY;
        float quaZ;

        // lia (linear acceleration) data
        float liaX;
        float liaY;
        float liaZ;

        // grv (gravity vector) data
        float grvX;
        float grvY;
        float grvZ;
    } *bno055_context;

    /**
     * BNO055 initialization.
     *
     * By default, this function sets the acceleration units to m/s^2,
     * gyro and Euler units to degrees, and temperature to celsius.
     * It then enters the NDOF fusion mode.
     *
     * In addition, the internal clock is used so that compatibility
     * with other implementations is assured.  If you are using a
     * device with an external clock, call setClockExternal(true) to
     * enable it.
     *
     * @param bus I2C bus to use.
     * @param address The I2C address for this device.
     * @return The device context, or NULL if an error occurred.
     */
    bno055_context bno055_init(int bus, uint8_t addr, mraa_io_descriptor* descs);

    /**
     * BNO055 close function.
     *
     * @param dev The device context.
     */
    void bno055_close(bno055_context dev);

    /**
     * Update the internal stored values from sensor data.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bno055_update(const bno055_context dev);

    /**
     * Return the chip ID.
     *
     * @param dev The device context.
     * @param chip_id The chip ID (BNO055_CHIPID).
     * @return UPM result.
     */
    upm_result_t bno055_get_chip_id(const bno055_context dev,
                                    uint8_t *chip_id);

    /**
     * Return the accelerometer chip ID.
     *
     * @param dev The device context.
     * @param chip_id The chip ID
     * @return UPM result.
     */
    upm_result_t bno055_get_acc_id(const bno055_context dev,
                                   uint8_t *chip_id);

    /**
     * Return the magnetometer chip ID.
     *
     * @param dev The device context.
     * @param chip_id The chip ID
     * @return UPM result.
     */
    upm_result_t bno055_get_mag_id(const bno055_context dev,
                                   uint8_t *chip_id);

    /**
     * Return the gyroscope chip ID.
     *
     * @param dev The device context.
     * @param chip_id The chip ID
     * @return UPM result.
     */
    upm_result_t bno055_get_gyr_id(const bno055_context dev,
                                   uint8_t *chip_id);

    /**
     * Return the fusion firmware revison.
     *
     * @param dev The device context.
     * @param sw_rev The firmware revison.
     * @return UPM result.
     */
    upm_result_t bno055_get_sw_revision(const bno055_context dev,
                                        uint16_t *sw_rev);

    /**
     * Return the bootloader ID.
     *
     * @param dev The device context.
     * @param bl_id The bootloader ID.
     * @return UPM result.
     */
    upm_result_t bno055_get_bootloader_id(const bno055_context dev,
                                          uint8_t *bl_id);

    /**
     * Enable or disables the use of the external clock.  The Adafriut
     * device does contain an external clock which might be more
     * stable.  By default, the internal clock is used.
     *
     * @param dev The device context.
     * @param extClock true to use external clock, false otherwise.
     * @return UPM result.
     */
    upm_result_t bno055_set_clock_external(const bno055_context dev,
                                           bool extClock);

    /**
     * Select the temperature source.  This can be the accelerometer
     * or the gyroscope.  By default, the accelerometer temperature is
     * used as the source.
     *
     * @param dev The device context.
     * @param src One of the TEMP_SOURCES_T values.
     * @return UPM result.
     */
    upm_result_t bno055_set_temperature_source(const bno055_context dev,
                                               BNO055_TEMP_SOURCES_T src);

    /**
     * Set the temperature units of the device to Celsius (always).
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bno055_set_temperature_units_celsius(const bno055_context dev);

    /**
     * Set the operating mode for the device.  This places the device
     * into a config mode, one of 7 non-fusion modes, or one of 5
     * fusion modes.  All stored sensor data is cleared when switching
     * modes.  The device must be in config mode for most
     * configuration operations.  See the datasheet for details.
     *
     * @param dev The device context.
     * @param mode One of the OPERATION_MODES_T values.
     * @return UPM result.
     */
    upm_result_t bno055_set_operation_mode(const bno055_context dev,
                                           BNO055_OPERATION_MODES_T mode);

    /**
     * Reboot the sensor.  This is equivalent to a power on reset.
     * All calibration data will be lost, and the device must be
     * re-calibrated.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bno055_reset_system(const bno055_context dev);

    /**
     * Read the calibration status registers and return them.  The
     * values range from 0 (uncalibrated) to 3 (fully calibrated).
     *
     * @param dev The device context.
     * @param mag The calibration status of the magnetometer.
     * @param acc The calibration status of the accelerometer.
     * @param mag The calibration status of the gyroscope.
     * @param mag The calibration status of the overall system.
     * @return UPM result.
     */
    upm_result_t bno055_get_calibration_status(const bno055_context dev,
                                               int *mag, int *acc,
                                               int *gyr, int *sys);

    /**
     * Read the calibration status registers and return true or false,
     * indicating whether all of the calibration parameters are fully
     * calibrated.
     *
     * @param dev The device context.
     * @return true if all 4 calibration parameters are fully
     * calibrated, else false.
     */
    bool bno055_is_fully_calibrated(const bno055_context dev);

    /**
     * Read the calibration data and place it into a buffer.  The
     * buffer must be BNO055_CALIBRATION_DATA_SIZE bytes in size.
     * This data can then be saved for later reuse by
     * bno055_write_calibration_data() to restore calibration data
     * after a reset.  Note: bno055_is_fully_calibrated() (calibration
     * is complete) must be true or an error will result.
     *
     * @param dev The device context.
     * @param data A pointer to a buffer to hold the calibration data.
     * This buffer must be BNO055_CALIBRATION_DATA_SIZE bytes in
     * length.
     * @param len The length of the buffer.  An error will be returned
     * if the length is not equal to BNO055_CALIBRATION_DATA_SIZE.
     * @return UPM result.
     */
    upm_result_t bno055_read_calibration_data(const bno055_context dev,
                                              uint8_t *data, size_t len);

    /**
     * Write previously saved calibration data to the calibration
     * registers.  The array containing the data must be
     * BNO055_CALIBRATION_DATA_SIZE bytes in size.
     *
     * @param dev The device context.
     * @param data uint8_t array representing calibration data, as obtained
     * by bno055_read_calibration_data().
     * @param len The length of the supplied buffer.  An error will be
     * returned if the length is not equal to BNO055_CALIBRATION_DATA_SIZE.
     * @return UPM result.
     */
    upm_result_t bno055_write_calibration_data(const bno055_context dev,
                                               uint8_t *data,
                                               size_t len);

    /**
     * Return the current measured temperature.  Note, this is not
     * ambient temperature - this is the temperature of the selected
     * source on the chip.  update() must have been called prior to
     * calling this method.
     *
     * @param dev The device context.
     * @return The temperature in degrees Celsius.
     */
    float bno055_get_temperature(const bno055_context dev);

    /**
     * Return current orientation fusion data in the form of Euler
     * Angles.  By default, the returned values are in degrees.
     * update() must have been called prior to calling this method.
     *
     * @param dev The device context.
     * @param heading Pointer to a floating point value that will have
     * the current heading angle placed into it.
     * @param roll Pointer to a floating point value that will have
     * the current roll angle placed into it.
     * @param pitch Pointer to a floating point value that will have
     * the current pitch angle placed into it.
     */
    void bno055_get_euler_angles(const bno055_context dev, float *heading,
                                 float *roll, float *pitch);

    /**
     * Return current orientation fusion data in the form of
     * Quaternions.  update() must have been called prior to calling
     * this method.
     *
     * @param dev The device context.
     * @param w Pointer to a floating point value that will have
     * the current w component placed into it.
     * @param x Pointer to a floating point value that will have
     * the current x component placed into it.
     * @param y Pointer to a floating point value that will have
     * the current y component placed into it.
     * @param z Pointer to a floating point value that will have
     * the current z component placed into it.
     */
    void bno055_get_quaternions(const bno055_context dev, float *w, float *x,
                                float *y, float *z);

    /**
     * Return current orientation fusion data in the form of Linear
     * Acceleration.  By default the returned values are in meters
     * per-second squared (m/s^2).  update() must have been called
     * prior to calling this method.
     *
     * @param dev The device context.
     * @param x Pointer to a floating point value that will have
     * the current x component placed into it.
     * @param y Pointer to a floating point value that will have
     * the current y component placed into it.
     * @param z Pointer to a floating point value that will have
     * the current z component placed into it.
     */
    void bno055_get_linear_acceleration(const bno055_context dev, float *x,
                                        float *y, float *z);

    /**
     * Return current orientation fusion data in the form of a Gravity
     * Vector per-axis.  By default the returned values are in meters
     * per-second squared (m/s^2).  update() must have been called
     * prior to calling this method.
     *
     * @param dev The device context.
     * @param x Pointer to a floating point value that will have
     * the current x component placed into it.
     * @param y Pointer to a floating point value that will have
     * the current y component placed into it.
     * @param z Pointer to a floating point value that will have
     * the current z component placed into it.
     */
    void bno055_get_gravity_vectors(const bno055_context dev,
                                    float *x, float *y, float *z);

    /**
     * Return accelerometer data (non-fusion).  In fusion modes, this
     * data will be of little value.  By default the returned values
     * are in meters per-second squared (m/s^2).  update() must have
     * been called prior to calling this method.
     *
     * @param dev The device context.
     * @param x Pointer to a floating point value that will have
     * the current x component placed into it.
     * @param y Pointer to a floating point value that will have
     * the current y component placed into it.
     * @param z Pointer to a floating point value that will have
     * the current z component placed into it.
     */
    void bno055_get_accelerometer(const bno055_context dev, float *x, float *y,
                                  float *z);

    /**
     * Return magnetometer data (non-fusion).  In fusion modes, this
     * data will be of little value.  The returned values are in
     * micro-teslas (uT).  update() must have been called prior to
     * calling this method.
     *
     * @param dev The device context.
     * @param x Pointer to a floating point value that will have
     * the current x component placed into it.
     * @param y Pointer to a floating point value that will have
     * the current y component placed into it.
     * @param z Pointer to a floating point value that will have
     * the current z component placed into it.
     */
    void bno055_get_magnetometer(const bno055_context dev, float *x, float *y,
                                 float *z);

    /**
     * Return gyroscope data (non-fusion).  In fusion modes, this data
     * will be of little value.  By default the returned values are in
     * meters per-second squared (m/s^2).  update() must have been
     * called prior to calling this method.
     *
     * @param dev The device context.
     * @param x Pointer to a floating point value that will have
     * the current x component placed into it.
     * @param y Pointer to a floating point value that will have
     * the current y component placed into it.
     * @param z Pointer to a floating point value that will have
     * the current z component placed into it.
     */
    void bno055_get_gyroscope(const bno055_context dev,
                              float *x, float *y, float *z);

    /**
     * Set the bandwidth, range, and power modes of the accelerometer.
     * In fusion modes, these values will be ignored.
     *
     * @param dev The device context.
     * @param range One of the BNO055_ACC_RANGE_T values.
     * @param bw One of the BNO055_ACC_BW_T values.
     * @param pwr One of the BNO055_ACC_PWR_MODE_T values.
     * @return UPM result.
     */
    upm_result_t bno055_set_acceleration_config(const bno055_context dev,
                                                BNO055_ACC_RANGE_T range,
                                                BNO055_ACC_BW_T bw,
                                                BNO055_ACC_PWR_MODE_T pwr);

    /**
     * Set the output data rate, operating mode and power mode of the
     * magnetometer.  In fusion modes, these values will be ignored.
     *
     * @param dev The device context.
     * @param odr One of the BNO055_MAG_ODR_T values.
     * @param opr One of the BNO055_MAG_OPR_T values.
     * @param pwr One of the BNO055_MAG_POWER_T values.
     * @return UPM result.
     */
    upm_result_t bno055_set_magnetometer_config(const bno055_context dev,
                                                BNO055_MAG_ODR_T odr,
                                                BNO055_MAG_OPR_T opr,
                                                BNO055_MAG_POWER_T pwr);

    /**
     * Set the range, bandwidth and power modes of the gyroscope.  In
     * fusion modes, these values will be ignored.
     *
     * @param dev The device context.
     * @param range One of the BNO055_GYR_RANGE_T values.
     * @param bw One of the BNO055_GYR_BW_T values.
     * @param pwr One of the BNO055_GYR_POWER_MODE_T values.
     * @return UPM result.
     */
    upm_result_t bno055_set_gyroscope_config(const bno055_context dev,
                                             BNO055_GYR_RANGE_T range,
                                             BNO055_GYR_BW_T bw,
                                             BNO055_GYR_POWER_MODE_T pwr);

    /**
     * Set the unit of measurement for the accelerometer related
     * sensor values.  The choices are mg (milli-gravities) or meters
     * per-second squared (m/s^2).  The default is m/s^2.
     *
     * @param dev The device context.
     * @param mg true for mg, false for m/s^2.
     * @return UPM result.
     */
    upm_result_t bno055_set_accelerometer_units(const bno055_context dev,
                                                bool mg);

    /**
     * Set the unit of measurement for the gyroscope related sensor
     * values.  The choices are degrees and radians.  The default is
     * degrees.
     *
     * @param dev The device context.
     * @param radians true for radians, false for degrees.
     * @return UPM result.
     */
    upm_result_t bno055_set_gyroscope_units(const bno055_context dev,
                                            bool radians);

    /**
     * Set the unit of measurement for the Euler Angle related sensor
     * values.  The choices are degrees and radians.  The default is
     * degrees.
     *
     * @param dev The device context.
     * @param radians true for radians, false for degrees.
     * @return UPM result.
     */
    upm_result_t bno055_set_euler_units(const bno055_context dev,
                                        bool radians);

    /**
     * Reset all interrupt status bits and interrupt output.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bno055_reset_interrupt_status(const bno055_context dev);

    /**
     * Get the interrupt status register.  This is a bitmask of the
     * INT_STA_BITS_T bits.
     *
     * @param dev The device context.
     * @param istat A bitmask of INT_STA_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bno055_get_interrupt_status(const bno055_context dev,
                                             uint8_t *istat);

    /**
     * Get the interrupt enables register.  This is a bitmask of the
     * INT_STA_BITS_T bits.
     *
     * @param dev The device context.
     * @param ienable A bitmask of INT_STA_BITS_T bits currently set in the
     * enable register.
     * @return UPM result.
     */
    upm_result_t bno055_get_interrupt_enable(const bno055_context dev,
                                             uint8_t *ienable);

    /**
     * Set the interrupt enable register. This is composed of a
     * bitmask of the INT_STA_BITS_T bits.
     *
     * @param dev The device context.
     * @param enables a bitmask of INT_STA_BITS_T bits to enable
     * @return UPM result.
     */
    upm_result_t bno055_set_interrupt_enable(const bno055_context dev,
                                             uint8_t enables);

    /**
     * Get the interrupt mask register.  This is a bitmask of the
     * INT_STA_BITS_T bits.  The interrupt mask is used to mask off
     * enabled interrupts from generating a hardware interrupt.  The
     * interrupt status register can still be used to detect masked
     * interrupts if they are enabled.
     *
     * @param dev The device context.
     * @param imask a bitmask of INT_STA_BITS_T bits currently set in the
     * interrupt mask register.
     * @return UPM result.
     */
    upm_result_t bno055_get_interrupt_mask(const bno055_context dev,
                                           uint8_t *imask);

    /**
     * Set the interrupt mask register.  This is a bitmask of the
     * INT_STA_BITS_T bits.  The interrupt mask is used to mask off
     * enabled interrupts from generating a hardware interrupt.  The
     * interrupt status register can still be used to detect masked
     * interrupts if they are enabled.
     *
     * @param dev The device context.
     * @param mask A bitmask of INT_STA_BITS_T bits to set in the interrupt
     * mask register.
     * @return UPM result.
     */
    upm_result_t bno055_set_interrupt_mask(const bno055_context dev,
                                           uint8_t mask);

    /**
     * Get the value of the system status register.  This method
     * can be used to determine the overall status of the device.
     *
     * @param dev The device context.
     * @param sys_stat One of the SYS_STATUS_T values.
     * @return UPM result.
     */
    upm_result_t bno055_get_system_status(const bno055_context dev,
                                          BNO055_SYS_STATUS_T *sys_stat);

    /**
     * Return the value of the system error register.  This mathod can
     * be used to determine a variety of system related error
     * conditions.
     *
     * @param dev The device context.
     * @param sys_stat One of the SYS_ERR_T values.
     * @return UPM result.
     */
    upm_result_t bno055_get_system_error(const bno055_context dev,
                                         BNO055_SYS_ERR_T *sys_err);

    /**
     * install an interrupt handler.
     *
     * @param dev The device context.
     * @param gpio The GPIO pin to use as interrupt pin.
     * @param level The interrupt trigger level (one of mraa_gpio_edge_t
     * values).  Make sure that you have configured the interrupt pin
     * properly (on the device) for whatever level method you choose.
     * @param isr The interrupt handler, accepting a void * argument.
     * @param arg The argument to pass the the interrupt handler.
     * @return UPM result.
     */
    upm_result_t bno055_install_isr(const bno055_context dev,
                                    int gpio, mraa_gpio_edge_t level,
                                    void (*isr)(void *), void *arg);

    /**
     * uninstall a previously installed interrupt handler
     *
     * @param dev The device context.
     */
    void bno055_uninstall_isr(const bno055_context dev);

    /**
     * Set the current internal device register page.  This is a low
     * level function and should not be used unless you know what you
     * are doing.
     *
     * @param dev The device context.
     * @param page The page number to set.  This can only be 0 or 1.
     * @param force If true, force the device page state to match
     * indicated internal page state regardless of current state.
     * @return UPM result.
     */
    upm_result_t bno055_set_page(const bno055_context dev, uint8_t page,
                                 bool force);

    /**
     * Read a register.  This is a low level function and should not
     * be used unless you know what you are doing.
     *
     * @param dev The device context.
     * @param reg The register to read
     * @param retval The returned value of the register
     * @return UPM result.
     */
    upm_result_t bno055_read_reg(const bno055_context dev, uint8_t reg,
                                 uint8_t *retval);

    /**
     * Read contiguous registers into a buffer.  This is a low level
     * function and should not be used unless you know what you are
     * doing.
     *
     * @param dev The device context.
     * @param buffer The buffer to store the results
     * @param len The number of registers to read
     * @return UPM result.
     */
    upm_result_t bno055_read_regs(const bno055_context dev, uint8_t reg,
                                  uint8_t *buffer, size_t len);

    /**
     * Write to a register.  This is a low level function and should
     * not be used unless you know what you are doing.
     *
     * @param dev The device context.
     * @param reg The register to write to
     * @param val The value to write
     * @return UPM result.
     */
    upm_result_t bno055_write_reg(const bno055_context dev,
                                  uint8_t reg, uint8_t val);

    /**
     * Write data to contiguous registers.  This is a low level
     * function and should not be used unless you know what you are
     * doing.
     *
     * @param dev The device context.
     * @param reg The starting register to write to
     * @param buffer The buffer containing the data to write
     * @param len The number of bytes to write
     * @return UPM result.
     */
    upm_result_t bno055_write_regs(const bno055_context dev, uint8_t reg,
                                   uint8_t *buffer, size_t len);

#ifdef __cplusplus
}
#endif
