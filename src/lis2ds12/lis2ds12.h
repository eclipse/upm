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

#include "lis2ds12_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file lis2ds12.h
     * @library lis2ds12
     * @brief C API for the lis2ds12 driver
     *
     * @include lis2ds12.c
     */

    /**
     * Device context
     */
    typedef struct _lis2ds12_context {
        mraa_i2c_context i2c;
        mraa_spi_context spi;

        mraa_gpio_context gpioCS; // SPI CS pin
        mraa_gpio_context gpioINT1;  // intr 1
        mraa_gpio_context gpioINT2;  // intr 2

        // temperature
        float temperature;

        // uncompensated acc data
        float accX;
        float accY;
        float accZ;

        // acc scaling
        float accScale;
    } *lis2ds12_context;

    /**
     * LIS2DS12 initialization.
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
    lis2ds12_context lis2ds12_init(int bus, int addr, int cs);

    /**
     * LIS2DS12 Destructor
     *
     * @param dev The device context
     */
    void lis2ds12_close(lis2ds12_context dev);

    /**
     * Update the internal stored values from sensor data
     *
     * @param dev The device context
     * @return UPM result
     */
    upm_result_t lis2ds12_update(const lis2ds12_context dev);

    /**
     * Return the chip ID
     *
     * @param dev The device context
     * @return The chip ID (LIS2DS12_CHIPID)
     */
    uint8_t lis2ds12_get_chip_id(const lis2ds12_context dev);

    /**
     * Initialize the device and start operation.  This function is
     * called from lis2ds12_init(), so it will not need to be called by
     * a user unless the device is reset.
     *
     * @param dev The device context
     * @param odr One of the LIS2DS12_ODR_T values
     * @param fs One of the LIS2DS12_FS_T values
     * @return UPM result
     */
    upm_result_t lis2ds12_devinit(const lis2ds12_context dev,
                                  LIS2DS12_ODR_T odr,
                                  LIS2DS12_FS_T fs);

    /**
     * Set the output data rate (ODR) of the device
     *
     * @param dev The device context
     * @param odr One of the LIS2DS12_ODR_T values
     * @return UPM result
     */
    upm_result_t lis2ds12_set_odr(const lis2ds12_context dev,
                                  LIS2DS12_ODR_T odr);

    /**
     * Set the full scale (FS) of the device.  This device supports a
     * full scale of 2, 4, 8, and 16G.
     *
     * @param dev The device context
     * @param fs One of the LIS2DS12_FS_T values
     * @return UPM result
     */
    upm_result_t lis2ds12_set_full_scale(const lis2ds12_context dev,
                                         LIS2DS12_FS_T fs);

    /**
     * Return accelerometer data gravities (g).  lis2ds12_update()
     * must have been called prior to calling this method.
     *
     * @param dev The device context
     * @param x Pointer to a floating point value that will have the
     * current x component placed into it
     * @param y Pointer to a floating point value that will have the
     * current y component placed into it
     * @param z Pointer to a floating point value that will have the
     * current z component placed into it
     */
    void lis2ds12_get_accelerometer(const lis2ds12_context dev,
                                    float *x, float *y, float *z);

    /**
     * Return the current measured temperature.  Note, this is not
     * ambient temperature.  lis2ds12_update() must have been called
     * prior to calling this method.
     *
     * @param dev The device context
     * @return The temperature in degrees Celsius
     */
    float lis2ds12_get_temperature(const lis2ds12_context dev);

    /**
     * Reset the device as if during a power on reset.  All configured
     * values are lost when this happens.  You should call
     * lis2ds12_devinit() afterwards, or at least perform the same
     * initialization lis2ds12_devinit() does before continuing.
     *
     * @param dev The device context
     * @return UPM result
     */
    upm_result_t lis2ds12_reset(const lis2ds12_context dev);

    /**
     * Enable high pass filtering of the accelerometer axis data.
     * lis2ds12_devinit() disables this by default.  See the datasheet
     * for details.
     *
     * @param dev The device context
     * @param filter true to enable filtering, false to disable
     * @return UPM result
     */
    upm_result_t lis2ds12_enable_hp_filtering(const lis2ds12_context dev,
                                              bool filter);

    /**
     * Enable or disable interrupt latching.  If latching is disabled,
     * pulsed is enabled.  See the datasheet for details.
     *
     * @param dev The device context
     * @param latch true to enable latching, false to disable
     * @return UPM result
     */
    upm_result_t lis2ds12_enable_interrupt_latching(const lis2ds12_context dev,
                                                    bool latch);

    /**
     * Indicate whether the interrupt should be active high (default)
     * or active low.  See the datasheet for details.
     *
     * @param dev The device context
     * @param high true for active high, false for active low
     * @return UPM result
     */
    upm_result_t lis2ds12_set_interrupt_active_high(const lis2ds12_context dev,
                                                    bool high);

    /**
     * Indicate whether interrupts are push-pull (default) or open
     * drain.  See the datasheet for details.
     *
     * @param dev The device context
     * @param pp true for push-pull, false for open-drain
     * @return UPM result
     */
    upm_result_t lis2ds12_set_interrupt_push_pull(const lis2ds12_context dev,
                                                  bool pp);

    /**
     * Set interrupt 1 configuration.  See the datasheet for details.
     *
     * @param dev The device context
     * @param cfg A bitmask of values from LIS2DS12_CTRL4_BITS_T
     * @return UPM result
     */
    upm_result_t lis2ds12_set_int1_config(const lis2ds12_context dev,
                                          uint8_t cfg);

    /**
     * Set interrupt 2 configuration.  See the datasheet for details.
     *
     * @param dev The device context
     * @param cfg A bitmask of values from LIS2DS12_CTRL5_BITS_T
     * @return UPM result
     */
    upm_result_t lis2ds12_set_int2_config(const lis2ds12_context dev,
                                          uint8_t cfg);

    /**
     * Return the contents of the status register
     *
     * @param dev The device context
     * @return A bitmask of values from LIS2DS12_STATUS_BITS_T
     */
    uint8_t lis2ds12_get_status(const lis2ds12_context dev);

    /**
     * Install an interrupt handler
     *
     * @param dev The device context
     * @param intr One of the LIS2DS12_INTERRUPT_PINS_T values
     * specifying which interrupt pin you are installing
     * @param gpio GPIO pin to use as interrupt pin
     * @param level The interrupt trigger level (one of mraa_gpio_edge_t
     * values).  Make sure that you have configured the interrupt pin
     * properly for whatever level you choose.
     * @param isr The interrupt handler, accepting a void * argument
     * @param arg The argument to pass the the interrupt handler
     * @return UPM result
     */
    upm_result_t lis2ds12_install_isr(const lis2ds12_context dev,
                                      LIS2DS12_INTERRUPT_PINS_T intr, int gpio,
                                      mraa_gpio_edge_t level,
                                      void (*isr)(void *), void *arg);

    /**
     * Uninstall a previously installed interrupt handler
     *
     * @param dev The device context
     * @param intr One of the LIS2DS12_INTERRUPT_PINS_T values
     * specifying which interrupt pin you are removing
     */
    void lis2ds12_uninstall_isr(const lis2ds12_context dev,
                                LIS2DS12_INTERRUPT_PINS_T intr);

    /**
     * Read a register
     *
     * @param dev The device context
     * @param reg The register to read
     * @return The value of the register
     */
    uint8_t lis2ds12_read_reg(const lis2ds12_context dev, uint8_t reg);

    /**
     * Read contiguous registers into a buffer
     *
     * @param dev The device context
     * @param reg The register to start the read from
     * @param buffer The buffer to store the results
     * @param len The number of registers to read
     * @return The number of bytes read, or -1 on error
     */
    int lis2ds12_read_regs(const lis2ds12_context dev, uint8_t reg,
                           uint8_t *buffer, int len);

    /**
     * Write to a register
     *
     * @param dev The device context
     * @param reg The register to write to
     * @param val The value to write
     * @return UPM result
     */
    upm_result_t lis2ds12_write_reg(const lis2ds12_context dev,
                                    uint8_t reg, uint8_t val);


#ifdef __cplusplus
}
#endif
