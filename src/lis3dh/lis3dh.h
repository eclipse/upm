/*
 * Author: Alex Tereschenko <alext.mkrs@gmail.com>
 * Copyright (c) 2018 Alex Tereschenko.
 *
 * Based on LIS2DS12 module by
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

#include <string.h>
#include <unistd.h>

#include <mraa/gpio.h>
#include <mraa/i2c.h>
#include <mraa/spi.h>

#include "upm.h"

#include "lis3dh_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file lis3dh.h
 * @library lis3dh
 * @brief C API for the lis3dh driver
 *
 * @include lis3dh.c
 */

/**
 * Device context
 */
typedef struct _lis3dh_context {
    mraa_i2c_context i2c;
    mraa_spi_context spi;

    mraa_gpio_context gpioCS;   // SPI CS pin
    mraa_gpio_context gpioINT1; // Interrupt 1
    mraa_gpio_context gpioINT2; // Interrupt 2

    // Uncompensated temperature data
    float temperature;

    // Temperature scaling factor. It accounts for storing 8b or 10b actual
    // data in 16b register pair.
    float temperatureFactor;

    // Uncompensated acceleration data
    float accX;
    float accY;
    float accZ;

    // Acceleration scaling - used to calculate actual acceleration,
    // depending on sensor working mode (low power/normal/high resolution)
    float accScale;
} * lis3dh_context;

/**
 * LIS3DH initialization.
 *
 * This device can support both I2C and SPI. For SPI, set the addr
 * to -1, and specify a positive integer representing the Chip
 * Select (CS) pin for the cs argument. If you are using a
 * hardware CS pin (like Intel Edison with Arduino breakout), then
 * you can connect the proper pin to the hardware CS pin on your
 * MCU and supply -1 for cs.
 *
 * @param bus I2C or SPI bus to use
 * @param addr The address for this device, or -1 for SPI
 * @param cs The gpio pin to use for the SPI Chip Select. Use -1 for
 * I2C or for SPI with a hardware controlled pin.
 * @return The device context, or NULL on error
 */
lis3dh_context lis3dh_init(int bus, int addr, int cs);

/**
 * LIS3DH Destructor
 *
 * @param dev The device context
 */
void lis3dh_close(lis3dh_context dev);

/**
 * Initialize the device and start operation. This function is
 * called from lis3dh_init(), so it will not need to be called by
 * a user unless the device is reset.
 *
 * @param dev The device context
 * @param odr One of the LIS3DH_ODR_T values
 * @param fs One of the LIS3DH_FS_T values
 * @param high_res true to enable high resolution mode, false to disable
 * @return UPM result
 */
upm_result_t
lis3dh_devinit(const lis3dh_context dev, LIS3DH_ODR_T odr, LIS3DH_FS_T fs, bool high_res);

/**
 * Enable or disable specific axes.
 *
 * @param dev The device context
 * @param x_axis_enable true to enable X axis, false to disable
 * @param y_axis_enable true to enable Y axis, false to disable
 * @param z_axis_enable true to enable Z axis, false to disable
 * @return UPM result
 */
upm_result_t lis3dh_enable_axes(const lis3dh_context dev,
                                bool x_axis_enable,
                                bool y_axis_enable,
                                bool z_axis_enable);

/**
 * Enable or disable Block Data Update (BDU) mode.
 *
 * @param dev The device context
 * @param bdu_enable true to enable BDU mode, false to disable
 * @return UPM result
 */
upm_result_t lis3dh_enable_bdu_mode(const lis3dh_context dev, bool bdu_enable);

/**
 * Enable or disable Low Power (LP) mode. Checks if mutually exclusive
 * High Resolution (HR) mode is enabled and bails out if yes.
 *
 * @param dev The device context
 * @param lp_enable true to enable LP mode, false to disable
 * @return UPM result
 */
upm_result_t lis3dh_enable_lp_mode(const lis3dh_context dev, bool lp_enable);

/**
 * Enable or disable High Resolution (HR) mode. Checks if mutually exclusive
 * Low Power (LP) mode is enabled and bails out if yes.
 *
 * @param dev The device context
 * @param hr_enable true to enable HR mode, false to disable
 * @return UPM result
 */
upm_result_t lis3dh_enable_hr_mode(const lis3dh_context dev, bool hr_enable);

/**
 * Enable Normal mode by explicitly disabling LP and HR ones.
 * Note that there's no "disable" part as it's generally unknown, which mode
 * we were in previously. To get out of Normal mode, just enable HR or LP one.
 *
 * @param dev The device context
 * @return UPM result
 */
upm_result_t lis3dh_enable_normal_mode(const lis3dh_context dev);

/**
 * Enable high pass filtering of the accelerometer axis data.
 * lis3dh_devinit() disables this by default. See the datasheet
 * for details.
 *
 * @param dev The device context
 * @param filter true to enable filtering, false to disable
 * @return UPM result
 */
upm_result_t lis3dh_enable_hp_filtering(const lis3dh_context dev, bool filter);

/**
 * Enable or disable built-in Analog-to-Digital Converter (ADC).
 *
 * @param dev The device context
 * @param adc_enable true to enable ADC, false to disable
 * @return UPM result
 */
upm_result_t lis3dh_enable_adc(const lis3dh_context dev, bool adc_enable);

/**
 * Enable or disable built-in temperature sensor. It depends on ADC being
 * enabled, so we enable it unconditionally. See datasheet for details.
 *
 * @param dev The device context
 * @param temperature_enable true to enable temp sensor, false to disable
 * @return UPM result
 */
upm_result_t lis3dh_enable_temperature(const lis3dh_context dev, bool temperature_enable);

/**
 * Enable or disable interrupt latching for INT1 and INT2.
 * See the datasheet for details.
 *
 * @param dev The device context
 * @param int1_latch true to enable latching for INT1, false to disable
 * @param int2_latch true to enable latching for INT2, false to disable
 * @return UPM result
 */
upm_result_t
lis3dh_enable_interrupt_latching(const lis3dh_context dev, bool int1_latch, bool int2_latch);

/**
 * Set the output data rate (ODR) of the device
 *
 * @param dev The device context
 * @param odr One of the LIS3DH_ODR_T values
 * @return UPM result
 */
upm_result_t lis3dh_set_odr(const lis3dh_context dev, LIS3DH_ODR_T odr);

/**
 * Set the full scale (FS) of the device. This device supports a
 * full scale of 2, 4, 8, and 16G.
 *
 * @param dev The device context
 * @param fs One of the LIS3DH_FS_T values
 * @return UPM result
 */
upm_result_t lis3dh_set_full_scale(const lis3dh_context dev, LIS3DH_FS_T fs);

/**
 * Indicate whether INT1 and INT2 interrupts should be active high (default)
 * or active low. See the datasheet for details.
 *
 * @param dev The device context
 * @param high true for active high, false for active low
 * @return UPM result
 */
upm_result_t lis3dh_set_interrupt_active_high(const lis3dh_context dev, bool high);

/**
 * Set interrupt 1 configuration. See the datasheet for details.
 *
 * @param dev The device context
 * @param cfg A bitmask of values from LIS3DH_CTRL_REG3_BITS_T
 * @return UPM result
 */
upm_result_t lis3dh_set_int1_config(const lis3dh_context dev, uint8_t cfg);

/**
 * Set interrupt 2 configuration. See the datasheet for details.
 *
 * @param dev The device context
 * @param cfg A bitmask of values from LIS3DH_CTRL_REG6_BITS_T
 * @return UPM result
 */
upm_result_t lis3dh_set_int2_config(const lis3dh_context dev, uint8_t cfg);

/**
 * Read a register
 *
 * @param dev The device context
 * @param reg The register to read
 * @return The value of the register
 */
uint8_t lis3dh_read_reg(const lis3dh_context dev, uint8_t reg);

/**
 * Read contiguous registers into a buffer
 *
 * @param dev The device context
 * @param reg The register to start the read from
 * @param buffer The buffer to store the results
 * @param len The number of registers to read
 * @return The number of bytes read, or -1 on error
 */
int lis3dh_read_regs(const lis3dh_context dev, uint8_t reg, uint8_t* buffer, int len);

/**
 * Write to a register
 *
 * @param dev The device context
 * @param reg The register to write to
 * @param val The value to write
 * @return UPM result
 */
upm_result_t lis3dh_write_reg(const lis3dh_context dev, uint8_t reg, uint8_t val);

/**
 * Update the internal stored values from sensor data
 *
 * @param dev The device context
 * @return UPM result
 */
upm_result_t lis3dh_update(const lis3dh_context dev);

/**
 * Return the chip ID
 *
 * @param dev The device context
 * @return The chip ID (LIS3DH_CHIPID)
 */
uint8_t lis3dh_get_chip_id(const lis3dh_context dev);

/**
 * Return accelerometer data gravities (g). lis3dh_update()
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
void lis3dh_get_accelerometer(const lis3dh_context dev, float* x, float* y, float* z);

/**
 * Return the current measured temperature. Note, this is not
 * ambient temperature. lis3dh_update() must have been called
 * prior to calling this method.
 *
 * @param dev The device context
 * @return The temperature in degrees Celsius
 */
float lis3dh_get_temperature(const lis3dh_context dev);

/**
 * Return the contents of the status register
 *
 * @param dev The device context
 * @return A bitmask of values from LIS3DH_STATUS_REG_BITS_T
 */
uint8_t lis3dh_get_status(const lis3dh_context dev);

/**
 * Return the contents of the status aux register
 *
 * @param dev The device context
 * @return A bitmask of values from LIS3DH_STATUS_REG_AUX_BITS_T
 */
uint8_t lis3dh_get_status_aux(const lis3dh_context dev);

/**
 * Install an interrupt handler
 *
 * @param dev The device context
 * @param intr One of the LIS3DH_INTERRUPT_PINS_T values
 * specifying which interrupt pin you are installing
 * @param gpio GPIO pin to use as interrupt pin
 * @param level The interrupt trigger level (one of mraa_gpio_edge_t
 * values). Make sure that you have configured the interrupt pin
 * properly for whatever level you choose.
 * @param isr The interrupt handler, accepting a void * argument
 * @param arg The argument to pass the the interrupt handler
 * @return UPM result
 */
upm_result_t lis3dh_install_isr(const lis3dh_context dev,
                                LIS3DH_INTERRUPT_PINS_T intr,
                                int gpio,
                                mraa_gpio_edge_t level,
                                void (*isr)(void*),
                                void* arg);

/**
 * Uninstall a previously installed interrupt handler
 *
 * @param dev The device context
 * @param intr One of the LIS3DH_INTERRUPT_PINS_T values
 * specifying which interrupt pin you are removing
 */
void lis3dh_uninstall_isr(const lis3dh_context dev, LIS3DH_INTERRUPT_PINS_T intr);

#ifdef __cplusplus
}
#endif
