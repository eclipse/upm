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

#include "bmm150_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file bmm150.h
     * @library bmm150
     * @brief C API for the bmm150 driver
     *
     * @include bmm150.c
     */

    /**
     * Device context
     */
    typedef struct _bmm150_context {
        mraa_i2c_context i2c;
        mraa_spi_context spi;
        mraa_gpio_context gpioCS;   // SPI CS pin
        mraa_gpio_context gpioINT;  // intr
        mraa_gpio_context gpioDR;   // DR (Data Ready) intr

        // using SPI?
        bool isSPI;
        BMM150_OPERATION_MODE_T opmode;

        // mag data
        float magX;
        float magY;
        float magZ;

        // hall resistance (it is NOT futile!)
        uint16_t hall;

        // trimming data
        int8_t dig_x1;
        int8_t dig_y1;

        int16_t dig_z4;
        int8_t dig_x2;
        int8_t dig_y2;

        int16_t dig_z2;
        uint16_t dig_z1;
        uint16_t dig_xyz1;
        int16_t dig_z3;
        int8_t dig_xy2;
        uint8_t dig_xy1;
    } *bmm150_context;


    /**
     * BMM150 initialization.
     *
     * This device can support both I2C and SPI. For SPI, set the addr
     * to -1, and specify a positive integer representing the Chip
     * Select (CS) pin for the cs argument.  If you are using a
     * hardware CS pin (like edison with arduino breakout), then you
     * can connect the proper pin to the hardware CS pin on your MCU
     * and supply -1 for cs.  The default operating mode is I2C.
     *
     * @param bus I2C or SPI bus to use.
     * @param addr The address for this device, or -1 for SPI.
     * @param cs The gpio pin to use for the SPI Chip Select.  Use -1
     * for I2C or for SPI with a hardware controlled pin.
     * @return The device context, or NULL if an error occurred.
     */
    bmm150_context bmm150_init(int bus, int addr, int cs);

    /**
     * BMM150 Destructor.
     *
     * @param dev The device context.
     */
    void bmm150_close(bmm150_context dev);

    /**
     * Update the internal stored values from sensor data.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bmm150_update(const bmm150_context dev);

    /**
     * Return the chip ID.
     *
     * @param dev The device context.
     * @return The chip ID.
     */
    uint8_t bmm150_get_chip_id(const bmm150_context dev);

    /**
     * Return magnetometer data in micro-Teslas (uT).  update() must
     * have been called prior to calling this method.
     *
     * @param dev The device context.
     * @param x Pointer to a floating point value that will have the
     * current x component placed into it.
     * @param y Pointer to a floating point value that will have the
     * current y component placed into it.
     * @param z Pointer to a floating point value that will have the
     * current z component placed into it.
     */
    void bmm150_get_magnetometer(const bmm150_context dev,
                                 float *x, float *y, float *z);

    /**
     * Initialize the device and start operation.  This function is
     * called from the constructor so it will not typically need to be
     * called by a user unless the device is reset.  This method will
     * call bmm150_set_preset_mode() with the passed parameter.
     *
     * @param dev The device context.
     * @param usage One of the BMM150_USAGE_PRESETS_T values.  The
     * default is BMM150_USAGE_HIGH_ACCURACY.
     * @return UPM result.
     */
    upm_result_t bmm150_devinit(const bmm150_context dev,
                                BMM150_USAGE_PRESETS_T usage);

    /**
     * Set one of the Bosch recommended preset modes.  These modes
     * configure the sensor for varying use cases.
     *
     * @param dev The device context.
     * @param usage One of the BMM150_USAGE_PRESETS_T values.  The
     * default set at initilaization time is
     * BMM150_USAGE_HIGH_ACCURACY.
     * @return UPM result.
     */
    upm_result_t bmm150_set_preset_mode(const bmm150_context dev,
                                        BMM150_USAGE_PRESETS_T usage);

    /**
     * Perform a device soft-reset.  The device will be placed in
     * SUSPEND mode afterward with all configured setting lost, so
     * some re-initialization will be required to get data from the
     * sensor.  Calling bmm150_devinit() will get everything running
     * again.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bmm150_reset(const bmm150_context dev);

    /**
     * Set the magnetometer Output Data Rate. See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @param odr One of the BMM150_DATA_RATE_T values.
     * @return UPM result.
     */
    upm_result_t bmm150_set_output_data_rate(const bmm150_context dev,
                                             BMM150_DATA_RATE_T odr);

    /**
     * Set or clear the Power bit.  When the power bit is cleared, the
     * device enters a deep suspend mode where only the
     * BMM150_REG_POWER_CTRL register can be accessed.  This bit needs
     * to be enabled for the device to operate.  See the datasheet for
     * details.  The constructor enables this by default.  After a
     * deep suspend mode has been entered, all configured data is lost
     * and the device must be reconfigured (as via bmm150_devinit()).
     *
     * @param dev The device context.
     * @param power True to enable the bit, false otherwise.
     * @return UPM result.
     */
    upm_result_t bmm150_set_power_bit(const bmm150_context dev, bool power);

    /**
     * Set the operating mode of the device.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @param power One of the BMM150_POWER_MODE_T values.
     * @return UPM result.
     */
    upm_result_t bmm150_set_opmode(const bmm150_context dev,
                                   BMM150_OPERATION_MODE_T opmode);

    /**
     * Get the current operating mode of the device.  See the datasheet for
     * details.  The power bit must be one for this method to succeed.
     *
     * @param dev The device context.
     * @return One of the BMM150_OPERATION_MODE_T values.
     */
    BMM150_OPERATION_MODE_T bmm150_get_opmode(const bmm150_context dev);

    /**
     * Return the Interrupt Enables register.  This register
     * allows you to enable various interrupt conditions.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @return A bitmask of BMM150_INT_EN_BITS_T bits.
     */
    uint8_t bmm150_get_interrupt_enable(const bmm150_context dev);

    /**
     * Set the Interrupt Enables register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMM150_INT_EN_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bmm150_set_interrupt_enable(const bmm150_context dev,
                                             uint8_t bits);

    /**
     * Return the Interrupt Config register.  This register allows
     * determining the electrical characteristics of the 2 interrupt
     * pins (open-drain/push-pull and level/edge triggering) as well
     * as other options.  See the datasheet for details.
     *
     * @param dev The device context.
     * @return A bitmask of BMM150_INT_CONFIG_BITS_T bits.
     */
    uint8_t bmm150_get_interrupt_config(const bmm150_context dev);

    /**
     * Set the Interrupt Config register.  This register
     * allows determining the electrical characteristics of the 2
     * interrupt pins (open-drain/push-pull and level/edge
     * triggering).  See the datasheet for details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMM150_INT_CONFIG_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bmm150_set_interrupt_config(const bmm150_context dev,
                                             uint8_t bits);

    /**
     * Return the interrupt status register.  This register
     * indicates which interrupts have been triggered.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @return a bitmask of BMM150_INT_STATUS_BITS_T bits.
     */
    uint8_t bmm150_get_interrupt_status(const bmm150_context dev);

    /**
     * Set the repetition counter for the X and Y axes.  This allows the
     * device to average a number of measurements for a more stable
     * output.  See the datasheet for details.
     *
     * @param dev The device context.
     * @param reps A coefficient for specifying the number of
     * repititions to perform. (1 + 2(reps))
     * @return UPM result.
     */
    upm_result_t bmm150_set_repetitions_xy(const bmm150_context dev,
                                           uint8_t reps);

    /**
     * Set the repetition counter for the Z axis.  This allows the
     * device to average a number of measurements for a more stable
     * output.  See the datasheet for details.
     *
     * @param dev The device context.
     * @param reps A coefficient for specifying the number of
     * repititions to perform. (1 + (reps))
     * @return UPM result.
     */
    upm_result_t bmm150_set_repetitions_z(const bmm150_context dev,
                                          uint8_t reps);

    /**
     * Install an interrupt handler.
     *
     * @param dev The device context.
     * @param intr One of the BMM150_INTERRUPT_PINS_T values
     * specifying which interrupt pin you are installing.
     * @param gpio GPIO pin to use as interrupt pin.
     * @param level The interrupt trigger level (one of the
     * mraa_gpio_edge_t values).  Make sure that you have configured
     * the interrupt pin properly for whatever level you choose.
     * @param isr The interrupt handler, accepting a void * argument
     * @param arg The argument to pass the the interrupt handler.
     * @return UPM result.
     */
    upm_result_t bmm150_install_isr(const bmm150_context dev,
                                    BMM150_INTERRUPT_PINS_T intr,
                                    int gpio, mraa_gpio_edge_t level,
                                    void (*isr)(void *), void *arg);

   /**
     * Uninstall a previously installed interrupt handler.
     *
     * @param dev The device context.
     * @param intr One of the BMM150_INTERRUPT_PINS_T values
     * specifying which interrupt pin you are removing.
     */
    void bmm150_uninstall_isr(const bmm150_context dev,
                              BMM150_INTERRUPT_PINS_T intr);

    /**
     * Read a register.
     *
     * @param dev The device context.
     * @param reg The register to read.
     * @return The value of the register.
     */
    uint8_t bmm150_read_reg(const bmm150_context dev, uint8_t reg);

    /**
     * Read contiguous registers into a buffer.
     *
     * @param dev The device context.
     * @param buffer The buffer to store the results.
     * @param len The number of registers to read.
     * @return The number of bytes read, or -1 on error.
     */
    int bmm150_read_regs(const bmm150_context dev, uint8_t reg,
                         uint8_t *buffer, int len);

    /**
     * Write to a register.
     *
     * @param dev The device context.
     * @param reg The register to write to.
     * @param val The value to write.
     * @return UPM result.
     */
    upm_result_t bmm150_write_reg(const bmm150_context dev,
                                  uint8_t reg, uint8_t val);

#ifdef __cplusplus
}
#endif
