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

#include "bmg160_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file bmg160.h
     * @library bmg160
     * @brief C API for the bmg160 driver
     *
     * @include bmg160.c
     */

    /**
     * Device context
     */
    typedef struct _bmg160_context {
        mraa_i2c_context i2c;
        mraa_spi_context spi;
        mraa_gpio_context gpioCS; // SPI CS pin
        mraa_gpio_context gpio1;  // intr 1
        mraa_gpio_context gpio2;  // intr 2

        // using SPI?
        bool isSPI;
        // use the FIFO?
        bool useFIFO;

        // always stored in C
        float temperature;

        // gyr data
        float gyrX;
        float gyrY;
        float gyrZ;

        // gyr scaling
        float gyrScale;
    } *bmg160_context;

    /**
     * BMG160 initialization.
     *
     * This device can support both I2C and SPI. For SPI, set the addr
     * to -1, and specify a positive integer representing the Chip
     * Select (CS) pin for the cs argument.  If you are using a
     * hardware CS pin (like Intel Edison with Arduino breakout), then
     * you can connect the proper pin to the hardware CS pin on your
     * MCU and supply -1 for cs.
     *
     * @param bus I2C or SPI bus to use.
     * @param addr The address for this device, or -1 for SPI.
     * @param cs The gpio pin to use for the SPI Chip Select.  Use -1 for
     * I2C or for SPI with a hardware controlled pin.
     * @return The device context, or NULL on error.
     */
    bmg160_context bmg160_init(int bus, int addr, int cs);

    /**
     * BMG160 Destructor.
     *
     * @param dev The device context.
     */
    void bmg160_close(bmg160_context dev);

    /**
     * Update the internal stored values from sensor data.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bmg160_update(const bmg160_context dev);

    /**
     * Return the chip ID.
     *
     * @param dev The device context.
     * @return The chip ID (BMG160_CHIPID).
     */
    uint8_t bmg160_get_chip_id(const bmg160_context dev);

    /**
     * Return gyroscope data in degrees per second.  bmg160_update() must
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
    void bmg160_get_gyroscope(const bmg160_context dev,
                              float *x, float *y, float *z);

    /**
     * Return the current measured temperature.  Note, this is not
     * ambient temperature.  bmg160_update() must have been called prior to
     * calling this method.
     *
     * @param dev The device context.
     * @return The temperature in degrees Celsius.
     */
    float bmg160_get_temperature(const bmg160_context dev);

    /**
     * Initialize the device and start operation.  This function is
     * called from bmg160_init(), so it will not need to be called by
     * a user unless the device is reset.
     *
     * In addition, it will enable register shadowing and enable the
     * FIFO in bypass mode.
     *
     * @param dev The device context.
     * @param pwr One of the BMG160_POWER_MODE_T values.
     * @param range One of the BMG160_RANGE_T values.
     * @param bw One of the filtering BMG160_BW_T values.
     * @return UPM result.
     */
    upm_result_t bmg160_devinit(const bmg160_context dev,
                                BMG160_POWER_MODE_T pwr,
                                BMG160_RANGE_T range,
                                BMG160_BW_T bw);

    /**
     * Reset the device as if during a power on reset.  All configured
     * values are lost when this happens.  You should call
     * bmg160_devinit() afterwards, or at least perform the same
     * initialization bmg160_devinit() does before continuing.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bmg160_reset(const bmg160_context dev);

    /**
     * Set the gyroscope detection scaling range.  This device
     * supports 125, 250, 500, 1000, and 2000 degree/s ranges.
     *
     * @param dev The device context.
     * @param range One of the BMG160_RANGE_T values.
     * @return UPM result.
     */
    upm_result_t bmg160_set_range(const bmg160_context dev,
                                  BMG160_RANGE_T range);

    /**
     * Set the output filtering bandwidth of the device.
     *
     * @param dev The device context.
     * @param bw One of the BMG160_BW_T values.
     * @return UPM result.
     */
    upm_result_t bmg160_set_bandwidth(const bmg160_context dev,
                                      BMG160_BW_T bw);

    /**
     * Set the power mode of the device.  Care must be taken when
     * setting a low power or suspend mode.  See the datasheet for
     * details.  In certain power modes, register write must be
     * drastically slowed down, which we cannot support.
     *
     * @param dev The device context.
     * @param power One of the BMG160_POWER_MODE_T values.
     * @return UPM result.
     */
    upm_result_t bmg160_set_power_mode(const bmg160_context dev,
                                       BMG160_POWER_MODE_T power);

    /**
     * Enable bmg160_update() to read from the FIFO rather than the gyroscope
     * axis registers directly.  bmg160_devinit() enables this mode by default.
     * An advantage to this mode that all axis data is sampled from
     * the same timeslice.  When reading directly from the gyroscope
     * output registers, it's possible for one axis to be updated
     * while another is being read, causing a temporal
     * inconsistency.
     *
     * Using the FIFO removes this problem.
     *
     * @param dev The device context.
     * @param useFIFO true to enable bmg160_update() to read from the FIFO.
     * When false, update will read from the gyroscope output
     * registers directly.
     */
    void bmg160_enable_fifo(const bmg160_context dev, bool useFIFO);

    /**
     * Set the FIFO watermark.  When the watermark is reached an
     * interrupt (if enabled) will be generated.
     *
     * @param dev The device context.
     * @param wm The FIFO watermark to use.  The maximum value is 63.
     * @return UPM result.
     */
    upm_result_t bmg160_fifo_set_watermark(const bmg160_context dev, int wm);

    /**
     * Set the FIFO configuration.  bmg160_devinit() uses the
     * BMG160_FIFO_MODE_BYPASS mode with axes set to
     * BMG160_FIFO_DATA_SEL_XYZ by default.
     *
     * @param dev The device context.
     * @param mode One of the BMG160_FIFO_MODE_T values.
     * @param axes One of the BMG160_FIFO_DATA_SEL_T values.
     * @return UPM result.
     */
    upm_result_t bmg160_fifo_config(const bmg160_context dev,
                                    BMG160_FIFO_MODE_T mode,
                                    BMG160_FIFO_DATA_SEL_T axes);

    /**
     * Return the Interrupt Enables 0 register.  These registers
     * allow you to enable various interrupt conditions.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @return A bitmask of BMG160_INT_EN_0_BITS_T bits.
     */
    uint8_t bmg160_get_interrupt_enable0(const bmg160_context dev);

    /**
     * Set the Interrupt Enables 0 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMG160_INT_EN_0_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bmg160_set_interrupt_enable0(const bmg160_context dev,
                                              uint8_t bits);

    /**
     * Return the Interrupt Map 0 register.  These registers allow you
     * to map specific interrupts to the interrupt 1 or interrupt 2
     * pin.  See the datasheet for details.
     *
     * @param dev The device context.
     * @return A bitmask of BMG160_INT_MAP_0_BITS_T bits.
     */
    uint8_t bmg160_get_interrupt_map0(const bmg160_context dev);

    /**
     * Set the Interrupt Map 0 register.  These registers allow you
     * to map specific interrupts to the interrupt 1 or interrupt 2
     * pin.  See the datasheet for details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMG160_INT_MAP_0_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bmg160_set_interrupt_map0(const bmg160_context dev,
                                           uint8_t bits);

    /**
     * Return the Interrupt Map 1 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @return A bitmask of BMG160_INT_MAP_1_BITS_T bits.
     */
    uint8_t bmg160_get_interrupt_map1(const bmg160_context dev);

    /**
     * Set the Interrupt Map 1 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMG160_INT_MAP_1_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bmg160_set_interrupt_map1(const bmg160_context dev,
                                           uint8_t bits);

    /**
     * Return the Interrupt source register.  This register allows
     * determining where data comes from (filtered/unfiltered) for
     * those interrupt sources where this is selectable.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @return A bitmask of BMG160_INT_1A_BITS_T bits.
     */
    uint8_t bmg160_get_interrupt_src(const bmg160_context dev);

    /**
     * Set the Interrupt source register.  This register allows
     * determining where data comes from (filtered/unfiltered) for
     * those interrupt sources where this is selectable.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMG160_INT_1A_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bmg160_set_interrupt_src(const bmg160_context dev,
                                          uint8_t bits);

    /**
     * Return the Interrupt output control register.  This register
     * allows determining the electrical characteristics of the 2
     * interrupt pins (open-drain/push-pull and level/edge
     * triggering).  See the datasheet for details.
     *
     * @param dev The device context.
     * @return A bitmask of BMG160_INT_EN_1_BITS_T bits.
     */
    uint8_t bmg160_get_interrupt_output_control(const bmg160_context dev);

    /**
     * Set the Interrupt output control register.  This register
     * allows determining the electrical characteristics of the 2
     * interrupt pins (open-drain/push-pull and level/edge
     * triggering).  See the datasheet for details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMG160_INT_EN_1_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bmg160_set_interrupt_output_control(const bmg160_context dev,
                                                     uint8_t bits);

    /**
     * Clear all latched interrupts.  See the datasheet for details.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bmg160_clear_interrupt_latches(const bmg160_context dev);

    /**
     * Return the current interrupt latching behavior.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @return One of the BMG160_RST_LATCH_T values.
     */
    BMG160_RST_LATCH_T bmg160_get_interrupt_latch_behavior(
        const bmg160_context dev);

    /**
     * Set the current interrupt latching behavior.  See the datasheet
     * for details.
     *
     * @param dev The device context.
     * @param latch One of the BMG160_RST_LATCH_T values.
     * @return UPM result.
     */
    upm_result_t bmg160_set_interrupt_latch_behavior(const bmg160_context dev,
                                                     BMG160_RST_LATCH_T latch);

    /**
     * Return the interrupt status 0 register.  These registers
     * indicate which interrupts have been triggered.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @return a bitmask of BMG160_INT_STATUS_0_BITS_T bits.
     */
    uint8_t bmg160_get_interrupt_status0(const bmg160_context dev);

    /**
     * Return the interrupt status 1 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @return a bitmask of BMG160_INT_STATUS_1_BITS_T bits.
     */
    uint8_t bmg160_get_interrupt_status1(const bmg160_context dev);

    /**
     * Return the interrupt status 2 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @return a bitmask of BMG160_INT_STATUS_2_BITS_T bits.
     */
    uint8_t bmg160_get_interrupt_status2(const bmg160_context dev);

    /**
     * Return the interrupt status 3 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @return a bitmask of BMG160_INT_STATUS_3_BITS_T bits.
     */
    uint8_t bmg160_get_interrupt_status3(const bmg160_context dev);

    /**
     * Enable shadowing of the gyroscope output registers.  When
     * enabled, a read of an axis LSB register automatically locks the
     * MSB register of that axis until it has been read.  This is
     * usually a good thing to have enabled.  bmg160_devinit() enables this by
     * default.  If disabled, then it becomes possible for part of an
     * axis value to change while another part is being read, causing
     * inconsistent data.
     *
     * @param dev The device context.
     * @param shadow true to enable axis register shadowing, false otherwise.
     * @return UPM result.
     */
    upm_result_t bmg160_enable_register_shadowing(const bmg160_context dev,
                                                  bool shadow);

    /**
     * Enable filtering of the gyroscope axis data.  bmg160_devinit()
     * enables this by default.  If disabled, then gyroscope data
     * that is read will be raw and unfiltered (rated R).  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @param filter true to enable filtering, false to disable.
     * @return UPM result.
     */
    upm_result_t bmg160_enable_output_filtering(const bmg160_context dev,
                                                bool filter);

    /**
     * install an interrupt handler.
     *
     * @param dev The device context.
     * @param intr One of the BMG160_INTERRUPT_PINS_T values
     * specifying which interrupt pin you are installing.
     * @param gpio GPIO pin to use as interrupt pin
     * @param level The interrupt trigger level (one of mraa_gpio_edge_t
     * values).  Make sure that you have configured the interrupt pin
     * properly for whatever level you choose.
     * @param isr The interrupt handler, accepting a void * argument
     * @param arg The argument to pass the the interrupt handler
     * @return UPM result.
     */
    upm_result_t bmg160_install_isr(const bmg160_context dev,
                                    BMG160_INTERRUPT_PINS_T intr, int gpio,
                                    mraa_gpio_edge_t level,
                                    void (*isr)(void *), void *arg);

   /**
     * uninstall a previously installed interrupt handler
     *
     * @param dev The device context.
     * @param intr One of the BMG160_INTERRUPT_PINS_T values
     * specifying which interrupt pin you are removing.
     */
    void bmg160_uninstall_isr(const bmg160_context dev,
                              BMG160_INTERRUPT_PINS_T intr);

    /**
     * Read a register.
     *
     * @param dev The device context.
     * @param reg The register to read.
     * @return The value of the register.
     */
    uint8_t bmg160_read_reg(const bmg160_context dev, uint8_t reg);

    /**
     * Read contiguous registers into a buffer.
     *
     * @param dev The device context.
     * @param buffer The buffer to store the results.
     * @param len The number of registers to read.
     * @return The number of bytes read, or -1 on error.
     */
    int bmg160_read_regs(const bmg160_context dev, uint8_t reg,
                         uint8_t *buffer, int len);

    /**
     * Write to a register.
     *
     * @param dev The device context.
     * @param reg The register to write to.
     * @param val The value to write.
     * @return UPM result.
     */
    upm_result_t bmg160_write_reg(const bmg160_context dev,
                                  uint8_t reg, uint8_t val);


#ifdef __cplusplus
}
#endif
