/*
* The MIT License (MIT)
*
* Author: Assam Boudjelthia
* Copyright (c) 2018 Rohm Semiconductor.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <mraa/i2c.h>
#include <mraa/gpio.h>
#include <unistd.h>
#include <upm_types.h>
#include "bh1749_registers.h"

/**
 * @file bh1749.h
 * @library bh1749
 * @brief C API for the bh1749 driver
 *
 * @include bh1749.c
 */

/**
 * @brief Operation modes enum for interrupt modes (persistance)
 */
typedef enum {
    INT_JUDGE_0 = BH1749_PERSISTENCE_MODE_STATUS_ACTIVE_AFTER_MEASUREMENT,
    INT_JUDGE_1 = BH1749_PERSISTENCE_MODE_STATUS_UPDATE_AFTER_MEASUREMENT,
    INT_JUDGE_4 = BH1749_PERSISTENCE_MODE_STATUS_UPDATE_AFTER_4_SAME,
    INT_JUDGE_8 = BH1749_PERSISTENCE_MODE_STATUS_UPDATE_AFTER_8_SAME,
} OPERATING_MODES;

/**
 * @brief Measuremnt time choices
 */
typedef enum {
    MEAS_35MS = BH1749_MODE_CONTROL1_ODR_28P6,
    MEAS_120MS = BH1749_MODE_CONTROL1_ODR_8P333,
    MEAS_240MS = BH1749_MODE_CONTROL1_ODR_4P167
} MEAS_TIMES;

/**
 * @brief RGB gain choices
 */
typedef enum {
    RGB_GAIN_1X = BH1749_MODE_CONTROL1_RGB_GAIN_1X,
    RGB_GAIN_32X = BH1749_MODE_CONTROL1_RGB_GAIN_32X
} RGB_GAINS;

/**
 * @brief IR gain choices
 */
typedef enum {
    IR_GAIN_1X = BH1749_MODE_CONTROL1_IR_GAIN_1X,
    IR_GAIN_32X = BH1749_MODE_CONTROL1_IR_GAIN_32X
} IR_GAINS;

/**
 * @brief Interrupt source choices
 */
typedef enum {
    RED = BH1749_INTERRUPT_SOURCE_SELECT_RED,
    GREEN = BH1749_INTERRUPT_SOURCE_SELECT_GREEN,
    BLUE = BH1749_INTERRUPT_SOURCE_SELECT_BLUE
} INT_SOURCES;

/**
 * @brief The full sensor context
 */
typedef struct _bh1749_context
{
    mraa_i2c_context i2c;
    mraa_gpio_context interrupt;
    bool enabled;
    bool isrEnabled;
    uint16_t int_thh;
    uint16_t int_thl;
    INT_SOURCES int_src;
    IR_GAINS ir_gain;
    RGB_GAINS rgb_gain;
    MEAS_TIMES meas_time;
    OPERATING_MODES operating_mode;
} *bh1749_context;

/**
 * @brief Check "who am I" register value to identify the sensor
 *
 * @param dev Sensor context
 * @return UPM result
 */
upm_result_t bh1749_check_who_am_i(bh1749_context dev);

/**
 * @brief Init the sensor with specific bus and address. This function calls
 * the sensor_init() function to set default values for operating mode, gains,
 * measurement time, interrupt source and then sets threshold high to 511.
 *
 * @param bus I2C bus number
 * @param addr I2C sensor address
 * @return context of initialized sensor
 */
bh1749_context bh1749_init(int bus, int addr);

/**
 * @brief Close and free sensor context
 *
 * @param dev Sensor context
 */
void bh1749_close(bh1749_context dev);

/**
 * @brief Enables RGB color measurement on the sensor
 *
 * @param dev Sensor context
 * @return UPM result
 */
upm_result_t bh1749_enable(bh1749_context dev);

/**
 * @brief Disables RGB color measurement on the sensor
 *
 * @param dev Sensor context
 * @return UPM result
 */
upm_result_t bh1749_disable(bh1749_context dev);

/**
 * @brief Initializes (writes) configuration values to sensor
 *
 * @param dev Sensor context
 * @param opMode Operating mode choice, a value of OPERATING_MODES enum
 * @param measTime Measurement time choice, a value of MEAS_TIMES enum
 * @param rgbGain RGB gain choice, a value of RGB_GAINS enum
 * @param irGain IR gain choice, a value of IR_GAINS enum
 * @param intSource interrupt source choice, a value of INT_SOURCES enum
 * @return UPM result
 */
upm_result_t bh1749_sensor_init(bh1749_context dev, OPERATING_MODES opMode,
                        MEAS_TIMES measTime,
                        RGB_GAINS rgbGain,
                        IR_GAINS irGain,
                        INT_SOURCES intSource);

/**
 * @brief Sets operating mode (interrupt persistance)
 *
 * @param dev Sensor context
 * @param opMode Operating mode choice, a value of OPERATING_MODES enum
 * @return UPM result
 */
upm_result_t bh1749_set_operating_mode(bh1749_context dev, OPERATING_MODES opMode);

/**
 * @brief Gets operating mode (interrupt persistance) value
 *
 * @param dev Sensor context
 * @param opMode pointer (uint8_t) to save value
 * @return UPM result
 */
upm_result_t bh1749_get_operating_mode(bh1749_context dev, uint8_t *opMode);

/**
 * @brief Sets measurement time (ODR)
 *
 * @param dev Sensor context
 * @param measTime measurement time choice, a value of MEAS_TIMES enum
 * @return UPM result
 */
upm_result_t bh1749_set_measurement_time(bh1749_context dev, MEAS_TIMES measTime);

/**
 * @brief Sets measurement time (ODR)
 *
 * @param dev Sensor context
 * @param meas_time pointer (uint8_t) to save value
 * @return UPM result
 */
upm_result_t bh1749_get_measurement_time(bh1749_context dev, uint8_t *meas_time);

/**
 * @brief Sets RGB gain values
 *
 * @param dev Sensor context
 * @param rgbGain RGB gain choice, a value of RGB_GAINS enum
 * @return UPM result
 */
upm_result_t bh1749_set_rgb_gain(bh1749_context dev, RGB_GAINS rgbGain);

/**
 * @brief Gets RGB gain value
 *
 * @param dev Sensor context
 * @param gain pointer (uint8_t) to save value
 * @return UPM result
 */
upm_result_t bh1749_get_rgb_gain(bh1749_context dev, uint8_t *gain);

/**
 * @brief Sets IR gain values
 *
 * @param dev Sensor context
 * @param irGain IR gain choice, a value of IR_GAINS enum
 * @return UPM result
 */
upm_result_t bh1749_set_ir_gain(bh1749_context dev, IR_GAINS irGain);

/**
 * @brief Gets IR gain value
 *
 * @param dev Sensor context
 * @param gain pointer (uint8_t) to save value
 * @return UPM result
 */
upm_result_t bh1749_get_ir_gain(bh1749_context dev, uint8_t *gain);

/**
 * @brief Sets interrupt source value
 *
 * @param dev Sensor context
 * @param intSource interrupt source choice, a value of INT_SOURCES enum
 * @return UPM result
 */
upm_result_t bh1749_set_int_source(bh1749_context dev, INT_SOURCES intSource);

/**
 * @brief Gets interrupt source value
 *
 * @param dev Sensor context
 * @return character of interrupt source
 */
char bh1749_get_interrupt_source_char(bh1749_context dev);

/**
 * @brief Enables interrupt mode and resets the interrupt status (clear)
 *
 * @param dev Sensor context
 * @return UPM result
 */
upm_result_t bh1749_enable_interrupt(bh1749_context dev);

/**
 * @brief Disables interrupt mode
 *
 * @param dev Sensor context
 * @return UPM result
 */
upm_result_t bh1749_disable_interrupt(bh1749_context dev);

/**
 * @brief Resets interrupt status (clear) to allow new interrupts
 *
 * @param dev Sensor context
 * @return UPM result
 */
upm_result_t bh1749_reset_interrupt(bh1749_context dev);

/**
 * @brief Checks the status of the interrupt
 *
 * @param dev Sensor context
 * @return true if there is interrupt, otherwise false
 */
bool bh1749_is_interrupted(bh1749_context dev);

/**
 * @brief Checks whether interrupt mode is enabled
 *
 * @param dev Sensor context
 * @return true if interrupt is enabled, otherwise false
 */
bool bh1749_is_interrupt_enabled(bh1749_context dev);

/**
 * @brief Initiates a software reset to the sensor. All register values will
 * be written to their defaults, thus sensor_init() must be called after this,
 * and thresholds also needs to be set.
 *
 * @param dev Sensor context
 * @return UPM result
 */
upm_result_t bh1749_soft_reset(bh1749_context dev);

/**
 * @brief Sets interrupt threshold high value
 *
 * @param dev Sensor context
 * @param threshold Value to be written, range 0-65536
 * @return UPM result
 */
upm_result_t bh1749_set_threshold_high(bh1749_context dev, uint16_t threshold);

/**
 * @brief Gets interrupt threshold high value
 *
 * @param dev Sensor context
 * @param threshold Pointer (uint16_t) to write value
 * @return UPM result
 */
upm_result_t bh1749_get_threshold_high(bh1749_context dev, uint16_t *threshold);

/**
 * @brief Sets interrupt threshold low value
 *
 * @param dev Sensor context
 * @param threshold Value to be written, range 0-65536
 * @return UPM result
 */
upm_result_t bh1749_set_threshold_low(bh1749_context dev, uint16_t threshold);

/**
 * @brief Gets interrupt threshold low value
 *
 * @param dev Sensor context
 * @param threshold Pointer (uint16_t) to write value
 * @return UPM result
 */
upm_result_t bh1749_get_threshold_low(bh1749_context dev, uint16_t *threshold);

/**
 * @brief Gets value of Red color channel
 *
 * @param dev Sensor context
 * @param red Pointer (uint16_t) to write value
 * @return UPM result
 */
upm_result_t bh1749_get_red(bh1749_context dev, uint16_t *red);

/**
 * @brief Gets value of Green color channel
 *
 * @param dev Sensor context
 * @param green Pointer (uint16_t) to write value
 * @return UPM result
 */
upm_result_t bh1749_get_green(bh1749_context dev, uint16_t *green);

/**
 * @brief Gets value of Blue color channel
 *
 * @param dev Sensor context
 * @param blue Pointer (uint16_t) to write value
 * @return UPM result
 */
upm_result_t bh1749_get_blue(bh1749_context dev, uint16_t *blue);

/**
 * @brief Gets value of IR color channel
 *
 * @param dev Sensor context
 * @param ir Pointer (uint16_t) to write value
 * @return UPM result
 */
upm_result_t bh1749_get_ir(bh1749_context dev, uint16_t *ir);

/**
 * @brief Gets value of Green2 color channel
 *
 * @param dev Sensor context
 * @param green Pointer (uint16_t) to write value
 * @return UPM result
 */
upm_result_t bh1749_get_green2(bh1749_context dev, uint16_t *green2);

/**
 * @brief Gets all channels measurements values
 *
 * @param dev Sensor context
 * @param result Pointer of uint16_t to write all values ordered as:
 * Red, Green, Blue, IR, Green2
 *
 * @return UPM result
 */
upm_result_t bh1749_get_measurements(bh1749_context dev, uint16_t *result);

/**
 * @brief Installs the ISR to a given GPIO pin
 *
 * @param dev Sensor context
 * @param edge Edge type to raise ISR with, of type mraa_gpio_edge_t
 * @param pin GPIO pin number
 * @param isr Pointer to ISR function
 * @param isr_args Arguments to pass the ISR function
 * @return UPM result
 */
upm_result_t bh1749_install_isr(bh1749_context dev, mraa_gpio_edge_t edge, int pin,
                        void (*isr)(void *), void *isr_args);

/**
 * @brief Removes the ISR if it is installed
 *
 * @param dev Sensor context
 */
void bh1749_remove_isr(bh1749_context dev);

/**
 * @brief Gets a dump of configuration registers as a string
 *
 * @param dev Sensor context
 * @param dump Pointer of char to save dump string
 * @return UPM result
 */
upm_result_t bh1749_registers_dump(bh1749_context dev, char *dump);

#ifdef __cplusplus
}
#endif
