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

#include "bma250e_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file bma250e.h
     * @library bma250e
     * @brief C API for the bma250e driver
     *
     * @include bma250e.c
     */

    /**
     * Device context
     */
    typedef struct _bma250e_context {
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

        // does this chip support the fifo?
        bool fifoAvailable;

        // supported resolution
        BMA250E_RESOLUTION_T resolution;

        // acc data
        float accX;
        float accY;
        float accZ;

        // acceleration scaling
        float accScale;
    } *bma250e_context;

    /**
     * BMA250E initialization.
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
    bma250e_context bma250e_init(int bus, int addr, int cs);

    /**
     * BMA250E Destructor.
     *
     * @param dev The device context.
     */
    void bma250e_close(bma250e_context dev);

    /**
     * Update the internal stored values from sensor data.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bma250e_update(const bma250e_context dev);

    /**
     * Return the chip ID.
     *
     * @param dev The device context.
     * @return The chip ID.
     */
    uint8_t bma250e_get_chip_id(const bma250e_context dev);

    /**
     * Return accelerometer data in gravities.  bma250e_update() must
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
    void bma250e_get_accelerometer(const bma250e_context dev,
                                   float *x, float *y, float *z);

    /**
     * Return the current measured temperature.  Note, this is not
     * ambient temperature.  bma250e_update() must have been called prior to
     * calling this method.
     *
     * @param dev The device context.
     * @return The temperature in degrees Celsius.
     */
    float bma250e_get_temperature(const bma250e_context dev);

    /**
     * Initialize the device and start operation.  This function is
     * called from bma250e_init(), so it will not need to be called by
     * a user unless the device is reset.
     *
     * @param dev The device context.
     * @param pwr One of the BMA250E_POWER_MODE_T values.
     * @param range One of the BMA250E_RANGE_T values.
     * @param bw One of the filtering BMA250E_BW_T values.
     * @return UPM result.
     */
    upm_result_t bma250e_devinit(const bma250e_context dev,
                                 BMA250E_POWER_MODE_T pwr,
                                 BMA250E_RANGE_T range,
                                 BMA250E_BW_T bw);

    /**
     * Reset the device as if during a power on reset.  All configured
     * values are lost when this happens.  You should call
     * bma250e_devinit() afterwards, or at least perform the same
     * initialization bma250e_devinit() does before continuing.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bma250e_reset(const bma250e_context dev);

    /**
     * Set the acceleration scaling range.  This device supports 2, 4,
     * 8, and 16g ranges.
     *
     * @param dev The device context.
     * @param range One of the BMA250E_RANGE_T values.
     * @return UPM result.
     */
    upm_result_t bma250e_set_range(const bma250e_context dev,
                                   BMA250E_RANGE_T range);

    /**
     * Set the output filtering bandwidth of the device.
     *
     * @param dev The device context.
     * @param bw One of the BMA250E_BW_T values.
     * @return UPM result.
     */
    upm_result_t bma250e_set_bandwidth(const bma250e_context dev,
                                       BMA250E_BW_T bw);

    /**
     * Set the power mode of the device.  Care must be taken when
     * setting a low power or suspend mode.  By default
     * bma250e_devinit() calls bma250e_set_low_power_mode2() to ensure
     * that if any of these modes are entered we can still talk to the
     * device.  The default low power mode is LPM1, which requires
     * slowing down register writes, which we cannot support.
     * bma250e_set_low_power_mode2() enables LPM2 which keeps the
     * digital interface operational in low power or suspend modes.
     * See the datasheet for details.
     *
     * In short, if you reset your device and don't call
     * bma250e_devinit() or bma250e_set_low_power_mode2(), you could
     * lose control of the device by calling this function with
     * anything other than BMA250E_POWER_MODE_NORMAL.  You've been
     * warned :)
     *
     * @param dev The device context.
     * @param power One of the BMA250E_POWER_MODE_T values.
     * @return UPM result.
     */
    upm_result_t bma250e_set_power_mode(const bma250e_context dev,
                                        BMA250E_POWER_MODE_T power);

    /**
     * Enable bma250e_update() to read from the FIFO rather than the
     * accelerometer axis registers directly, if the device supports a
     * FIFO.  bma250e_devinit() enables this mode by default.  An
     * advantage to this mode that all axis data is sampled from the
     * same timeslice.  When reading directly from the accelerometer
     * output registers, it's possible for one axis to be updated
     * while another is being read, causing a temporal inconsistency.
     *
     * Using the FIFO removes this problem.
     *
     * @param dev The device context.
     * @param useFIFO true to enable bma250e_update() to read from the FIFO.
     * When false, update will read from the accelerometer output
     * registers directly.
     */
    void bma250e_enable_fifo(const bma250e_context dev, bool useFIFO);

    /**
     * Set the FIFO watermark.  When the watermark is reached an
     * interrupt (if enabled) will be generated.
     *
     * @param dev The device context.
     * @param wm The FIFO watermark to use.  The maximum value is 63.
     * @return UPM result.
     */
    upm_result_t bma250e_fifo_set_watermark(const bma250e_context dev, int wm);

    /**
     * Set the FIFO configuration.  bma250e_devinit() uses the
     * BMA250E_FIFO_MODE_BYPASS mode with axes set to
     * BMA250E_FIFO_DATA_SEL_XYZ by default.
     *
     * @param dev The device context.
     * @param mode One of the BMA250E_FIFO_MODE_T values.
     * @param axes One of the BMA250E_FIFO_DATA_SEL_T values.
     * @return UPM result.
     */
    upm_result_t bma250e_fifo_config(const bma250e_context dev,
                                     BMA250E_FIFO_MODE_T mode,
                                     BMA250E_FIFO_DATA_SEL_T axes);

    /**
     * Enable, disable, and configure the built in self test on a per
     * axis basis.  See the datasheet for details.
     *
     * @param sign True for a positive deflection, false for negative.
     * @param amp True for a high deflection, false for a low deflection.
     * @param axis One of the BMA250E_SELFTTEST_AXIS_T values.  Note,
     * only one axis at a time can be tested.  Accelerometer output
     * for other axes should be ignored.
     * @return UPM result.
     */
    upm_result_t bma250e_set_self_test(const bma250e_context dev,
                                       bool sign, bool amp,
                                       BMA250E_SELFTTEST_AXIS_T axis);

    /**
     * Return the Interrupt Enables 0 register.  These registers
     * allow you to enable various interrupt conditions.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @return A bitmask of BMA250E_INT_EN_0_BITS_T bits.
     */
    uint8_t bma250e_get_interrupt_enable0(const bma250e_context dev);

    /**
     * Set the Interrupt Enables 0 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMA250E_INT_EN_0_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bma250e_set_interrupt_enable0(const bma250e_context dev,
                                               uint8_t bits);

    /**
     * Return the Interrupt Enables 1 register.  These registers
     * allow you to enable various interrupt conditions.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @return A bitmask of BMA250E_INT_EN_1_BITS_T bits.
     */
    uint8_t bma250e_get_interrupt_enable1(const bma250e_context dev);

    /**
     * Set the Interrupt Enables 1 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMA250E_INT_EN_1_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bma250e_set_interrupt_enable1(const bma250e_context dev,
                                               uint8_t bits);
    /**
     * Return the Interrupt Enables 2 register.  These registers
     * allow you to enable various interrupt conditions.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @return A bitmask of BMA250E_INT_EN_2_BITS_T bits.
     */
    uint8_t bma250e_get_interrupt_enable2(const bma250e_context dev);

    /**
     * Set the Interrupt Enables 2 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMA250E_INT_EN_2_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bma250e_set_interrupt_enable2(const bma250e_context dev,
                                               uint8_t bits);

    /**
     * Return the Interrupt Map 0 register.  These registers allow you
     * to map specific interrupts to the interrupt 1 or interrupt 2
     * pin.  See the datasheet for details.
     *
     * @param dev The device context.
     * @return A bitmask of BMA250E_INT_MAP_0_BITS_T bits.
     */
    uint8_t bma250e_get_interrupt_map0(const bma250e_context dev);

    /**
     * Set the Interrupt Map 0 register.  These registers allow you
     * to map specific interrupts to the interrupt 1 or interrupt 2
     * pin.  See the datasheet for details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMA250E_INT_MAP_0_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bma250e_set_interrupt_map0(const bma250e_context dev,
                                            uint8_t bits);

    /**
     * Return the Interrupt Map 1 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @return A bitmask of BMA250E_INT_MAP_1_BITS_T bits.
     */
    uint8_t bma250e_get_interrupt_map1(const bma250e_context dev);

    /**
     * Set the Interrupt Map 1 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMA250E_INT_MAP_1_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bma250e_set_interrupt_map1(const bma250e_context dev,
                                            uint8_t bits);

    /**
     * Return the Interrupt Map 2 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @return A bitmask of BMA250E_INT_MAP_2_BITS_T bits.
     */
    uint8_t bma250e_get_interrupt_map2(const bma250e_context dev);

    /**
     * Set the Interrupt Map 2 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMA250E_INT_MAP_2_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bma250e_set_interrupt_map2(const bma250e_context dev,
                                            uint8_t bits);

    /**
     * Return the Interrupt source register.  This register allows
     * determining where data comes from (filtered/unfiltered) for
     * those interrupt sources where this is selectable.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @return A bitmask of BMA250E_INT_SRC_BITS_T bits.
     */
    uint8_t bma250e_get_interrupt_src(const bma250e_context dev);

    /**
     * Set the Interrupt source register.  This register allows
     * determining where data comes from (filtered/unfiltered) for
     * those interrupt sources where this is selectable.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMA250E_INT_SRC_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bma250e_set_interrupt_src(const bma250e_context dev,
                                           uint8_t bits);

    /**
     * Return the Interrupt output control register.  This register
     * allows determining the electrical characteristics of the 2
     * interrupt pins (open-drain/push-pull and level/edge
     * triggering).  See the datasheet for details.
     *
     * @param dev The device context.
     * @return A bitmask of BMA250E_INT_OUT_CTRL_BITS_T bits.
     */
    uint8_t bma250e_get_interrupt_output_control(const bma250e_context dev);

    /**
     * Set the Interrupt output control register.  This register
     * allows determining the electrical characteristics of the 2
     * interrupt pins (open-drain/push-pull and level/edge
     * triggering).  See the datasheet for details.
     *
     * @param dev The device context.
     * @param bits A bitmask of BMA250E_INT_OUT_CTRL_BITS_T bits.
     * @return UPM result.
     */
    upm_result_t bma250e_set_interrupt_output_control(const bma250e_context dev,
                                                      uint8_t bits);

    /**
     * Clear all latched interrupts.  See the datasheet for details.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bma250e_clear_interrupt_latches(const bma250e_context dev);

    /**
     * Return the current interrupt latching behavior.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @return One of the BMA250E_RST_LATCH_T values.
     */
    BMA250E_RST_LATCH_T bma250e_get_interrupt_latch_behavior(
        const bma250e_context dev);

    /**
     * Set the current interrupt latching behavior.  See the datasheet
     * for details.
     *
     * @param dev The device context.
     * @param latch One of the BMA250E_RST_LATCH_T values.
     * @return UPM result.
     */
    upm_result_t bma250e_set_interrupt_latch_behavior(
        const bma250e_context dev,
        BMA250E_RST_LATCH_T latch);

    /**
     * Return the interrupt status 0 register.  These registers
     * indicate which interrupts have been triggered.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @return a bitmask of BMA250E_INT_STATUS_0_BITS_T bits.
     */
    uint8_t bma250e_get_interrupt_status0(const bma250e_context dev);

    /**
     * Return the interrupt status 1 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @return a bitmask of BMA250E_INT_STATUS_1_BITS_T bits.
     */
    uint8_t bma250e_get_interrupt_status1(const bma250e_context dev);

    /**
     * Return the interrupt status 2 register.  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @return a bitmask of BMA250E_INT_STATUS_2_BITS_T bits.
     */
    uint8_t bma250e_get_interrupt_status2(const bma250e_context dev);

    /**
     * Return the interrupt status 3 register bitfields.  See the
     * datasheet for details.  The Orientation value is not returned
     * by this function, see
     * bma250e_get_interrupt_status3_orientation() for that
     * information.
     *
     * @param dev The device context.
     * @return a bitmask of BMA250E_INT_STATUS_3_BITS_T bits only.
     */
    uint8_t bma250e_get_interrupt_status3_bits(const bma250e_context dev);

    /**
     * Return the interrupt status 3 register Orientation value.  See the
     * datasheet for details.
     *
     * @param dev The device context.
     * @return One of the BMA250E_ORIENT_T values.
     */
    BMA250E_ORIENT_T bma250e_get_interrupt_status3_orientation(
        const bma250e_context dev);

    /**
     * Enable shadowing of the accelerometer output registers.  When
     * enabled, a read of an axis LSB register automatically locks the
     * MSB register of that axis until it has been read.  This is
     * usually a good thing to have enabled.  bma250e_devinit() enables
     * this by default.  If disabled, then it becomes possible for
     * part of an axis value to change while another part is being
     * read, causing inconsistent data.
     *
     * @param dev The device context.
     * @param shadow true to enable axis register shadowing, false otherwise.
     * @return UPM result.
     */
    upm_result_t bma250e_enable_register_shadowing(const bma250e_context dev,
                                                   bool shadow);

    /**
     * Enable filtering of the accelerometer axis data.
     * bma250e_devinit() enables this by default.  If disabled, then
     * accelerometer data that is read will be raw and unfiltered
     * (rated NC-17, mature audiences only).  See the datasheet for
     * details.
     *
     * @param dev The device context.
     * @param filter true to enable filtering, false to disable.
     * @return UPM result.
     */
    upm_result_t bma250e_enable_output_filtering(const bma250e_context dev,
                                                 bool filter);

    /**
     * Make sure low power mode config (LPM2) is set in case we later
     * go into the low power or suspend power modes.  LPM1 mode (the
     * default) requires drastically slowed register writes which we
     * cannot handle.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bma250e_set_low_power_mode2(const bma250e_context dev);

    /**
     * install an interrupt handler.
     *
     * @param dev The device context.
     * @param intr One of the BMA250E_INTERRUPT_PINS_T values
     * specifying which interrupt pin you are installing.
     * @param gpio GPIO pin to use as interrupt pin
     * @param level The interrupt trigger level (one of the
     * mraa_gpio_edge_t values).  Make sure that you have configured
     * the interrupt pin properly for whatever level you choose.
     * @param isr The interrupt handler, accepting a void * argument
     * @param arg The argument to pass to the interrupt handler
     * @return UPM result.
     */
    upm_result_t bma250e_install_isr(const bma250e_context dev,
                                     BMA250E_INTERRUPT_PINS_T intr, int gpio,
                                     mraa_gpio_edge_t level,
                                     void (*isr)(void *), void *arg);

    /**
     * uninstall a previously installed interrupt handler
     *
     * @param dev The device context.
     * @param intr One of the BMA250E_INTERRUPT_PINS_T values
     * specifying which interrupt pin you are removing.
     */
    void bma250e_uninstall_isr(const bma250e_context dev,
                               BMA250E_INTERRUPT_PINS_T intr);

    /**
     * Read a register.
     *
     * @param dev The device context.
     * @param reg The register to read.
     * @return The value of the register.
     */
    uint8_t bma250e_read_reg(const bma250e_context dev, uint8_t reg);

    /**
     * Read contiguous registers into a buffer.
     *
     * @param dev The device context.
     * @param buffer The buffer to store the results.
     * @param len The number of registers to read.
     * @return The number of bytes read, or -1 on error.
     */
    int bma250e_read_regs(const bma250e_context dev, uint8_t reg,
                          uint8_t *buffer, int len);

    /**
     * Write to a register.
     *
     * @param dev The device context.
     * @param reg The register to write to.
     * @param val The value to write.
     * @return UPM result.
     */
    upm_result_t bma250e_write_reg(const bma250e_context dev,
                                   uint8_t reg, uint8_t val);


#ifdef __cplusplus
}
#endif
