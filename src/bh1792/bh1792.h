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
#include <pthread.h>
#include "bh1792glc_registers.h"

/**
 * @file bh1792.h
 * @library bh1792
 * @brief C API for the bh1792 driver
 *
 * @include bh1792.c
 */

#define ONE_SEC_IN_MIRCO_SEC 1000000
#define FIFO_WATERMARK 32
#define LED_CURRENT_MAX 63

/**
 * @brief Measurement modes, for synchronized time modes, non synchronized
 * and single modes.
 */
typedef enum {
    MSR_64Hz = BH1792_MEAS_CONTROL1_MSR_64HZ,
    MSR_32Hz = BH1792_MEAS_CONTROL1_MSR_32HZ,
    MSR_128Hz = BH1792_MEAS_CONTROL1_MSR_128HZ,
    MSR_256Hz = BH1792_MEAS_CONTROL1_MSR_256HZ,
    MSR_1024Hz = BH1792_MEAS_CONTROL1_MSR_1024HZ,
    MSR_NON_SYNC = BH1792_MEAS_CONTROL1_MSR_NON_SYNCH_MODE,
    MSR_SINGLE = BH1792_MEAS_CONTROL1_MSR_SINGLE_MEAS_MODE
} MEAS_MODES;

/**
 * @brief LED light types
 */
typedef enum {
    GREEN,
    IR
} LED_TYPES;

/**
 * @brief Interrupt modes of the sensor
 */
typedef enum {
    WATER_MARK = BH1792_MEAS_CONTROL5_INT_SEL_FIFO_WATERMARK,
    IR_THRESHOLD_JUDGE = BH1792_MEAS_CONTROL5_INT_SEL_IR_THRESHOLD,
    ON_COMPLETE = BH1792_MEAS_CONTROL5_INT_SEL_ON_COMPLETE
} INTERRUPT_MODES;

/**
 * @brief Operations modes available with the sensor
 */
typedef enum {
    SYNCHRONIZED,
    NON_SYNCHRONIZED,
    SINGLE_GREEN,
    SINGLE_IR
} OP_MODES;

/**
 * @brief bh1792 sensor context
 */
typedef struct _bh1792_context
{
    mraa_i2c_context i2c;
    mraa_gpio_context interrupt;
    bool enabled;
    bool isrEnabled;
    OP_MODES op_mode;
    pthread_t sync_thread;
    bool sync_thread_alive;
    LED_TYPES led_type;
    INTERRUPT_MODES interrupt_mode;
    uint16_t meas_freq;
    uint8_t green_current;
    uint8_t ir_current;
    uint16_t threshold;
} *bh1792_context;

/**
 * @brief Init the sensor with specific bus and address. Before starting
 * measurement, one of (bh1792_enable_sync_mode, bh1792_enable_non_sync_mode,
 * or bh1792_enable_single_mode) have to be called.
 *
 * @param bus I2C bus number
 * @param addr I2C sensor address
 * @return context of initialized sensor
 */
bh1792_context bh1792_init(int bus, int addr);

/**
 * @brief Close and free sensor context
 *
 * @param dev Sensor context
 */
void bh1792_close(bh1792_context dev);

/**
 * @brief Check "who am I" register value to identify the sensor
 *
 * @param dev Sensor context
 * @return UPM result
 */
upm_result_t bh1792_check_who_am_i(bh1792_context dev);

/**
 * @brief Checks if the measuremnt on sensor is enabled
 *
 * @param The sensor context
 * @return True if measurement is enabled, or false otherwise
 */
bool bh1792_is_enabled(bh1792_context dev);

/**
 * @brief Initiates a software reset to the sensor. All register values will
 * be written to their defaults, thus intended operation mode need to be
 * enabled.
 *
 * @param dev Sensor context
 * @return UPM result
 */
upm_result_t bh1792_soft_reset(bh1792_context dev);

/**
 * @brief Gets the value of measurement time is milli-seconds
 *
 * @param dev The sensor context
 * @param meas_time_ms Float pointer to store value of measurement time
 * @return UPM result
 */
upm_result_t bh1792_get_meas_time_ms(bh1792_context dev, float *meas_time);

/**
 * @brief Sets the green LED Current value
 *
 * @param dev The sensor context
 * @param current The current value, accepted values are between 0-63
 * @return UPM result
 */
upm_result_t bh1792_set_green_leds_current(bh1792_context dev, uint8_t current);

/**
 * @brief Gets the green LED current value
 *
 * @param dev The sensor context
 * @param current Pointer to store the read current value
 * @return UPM result
 */
upm_result_t bh1792_get_green_leds_current(bh1792_context dev, uint8_t *current);

/**
 * @brief Sets the IR LED Current value
 *
 * @param dev The sensor context
 * @param current The current value, accepted values are between 0-63
 * @return UPM result
 */
upm_result_t bh1792_set_ir_led_current(bh1792_context dev, uint8_t current);

/**
 * @brief Gets the IR LED current value
 *
 * @param dev The sensor context
 * @param current Pointer to store the read current value
 * @return UPM result
 */
upm_result_t bh1792_get_ir_led_current(bh1792_context dev, uint8_t *current);

/**
 * @brief Sets interrupt ir threshold value for non-synchronized mode
 *
 * @param dev Sensor context
 * @param threshold Value to be written, range 0-65536
 * @return UPM result
 */
upm_result_t bh1792_set_ir_threshold(bh1792_context dev, uint16_t threshold);

/**
 * @brief Gets interrupt ir threshold value
 *
 * @param dev Sensor context
 * @param threshold Pointer (uint16_t) to write value
 * @return UPM result
 */
upm_result_t bh1792_get_ir_threshold(bh1792_context dev, uint16_t *threshold);

/**
 * @brief Gets the current size of built-in FIFO
 *
 * @param dev The sensor context
 * @param count The pointer to store the size
 * @return UPM result
 */
upm_result_t bh1792_get_fifo_size(bh1792_context dev, uint8_t *count);

/**
 * @brief Disables interrupt function. Interrupt is enabled by default on all
 * operation modes, so this have to be called after enabling a specific
 * operation mode
 *
 * @param dev The sensor context
 * @return UPM result
 */
upm_result_t bh1792_disable_interrupt(bh1792_context dev);

/**
 * @brief Clears the interrupt flag allowing more interrupts to be raised
 *
 * @param dev The sensor context
 * @return UPM result
 */
upm_result_t bh1792_clear_interrupt(bh1792_context dev);

/**
 * @brief Starts the measuremnt on sensor or enable the sensor
 *
 * @param dev The sensor context
 * @return UPM result
 */
upm_result_t bh1792_start_measurement(bh1792_context dev);

/**
 * @breif Stops the measurement by setting the software reset bit. This
 * function will reset all config registers. To resume from previous
 * configuration call bh1792_restart_measurement().
 *
 * @param dev The sensor context
 * @return UPM result
 */
upm_result_t bh1792_stop_measurement(bh1792_context dev);

/**
 * @brief Restart the measurement, This function stops measurement, then
 * re-writes the configuration registers again and start measurement with same
 * configuration.
 *
 * @param dev The sensor context
 * @return UPM result
 */
upm_result_t bh1792_restart_measurement(bh1792_context dev);

/**
 * @brief Gets the fifo data of 32 entries and resets the WATERMARK interrupt
 * flag.
 *
 * @param dev The sensor context
 * @param fifo_led_off Array/Pointer to store 32 entries of fifo LED_OFF data
 * @param fifo_led_on Array/Pointer to store 32 entries of fifo LED_ON data
 * @return UPM result
 */
upm_result_t bh1792_get_fifo_data(bh1792_context dev, uint16_t fifo_led_off[],
                                    uint16_t fifo_led_on[]);

/**
 * @brief Reads and discard the 32 FIFO entries to reset the WATERMARK
 * interrupt flag.
 *
 * @param dev The sensor context
 * @return UPM result
 */
upm_result_t bh1792_discard_fifo_data(bh1792_context dev);

/**
 * @brief Gets the values from using green LED light
 *
 * @param dev The sensor context
 * @param green_led_off Pointer to store the reading value of LED_OFF
 * @param green_led_on Pointer to store the reading value of LED_ON
 * @return UPM result
 */
upm_result_t bh1792_get_green_data(bh1792_context dev, uint16_t *green_led_off,
                                    uint16_t *green_led_on);

/**
 * @brief Gets the values from using IR LED light
 *
 * @param dev The sensor context
 * @param ir_led_off Pointer to store the reading value of LED_OFF
 * @param ir_led_on Pointer to store the reading value of LED_ON
 * @return UPM result
 */
upm_result_t bh1792_get_ir_data(bh1792_context dev, uint16_t *ir_led_off,
                                uint16_t *ir_led_on);

/**
 * @brief Enables and configures the sensor to use synchronized mode. This will
 * use the built-in FIFO and raises a WATERMARK interrupt when FIFO size
 * reaches 32 entries. To clear that interrupt flag FIFO must be read or
 * discarded. This mode uses only green LED.
 *
 * You need to call bh1792_start_measurement() after this.
 *
 * @param dev The sensor context
 * @param meas_freq Measurement frequncy mode, valid values 32, 64, 128, 256,
 * 1024
 * @param green_current Green LED current value
 * @return UPM result
 */
upm_result_t bh1792_enable_sync_mode(bh1792_context dev, uint16_t meas_freq,
                                    uint8_t green_current);

/**
 * @brief Enabled and configures the sensor to use non-synchronized mode. This
 * mode uses IR_THRESHOLD interrupt, it will raise an interrupt if IR LED ON
 * value is equal or greater than the threshold value. Interrupt must be
 * cleared before receiving new interrupts. This mode uses only IR LED.
 *
 * You need to call bh1792_start_measurement() after this.
 *
 * @param dev The sensor context
 * @param ir_current The IR LED current value
 * @param threshold The IR LED interrupt threshold value
 * @return UPM result
 */
upm_result_t bh1792_enable_non_sync_mode(bh1792_context dev, uint8_t ir_current,
                                        uint16_t threshold);

/**
 * @brief Enables and configures the sensor to use single mode. This sets the
 * sensor to take only one measurement and raise an interrupt after measurement
 * is finished. Interrupt flags has to be cleared to recieved new interrupts.
 * This mode uses both green and IR LEDs.
 *
 * You need to call bh1792_start_measurement() after this. If you need to get
 * continuous values, clear interrupt and bh1792_start_measurement()
 * continuously.
 *
 * @param dev The sensor context
 * @param led_type LED light type to use, takes GREEN or IR values
 * @param current The chosen led_type current value
 * @return UPM result
 */
upm_result_t bh1792_enable_single_mode(bh1792_context dev, LED_TYPES led_type,
                                        uint8_t current);

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
upm_result_t bh1792_install_isr(bh1792_context dev, mraa_gpio_edge_t edge,
                                int pin, void (*isr)(void *), void *isr_args);

/**
 * @brief Installs the ISR to a given GPIO pin
 *
 * @param dev Sensor context
 * @param pin GPIO pin number
 * @param isr Pointer to ISR function
 * @param isr_args Arguments to pass the ISR function
 * @return UPM result
 */
upm_result_t bh1792_install_isr_falling_edge(bh1792_context dev, int pin,
                                void (*isr)(void *), void *isr_args);

/**
 * @brief Removes the ISR if it is installed
 *
 * @param dev Sensor context
 */
void bh1792_remove_isr(bh1792_context dev);

/**
 * @brief Gets a dump of configuration registers as a string
 *
 * @param dev Sensor context
 * @param dump Pointer of char to save dump string
 * @return UPM result
 */
upm_result_t bh1792_registers_dump(bh1792_context dev, char *dump);

#ifdef __cplusplus
}
#endif
