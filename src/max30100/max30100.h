/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <errno.h>
#include <stdint.h>

#include "max30100_regs.h"

#include "mraa/gpio.h"
#include "mraa/i2c.h"
#include "upm.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file max30100.h
 * @library max30100
 * @brief C API for the Pulse oximeter and heart-rate sensor.
 *
 * @include max30100.c
 */

/**
 * device context
 */
typedef struct {
    /* mraa I2C context */
    mraa_i2c_context _i2c_context;

    /* mraa gpio context (for interrupt) */
    mraa_gpio_context _gpio_context;

    /* Sensor acquire mode */
    MAX30100_SAMPLE_STATE sample_state;

    /* Continuous sampling function ptr */
    func_sample_ready_handler func_sample_ready;

    /* Optional void ptr arg returned from callback */
    void* arg;
} max30100_context;

/**
 * Initialize sensor.  Note, the MAX30100 I2C address is set to 0x57.
 *
 * @param i2c_bus Target I2C bus
 * @return sensor context pointer
 */
max30100_context* max30100_init(int16_t i2c_bus);

/**
 * Sensor close method.
 *
 * Cleans up any memory held by this device
 * @param dev Sensor context pointer
 */
void max30100_close(max30100_context* dev);

/**
 * Sample a single set of infrared/red values
 *
 * Note, all setup (sample rate, LED current, and pulse width must be done
 * prior to calling this sample method.
 *
 * @param dev Sensor context pointer
 * @param samp IR/R values are returned in this structure
 * @return Function result code
 */
upm_result_t max30100_sample(max30100_context* dev, max30100_value *samp);

/**
 * Continuously sample Infrared/Red values.
 *
 * This method requires a GPIO pin which is used to signal
 * sample/samples ready.  The INT * pin is open-drain and requires a
 * pullup resistor. The interrupt pin is not designed  to sink large
 * currents, so the pull-up resistor value should be large, such as
 * 4.7k ohm.  The RCWL-0530 PCB which this library was designed with
 * had the I2C lines and INT pin pulled up to 1.8v.
 *
 * Note, all setup (sample rate, mode, LED current, and pulse width
 * must be done prior to calling this sample method.
 *
 * @param dev Sensor context pointer
 * @param gpio_pin GPIO pin used for interrupt (input from sensor INT pin)
 * @param buffered Enable buffered sampling.  In buffered sampling mode, the
 * device reads 16 samples at a time.  This can help with I2C read timing.
 *      buffered == true, enable buffered sampling
 *      buffered == false, single-sample mode
 * @param isr Function pointer which handles 1 IR/R sample and a void ptr arg
 * @param arg Void * passed back with ISR call
 * @return Function result code
 */
upm_result_t max30100_sample_continuous(max30100_context* dev,
                                        int gpio_pin,
                                        bool buffered,
                                        func_sample_ready_handler isr,
                                        void* arg);

/**
 * Stop continuous sampling.  Disable interrupts.
 *
 * @param dev Sensor context pointer
 * @return Function result code
 */
upm_result_t max30100_sample_stop(max30100_context* dev);

/**
 * Read Oximeter and heart-rate sensor register
 *
 * @param dev Sensor context pointer
 * @param reg Target register
 * @param rd_data Data from sensor
 * @return Function result code
 */
upm_result_t max30100_read(const max30100_context* dev, MAX30100_REG reg, uint8_t* rd_data);

/**
 * Write Oximeter and heart-rate sensor register
 *
 * @param dev Sensor context pointer
 * @param reg Target register to write
 * @param wr_data Target data to write
 * @return Function result code
 */
upm_result_t max30100_write(const max30100_context* dev, MAX30100_REG reg, uint8_t wr_data);

/**
 * Read modify write Oximeter and heart-rate sensor register
 *
 * @param dev Sensor context pointer
 * @param reg Target register
 * @param value Target bits to set/clear
 * @param mask Specify the bits to set/clear
 *      If mask = 0xf0, read full byte, modify only the upper 4 bits
 *      If mask = 0xaa, read full byte, modify every other bit
 * @return Function result code
 */
upm_result_t max30100_rd_mod_wr(const max30100_context* dev,
        MAX30100_REG reg, uint8_t value, uint8_t mask);

/**
 * Get sensor version
 * Sensor version is a 2 byte value:
 *      upper byte = PART ID
 *      lower byte = REVISION ID
 *
 * example:
 *      version() return 0x1105
 *      0x11 = PART ID
 *      0x05 = REVISION
 *
 * @param dev Sensor context pointer
 * @param rd_data Sensor version
 * @return Function result code
 */
upm_result_t max30100_get_version(const max30100_context* dev, uint16_t* version);

/**
 * Get temperature reading from device
 * @param dev Sensor context pointer
 * @param rd_data Temperature in degrees Celsius
 * @return Function result code
 */
upm_result_t max30100_get_temperature(const max30100_context* dev, float* temperature);

/**
 * Set the sampling mode (none vs red only vs SpO2)
 *
 * @param dev Sensor context pointer
 * @param mode Sensor mode value to write into the mode configuration register
 * @return Function result code
 */
upm_result_t max30100_set_mode(const max30100_context* dev, MAX30100_MODE mode);

/**
 * Get the mode field from the mode configuration register
 *
 * @param dev Sensor context pointer
 * @param mode Sensor mode value read from mode configuration register
 * @return Function result code
 */
upm_result_t max30100_get_mode(const max30100_context* dev, MAX30100_MODE* mode);

/**
 * Set the high-res field in the SpO2 configuration register
 *
 * @param dev Sensor context pointer
 * @param high_res Sensor high-res value to write into the SpO2 configuration register
 * @return Function result code
 */
upm_result_t max30100_set_high_res(const max30100_context* dev, bool high_res);

/**
 * Get the high-res field from the SpO2 configuration register
 *
 * @param dev Sensor context pointer
 * @param high_res Sensor high_res value read from the SpO2 configuration register
 * @return Function result code
 */
upm_result_t max30100_get_high_res(const max30100_context* dev, bool* high_res);

/**
 * Set the sample rate field in the SpO2 configuration register
 *
 * @param dev Sensor context pointer
 * @param sample_rate Sensor sample rate value to write into the SpO2 configuration register
 * @return Function result code
 */
upm_result_t max30100_set_sample_rate(const max30100_context* dev, MAX30100_SR sample_rate);

/**
 * Get the sample rate field from the SpO2 configuration register
 *
 * @param dev Sensor context pointer
 * @param sample_rate Sensor sample rate value read from the SpO2 configuration register
 * @return Function result code
 */
upm_result_t max30100_get_sample_rate(const max30100_context* dev, MAX30100_SR* sample_rate);

/**
 * Set the pulse width field in the SpO2 configuration register
 *
 * @param dev Sensor context pointer
 * @param pulse_width Sensor pulse width value to write into the SpO2 configuration register
 * @return Function result code
 */
upm_result_t max30100_set_pulse_width(const max30100_context* dev, MAX30100_LED_PW pulse_width);

/**
 * Get the pulse width field from the SpO2 configuration register
 *
 * @param dev Sensor context pointer
 * @param pulse_width Sensor pulse width value read from the SpO2 configuration register
 * @return Function result code
 */
upm_result_t max30100_get_pulse_width(const max30100_context* dev, MAX30100_LED_PW* pulse_width);

/**
 * Set the LED current
 *
 * @param dev Sensor context pointer
 * @param ir Infrared LED current enum
 * @param r Red LED current enum
 * @return Function result code
 */
upm_result_t max30100_set_current(const max30100_context* dev,
        MAX30100_LED_CURRENT ir,
        MAX30100_LED_CURRENT r);

/**
 * Get the LED current
 *
 * @param dev Sensor context pointer
 * @param ir Infrared LED current read from the LED configuration register
 * @param r Red LED current read from the LED configuration register
 * @return Function result code
 */
upm_result_t max30100_get_current(const max30100_context* dev,
        MAX30100_LED_CURRENT* ir,
        MAX30100_LED_CURRENT* r);

/**
 * Reset sensor
 *
 * When the RESET bit is set to one, all configuration, threshold,
 * and data registers are reset to their power-on-state. The only
 * exception is writing both RESET and TEMP_EN bits to one at the
 * same time since temperature data registers 0x16 and 0x17 are not
 * cleared. The RESET bit is cleared automatically back to zero after
 * the reset sequence is completed.
 *
 * @param dev Sensor context pointer
 * @return Function result code
 */
upm_result_t max30100_reset(const max30100_context* dev);

/**
 * Put device into power-save mode.  While in power-save mode, all
 * registers retain their values, and write/read operations function
 * as normal. All interrupts are cleared to zero in this mode.
 *
 * @param dev Sensor context pointer
 * @param sleep Enter/exit power-save mode
 *      true = Enter power-save mode
 *      false = Exit power-save mode
 * @return Function result code
 */
upm_result_t max30100_sleep(const max30100_context* dev, bool sleep);

#ifdef __cplusplus
}
#endif
