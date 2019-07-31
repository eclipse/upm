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
#include <mraa/spi.h>
#include <mraa/gpio.h>

#include "upm.h"

#include "lsm6dsl_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file lsm6dsl.h
     * @library lsm6dsl
     * @brief C API for the lsm6dsl driver
     *
     * @include lsm6dsl.c
     */

    /**
     * Device context
     */
    typedef struct _lsm6dsl_context {
        mraa_i2c_context i2c;
        mraa_spi_context spi;

        mraa_gpio_context gpioCS; // SPI CS pin
        mraa_gpio_context gpioINT1; // intr 1
        mraa_gpio_context gpioINT2; // intr 2

        // uncompensated temperature
        float temperature;

        // uncompensated acc data
        float accX;
        float accY;
        float accZ;

        // uncompensated gyr data
        float gyrX;
        float gyrY;
        float gyrZ;

        // acc scaling
        float accScale;
    } *lsm6dsl_context;

    /**
     * LSM6DSL initialization.
     *
     * This device can support both I2C and SPI. For SPI, set the addr
     * to -1, and specify a positive integer representing the Chip
     * Select (CS) pin for the cs argument.  If you are using a
     * hardware CS pin (like Intel Edison with Arduino breakout), then
     * you can connect the proper pin to the hardware CS pin on your
     * MCU and supply -1 for cs.
     *
     * @param bus I2C or SPI bus to use
     * @param addr The address for this device, or -1 for SPI
     * @param cs The gpio pin to use for the SPI Chip Select.  Use -1 for
     * I2C or for SPI with a hardware controlled pin.
     * @return The device context, or NULL on error
     */
    lsm6dsl_context lsm6dsl_init(int bus, int addr, int cs);

    /**
     * LSM6DSL Destructor
     *
     * @param dev The device context
     */
    void lsm6dsl_close(lsm6dsl_context dev);

    /**
     * Update the internal stored values from sensor data
     *
     * @param dev The device context
     * @return UPM result
     */
    upm_result_t lsm6dsl_update(const lsm6dsl_context dev);

    /**
     * Return the chip ID
     *
     * @param dev The device context
     * @return The chip ID (LSM6DSL_CHIPID)
     */
    uint8_t lsm6dsl_get_chip_id(const lsm6dsl_context dev);

    /**
     * Initialize the device and start operation.  This function is
     * called from lsm6dsl_init(), so it will not need to be called
     * by a user unless the device is reset.  It sets the
     * accelerometer and gyroscope ODR and FS modes, and enables BDU,
     * register auto-increment, and high performance mode.
     *
     * @param dev The device context
     * @param acc_odr One of the LSM6DSL_XL_ODR_T values
     * @param acc_fs One of the LSM6DSL_XL_FS_T values
     * @param gyr_odr One of the LSM6DSL_G_ODR_T values
     * @param gyr_fs One of the LSM6DSL_G_FS_T values
     * @return UPM result
     */
    upm_result_t lsm6dsl_devinit(const lsm6dsl_context dev,
                                 LSM6DSL_XL_ODR_T acc_odr,
                                 LSM6DSL_XL_FS_T acc_fs,
                                 LSM6DSL_G_ODR_T gyr_odr,
                                 LSM6DSL_G_FS_T gyr_fs);

    /**
     * Set the output data rate (ODR) of the accelerometer
     *
     * @param dev The device context
     * @param odr One of the LSM6DSL_XL_ODR_T values
     * @return UPM result
     */
    upm_result_t lsm6dsl_set_acc_odr(const lsm6dsl_context dev,
                                     LSM6DSL_XL_ODR_T odr);

    /**
     * Set the output data rate (ODR) of the gyroscope
     *
     * @param dev The device context
     * @param odr One of the LSM6DSL_G_ODR_T values
     * @return UPM result
     */
    upm_result_t lsm6dsl_set_gyr_odr(const lsm6dsl_context dev,
                                     LSM6DSL_G_ODR_T odr);

    /**
     * Set the full scale (FS) of the accelerometer.  This device
     * supports a full scale of 2, 4, 8, and 16G.
     *
     * @param dev The device context
     * @param fs One of the LSM6DSL_XL_FS_T values
     * @return UPM result
     */
    upm_result_t lsm6dsl_set_acc_full_scale(const lsm6dsl_context dev,
                                            LSM6DSL_XL_FS_T fs);

    /**
     * Set the full scale (FS) of the gyroscope.  This device supports
     * a full scale of 125, 245, 500, 1000, and 2000 degrees per
     * second (DPS)
     *
     * @param dev The device context
     * @param fs One of the LSM6DSL_G_FS_T values
     * @return UPM result
     */
    upm_result_t lsm6dsl_set_gyr_full_scale(const lsm6dsl_context dev,
                                            LSM6DSL_G_FS_T fs);

    /**
     * Return accelerometer data in gravities (g).  lsm6dsl_update()
     * must have been called prior to calling this function.
     *
     * @param dev The device context
     * @param x Pointer to a floating point value that will have the
     * current x component placed into it
     * @param y Pointer to a floating point value that will have the
     * current y component placed into it
     * @param z Pointer to a floating point value that will have the
     * current z component placed into it
     */
    void lsm6dsl_get_accelerometer(const lsm6dsl_context dev,
                                   float *x, float *y, float *z);

    /**
     * Return gyroscope data in degrees per second (DPS).
     * lsm6dsl_update() must have been called prior to calling this
     * function.
     *
     * @param dev The device context
     * @param x Pointer to a floating point value that will have the
     * current x component placed into it
     * @param y Pointer to a floating point value that will have the
     * current y component placed into it
     * @param z Pointer to a floating point value that will have the
     * current z component placed into it
     */
    void lsm6dsl_get_gyroscope(const lsm6dsl_context dev,
                               float *x, float *y, float *z);

    /**
     * Return the current measured temperature.  Note, this is not
     * ambient temperature.  lsm6dsl_update() must have been called
     * prior to calling this function.
     *
     * @param dev The device context
     * @return The temperature in degrees Celsius
     */
    float lsm6dsl_get_temperature(const lsm6dsl_context dev);

    /**
     * Enable accelerometer and gyroscope high performance modes.
     * These are further defined by the respective ODR settings to
     * allow low power, and normal/high-performance modes.
     *
     * @param dev The device context
     * @param enable true to enable high performance mode, false otherwise
     * @return UPM result
     */
    upm_result_t lsm6dsl_high_performance(const lsm6dsl_context dev,
                                          bool enable);

    /**
     * Reset the device as if during a power on reset.  All configured
     * values are lost when this happens.  You should call
     * lsm6dsl_devinit() afterwards, or at least perform the same
     * initialization lsm6dsl_devinit() does before continuing.
     *
     * @param dev The device context
     * @return UPM result
     */
    upm_result_t lsm6dsl_reset(const lsm6dsl_context dev);

    /**
     * Indicate whether the interrupt should be active high (default)
     * or active low.  See the datasheet for details.
     *
     * @param dev The device context
     * @param high true for active high, false for active low
     * @return UPM result
     */
    upm_result_t lsm6dsl_set_interrupt_active_high(const lsm6dsl_context dev,
                                                   bool high);

    /**
     * Indicate whether interrupts are push-pull (default) or open
     * drain.  See the datasheet for details.
     *
     * @param dev The device context
     * @param pp true for push-pull, false for open-drain
     * @return UPM result
     */
    upm_result_t lsm6dsl_set_interrupt_push_pull(const lsm6dsl_context dev,
                                                 bool pp);

    /**
     * Return the contents of the status register
     *
     * @param dev The device context
     * @return A bitmask of values from LSM6DSL_STATUS_BITS_T
     */
    uint8_t lsm6dsl_get_status(const lsm6dsl_context dev);

    /**
     * Set accelerometer offsets for each axis.  In the case of X and
     * Y, the offsets will be internally added before being placed
     * into the output registers.  For the Z offset, the value will be
     * subtracted before being placed into the output registers.  The
     * weight is used to determine the weighing of the offset bits.
     * All offsets must be in the range of -127 to 127.
     *
     * @param dev The device context
     * @param x X axis offset in the range -127 to 127
     * @param y Y axis offset in the range -127 to 127
     * @param z Z axis offset in the range -127 to 127
     * @param weight When true, the the offset will be weighted at
     * 2^-6 g/LSB, when false the weighting will be 2^-10 g/LSB
     * @return UPM result
     */
    upm_result_t lsm6dsl_set_acc_offsets(const lsm6dsl_context dev,
                                         int x, int y, int z, bool weight);


    /**
     * Install an interrupt handler
     *
     * @param dev The device context
     * @param intr One of the LSM6DSL_INTERRUPT_PINS_T values
     * specifying which interrupt pin you are installing
     * @param gpio GPIO pin to use as interrupt pin
     * @param level The interrupt trigger level (one of mraa_gpio_edge_t
     * values).  Make sure that you have configured the interrupt pin
     * properly for whatever level you choose.
     * @param isr The interrupt handler, accepting a void * argument
     * @param arg The argument to pass the the interrupt handler
     * @return UPM result
     */
    upm_result_t lsm6dsl_install_isr(const lsm6dsl_context dev,
                                     LSM6DSL_INTERRUPT_PINS_T intr, int gpio,
                                     mraa_gpio_edge_t level,
                                     void (*isr)(void *), void *arg);

    /**
     * Uninstall a previously installed interrupt handler
     *
     * @param dev The device context
     * @param intr One of the LSM6DSL_INTERRUPT_PINS_T values
     * specifying which interrupt pin you are removing
     */
    void lsm6dsl_uninstall_isr(const lsm6dsl_context dev,
                               LSM6DSL_INTERRUPT_PINS_T intr);

    /**
     * Read a register
     *
     * @param dev The device context
     * @param reg The register to read
     * @return The value of the register
     */
    uint8_t lsm6dsl_read_reg(const lsm6dsl_context dev, uint8_t reg);

    /**
     * Read contiguous registers into a buffer
     *
     * @param dev The device context
     * @param buffer The buffer to store the results
     * @param len The number of registers to read
     * @return The number of bytes read, or -1 on error
     */
    int lsm6dsl_read_regs(const lsm6dsl_context dev, uint8_t reg,
                          uint8_t *buffer, int len);

    /**
     * Write to a register
     *
     * @param dev The device context
     * @param reg The register to write to
     * @param val The value to write
     * @return UPM result
     */
    upm_result_t lsm6dsl_write_reg(const lsm6dsl_context dev,
                                   uint8_t reg, uint8_t val);


#ifdef __cplusplus
}
#endif
