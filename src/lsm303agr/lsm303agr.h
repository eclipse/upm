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
#pragma once

#include <unistd.h>
#include <string.h>

#include <mraa/i2c.h>
#include <mraa/gpio.h>

#include "upm.h"

#include "lsm303agr_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file lsm303agr.h
     * @library lsm303agr
     * @brief C API for the lsm303agr driver
     *
     * @include lsm303agr.c
     */

    /**
     * Device context
     */
    typedef struct _lsm303agr_context {
        mraa_i2c_context i2cACC; // accelerometer
        mraa_i2c_context i2cMAG; // magnetometer
        mraa_gpio_context gpioACC1; // acc intr
        mraa_gpio_context gpioACC2; // acc intr
        mraa_gpio_context gpioMAG;  // mag intr

        // currently set power mode
        LSM303AGR_POWER_MODE_T powerMode;

        // uncompensated temperature in C
        float temperature;

        // accelerometer scaling, depends on full scale sensitivity and
        // power mode
        float accScale;
        float accDivisor;

        // uncompensated acc data
        float accX;
        float accY;
        float accZ;

        // uncompensated mag data
        float magX;
        float magY;
        float magZ;
    } *lsm303agr_context;


    /**
     * LSM303AGR initialization
     *
     * This driver can only support I2C. SPI requires support for
     * 3-wire SPI which we cannot currently handle.  Only the basic
     * capabilities of the device are supported, however there is a
     * full register map defined (lsm303agr_defs.h), and with access
     * to the bus read/write functions, any desired additional
     * functionality can be implemented.
     *
     * @param bus I2C bus to use
     * @param acc_addr The I2C address of the accelerometer.  Use -1
     * if you do not wish to use the accelerometer.
     * @param mag_addr The I2C address of the magnetometer.  Use -1
     * if you do not wish to use the magnetometer.
     * @return The device context, or NULL if an error occurred
     */
    lsm303agr_context lsm303agr_init(int bus, int acc_addr, int mag_addr);

    /**
     * LSM303AGR Destructor
     *
     * @param dev The device context
     */
    void lsm303agr_close(lsm303agr_context dev);

    /**
     * Update the internal stored values from sensor data
     *
     * @param dev The device context
     * @return UPM result
     */
    upm_result_t lsm303agr_update(const lsm303agr_context dev);

    /**
     * Return magnetometer data in micro-Teslas (uT).  update() must
     * have been called prior to calling this function.
     *
     * @param dev The device context
     * @param x Pointer to a floating point value that will have the
     * current x component placed into it
     * @param y Pointer to a floating point value that will have the
     * current y component placed into it
     * @param z Pointer to a floating point value that will have the
     * current z component placed into it
     */
    void lsm303agr_get_magnetometer(const lsm303agr_context dev,
                                    float *x, float *y, float *z);

    /**
     * Return accelerometer data in gravities.  update() must have
     * been called prior to calling this function.
     *
     * @param dev The device context
     * @param x Pointer to a floating point value that will have the
     * current x component placed into it
     * @param y Pointer to a floating point value that will have the
     * current y component placed into it
     * @param z Pointer to a floating point value that will have the
     * current z component placed into it
     */
    void lsm303agr_get_accelerometer(const lsm303agr_context dev,
                                     float *x, float *y, float *z);

    /**
     * Return temperature data in degrees Celsius..  update() must
     * have been called prior to calling this function.
     *
     * @param dev The device context
     * @return Temperature in degrees Celsius
     */
    float lsm303agr_get_temperature(const lsm303agr_context dev);

    /**
     * Set the full scale (sensitivity) value for the accelerometer.
     * This device supports 2G, 4G, 8G, and 16G full scale modes.
     *
     * @param dev The device context
     * @param fs One of the LSM303AGR_A_FS_T values
     * @return UPM result
     */
    upm_result_t lsm303agr_set_full_scale(const lsm303agr_context dev,
                                          LSM303AGR_A_FS_T fs);

    /**
     * Initialize the device and start operation.  This function is
     * called from the constructor so it will not typically need to be
     * called by a user unless the device is reset.  It will
     * initialize the accelerometer and magnetometer (if enabled) to
     * certain default running modes.
     *
     * For the accelerometer, the full scale will be set to 2G, mode
     * continuous, all axes enabled, BDU enabled, temperature
     * measurement enabled, and an output data rate (ODR) of 100Hz
     * with the power mode set at high resolution.
     *
     * For the magnetometer, temperature compensation will be enabled,
     * mode continuous, and an output data rate of 10Hz with the power
     * mode set at high resolution.
     *
     * @param dev The device context
     * @param mode One of the LSM303AGR_POWER_MODE_T values.  This
     * parameter is passed to a call to lsm303agr_set_power_mode() and
     * will set the appropriate mode for both the accelerometer and
     * magnetometer.  The default set by the constructor is
     * LSM303AGR_POWER_HIGH_RESOLUTION.
     * @return UPM result
     */
    upm_result_t lsm303agr_devinit(const lsm303agr_context dev,
                                   LSM303AGR_POWER_MODE_T mode);

    /**
     * Set an operating power mode.  There are 3 modes available: low
     * power, normal, and high resolution.
     *
     * @param dev The device context
     * @param mode One of the LSM303AGR_POWER_MODE_T values.  The
     * default set at initialization time is
     * LSM303AGR_POWER_HIGH_RESOLUTION.
     * @return UPM result
     */
    upm_result_t lsm303agr_set_power_mode(const lsm303agr_context dev,
                                          LSM303AGR_POWER_MODE_T mode);

    /**
     * Set the accelerometer (acc) output data rate (odr)
     *
     * @param dev The device context
     * @param odr One of the LSM303AGR_A_ODR_T values.  The default
     * set at initialization time is LSM303AGR_A_ODR_100HZ.
     * @return UPM result
     */
    upm_result_t lsm303agr_set_acc_odr(const lsm303agr_context dev,
                                       LSM303AGR_A_ODR_T odr);

    /**
     * Set the magnetometer (mag) output data rate (odr)
     *
     * @param dev The device context
     * @param odr One of the LSM303AGR_CFG_A_M_ODR_T values.  The default
     * set at initialization time is LSM303AGR_CFG_A_M_ODR_10HZ.
     * @return UPM result
     */
    upm_result_t lsm303agr_set_mag_odr(const lsm303agr_context dev,
                                       LSM303AGR_CFG_A_M_ODR_T odr);

    /**
     * Return the accelerometer (acc) interrupt 1 config register.
     * This register allows you to enable various interrupt
     * conditions.  See the datasheet for details.
     *
     * @param dev The device context
     * @return A bitmask of LSM303AGR_INT_CFG_A_BITS_T bits
     */
    uint8_t lsm303agr_get_acc_int1_config(const lsm303agr_context dev);

    /**
     * Return the accelerometer (acc) interrupt 2 config register.
     * This register allows you to enable various interrupt
     * conditions.  See the datasheet for details.
     *
     * @param dev The device context
     * @return A bitmask of LSM303AGR_INT_CFG_A_BITS_T bits
     */
    uint8_t lsm303agr_get_acc_int2_config(const lsm303agr_context dev);

    /**
     * Set the accelerometer (acc) interrupt 1 config register.  See
     * the datasheet for details.
     *
     * @param dev The device context
     * @param bits A bitmask of LSM303AGR_INT_CFG_A_BITS_T bits
     * @return UPM result
     */
    upm_result_t lsm303agr_set_acc_int1_config(const lsm303agr_context dev,
                                               uint8_t bits);

    /**
     * Set the accelerometer (acc) interrupt 2 config register.  See
     * the datasheet for details.
     *
     * @param dev The device context
     * @param bits A bitmask of LSM303AGR_INT_CFG_A_BITS_T bits
     * @return UPM result
     */
    upm_result_t lsm303agr_set_acc_int2_config(const lsm303agr_context dev,
                                               uint8_t bits);

    /**
     * Return the magnetometer (mag) interrupt config register.  See
     * the datasheet for details.
     *
     * @param dev The device context
     * @return A bitmask of LSM303AGR_INT_CTRL_REG_M_BITS_T bits
     */
    uint8_t lsm303agr_get_mag_int_config(const lsm303agr_context dev);

    /**
     * Set the magnetometer (mag) interrupt config register.  See
     * the datasheet for details.
     *
     * @param dev The device context
     * @param bits A bitmask of LSM303AGR_INT_CTRL_REG_M_BITS_T bits
     */
    upm_result_t lsm303agr_set_mag_int_config(const lsm303agr_context dev,
                                              uint8_t bits);

    /**
     * Return the accelerometer (acc) interrupt 1 source register.
     * This register indicates which interrupts have been triggered.
     * See the datasheet for details.
     *
     * @param dev The device context
     * @return a bitmask of LSM303AGR_INT_SRC_A_BITS_T bits
     */
    uint8_t lsm303agr_get_acc_int1_src(const lsm303agr_context dev);

    /**
     * Return the accelerometer (acc) interrupt 2 source register.
     * This register indicates which interrupts have been triggered.
     * See the datasheet for details.
     *
     * @param dev The device context
     * @return a bitmask of LSM303AGR_INT_SRC_A_BITS_T bits
     */
    uint8_t lsm303agr_get_acc_int2_src(const lsm303agr_context dev);

    /**
     * Return the magnetometer (mag) interrupt source register.
     * This register indicates which interrupts have been triggered.
     * See the datasheet for details.
     *
     * @param dev The device context
     * @return a bitmask of LSM303AGR_INT_SRC_REG_M_BITS_T bits
     */
    uint8_t lsm303agr_get_mag_int_src(const lsm303agr_context dev);

    /**
     * Install an interrupt handler
     *
     * @param dev The device context
     * @param intr One of the LSM303AGR_INTERRUPT_PINS_T values
     * specifying which interrupt pin you are installing
     * @param gpio GPIO pin to use as interrupt pin
     * @param level The interrupt trigger level (one of the
     * mraa_gpio_edge_t values).  Make sure that you have configured
     * the interrupt pin properly for whatever level you choose.
     * @param isr The interrupt handler, accepting a void * argument
     * @param arg The argument to pass the the interrupt handler
     * @return UPM result
     */
    upm_result_t lsm303agr_install_isr(const lsm303agr_context dev,
                                       LSM303AGR_INTERRUPT_PINS_T intr,
                                       int gpio, mraa_gpio_edge_t level,
                                       void (*isr)(void *), void *arg);

    /**
     * Uninstall a previously installed interrupt handler
     *
     * @param dev The device context
     * @param intr One of the LSM303AGR_INTERRUPT_PINS_T values
     * specifying which interrupt pin you are removing
     */
    void lsm303agr_uninstall_isr(const lsm303agr_context dev,
                                 LSM303AGR_INTERRUPT_PINS_T intr);

    /**
     * Read a register.
     *
     * @param dev The device context
     * @param reg The register to read
     * @return The value of the register
     */
    uint8_t lsm303agr_read_reg(const lsm303agr_context dev, uint8_t reg);

    /**
     * Read contiguous registers into a buffer
     *
     * @param dev The device context
     * @param buffer The buffer to store the results
     * @param len The number of registers to read
     * @return The number of bytes read, or -1 on error
     */
    int lsm303agr_read_regs(const lsm303agr_context dev, uint8_t reg,
                            uint8_t *buffer, int len);

    /**
     * Write to a register
     *
     * @param dev The device context
     * @param reg The register to write to
     * @param val The value to write
     * @return UPM result
     */
    upm_result_t lsm303agr_write_reg(const lsm303agr_context dev,
                                     uint8_t reg, uint8_t val);

#ifdef __cplusplus
}
#endif
