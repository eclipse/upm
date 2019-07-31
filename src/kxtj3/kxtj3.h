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

#include <assert.h>
#include <unistd.h>

#include <mraa/i2c.h>
#include <mraa/gpio.h>
#include <upm.h>

#include "kxtj3_registers.h"

  /**
 * @brief C API for the kxtj3 driver
 * @defgroup kxtj3 libupm-kxtj3
 * @ingroup Kionix i2c accelerometer
 *
 * @include kxtj3.c
 */

  /**
 * @library kxtj3
 * @sensor kxtj3
 * @comname tri-axis accelerometer
 * @type accelerometer
 * @man Kionix
 * @con i2c
 *
 * @brief C API for the kxtj3 driver
 *
 * @image html kxtj3.png
 */

#define SECOND_IN_MICRO_S 1000000

  /**
 * @brief Resolution types
 */
  typedef enum
  {
    LOW_RES,
    HIGH_RES
  } KXTJ3_RESOLUTION_T;

  /**
 * @brief Interrupt polarity types
 */
  typedef enum
  {
    ACTIVE_LOW,
    ACTIVE_HIGH

  } KXTJ3_INTERRUPT_POLARITY_T;

  /**
 * @brief Interrupt response types
 */
  typedef enum
  {
    LATCH_UNTIL_CLEARED,
    TRANSMIT_ONE_PULSE
  } KXTJ3_INTERRUPT_RESPONSE_T;

  /**
 * @brief Interrupt sources
 */
  typedef enum
  {
    NO_INTERRUPT,
    KXTJ3_DATA_READY_INTERRUPT = KXTJ3_INT_SOURCE1_DRDY,
    KXTJ3_WAKEUP_INTERRUPT = KXTJ3_INT_SOURCE1_WUFS,
    KXTJ3_DATA_READY_AND_WAKEUP_INT = KXTJ3_INT_SOURCE1_DRDY | KXTJ3_INT_SOURCE1_WUFS
  } KXTJ3_INTERRUPT_SOURCE_T;

  /**
 * @brief Acceleration ranges
 */
  typedef enum
  {
    KXTJ3_RANGE_2G = KXTJ3_CTRL_REG1_GSEL_2G,
    KXTJ3_RANGE_4G = KXTJ3_CTRL_REG1_GSEL_4G,
    KXTJ3_RANGE_8G = KXTJ3_CTRL_REG1_GSEL_8G,
    KXTJ3_RANGE_8G_14 = KXTJ3_CTRL_REG1_GSEL_8G_14,
    KXTJ3_RANGE_16G = KXTJ3_CTRL_REG1_GSEL_16G,
    KXTJ3_RANGE_16G_2 = KXTJ3_CTRL_REG1_GSEL_16G2,
    KXTJ3_RANGE_16G_3 = KXTJ3_CTRL_REG1_GSEL_16G3,
    KXTJ3_RANGE_16G_14 = KXTJ3_CTRL_REG1_GSEL_16G_14
  } KXTJ3_G_RANGE_T;

  /**
 * @brief Output Data Rates for normal mode
 */
  typedef enum
  {
    KXTJ3_ODR_0P781 = KXTJ3_DATA_CTRL_REG_OSA_0P781,
    KXTJ3_ODR_1P563 = KXTJ3_DATA_CTRL_REG_OSA_1P563,
    KXTJ3_ODR_3P125 = KXTJ3_DATA_CTRL_REG_OSA_3P125,
    KXTJ3_ODR_6P25 = KXTJ3_DATA_CTRL_REG_OSA_6P25,
    KXTJ3_ODR_12P5 = KXTJ3_DATA_CTRL_REG_OSA_12P5,
    KXTJ3_ODR_25 = KXTJ3_DATA_CTRL_REG_OSA_25,
    KXTJ3_ODR_50 = KXTJ3_DATA_CTRL_REG_OSA_50,
    KXTJ3_ODR_100 = KXTJ3_DATA_CTRL_REG_OSA_100,
    KXTJ3_ODR_200 = KXTJ3_DATA_CTRL_REG_OSA_200,
    KXTJ3_ODR_400 = KXTJ3_DATA_CTRL_REG_OSA_400,
    KXTJ3_ODR_800 = KXTJ3_DATA_CTRL_REG_OSA_800,
    KXTJ3_ODR_1600 = KXTJ3_DATA_CTRL_REG_OSA_1600
  } KXTJ3_ODR_T;

  /**
 * @brief Output Data Rates for wake-up function
 */
  typedef enum
  {
    KXTJ3_ODR_WAKEUP_0P781 = KXTJ3_CTRL_REG2_OWUF_0P781,
    KXTJ3_ODR_WAKEUP_1P563 = KXTJ3_CTRL_REG2_OWUF_1P563,
    KXTJ3_ODR_WAKEUP_3P125 = KXTJ3_CTRL_REG2_OWUF_3P125,
    KXTJ3_ODR_WAKEUP_6P25 = KXTJ3_CTRL_REG2_OWUF_6P25,
    KXTJ3_ODR_WAKEUP_12P5 = KXTJ3_CTRL_REG2_OWUF_12P5,
    KXTJ3_ODR_WAKEUP_25 = KXTJ3_CTRL_REG2_OWUF_25,
    KXTJ3_ODR_WAKEUP_50 = KXTJ3_CTRL_REG2_OWUF_50,
    KXTJ3_ODR_WAKEUP_100 = KXTJ3_CTRL_REG2_OWUF_100
  } KXTJ3_ODR_WAKEUP_T;

  /**
 * @brief Wake-up source axis and direction
 */
  typedef enum
  {
    X_NEGATIVE = KXTJ3_INT_CTRL_REG2_XNWU,
    X_POSITIVE = KXTJ3_INT_CTRL_REG2_XPWU,
    Y_NEGATIVE = KXTJ3_INT_CTRL_REG2_YNWU,
    Y_POSITIVE = KXTJ3_INT_CTRL_REG2_YPWU,
    Z_NEGATIVE = KXTJ3_INT_CTRL_REG2_ZNWU,
    Z_POSITIVE = KXTJ3_INT_CTRL_REG2_ZPWU
  } KXTJ3_WAKEUP_SOURCE_T;

  /**
 * @brief Use it to get axis and direction for wake-up function
 */
  typedef struct
  {
    bool X_NEGATIVE,
        X_POSITIVE,
        Y_NEGATIVE,
        Y_POSITIVE,
        Z_NEGATIVE,
        Z_POSITIVE;
  } kxtj3_wakeup_axes;

  /**
 * @brief Sensor context
 */
  typedef struct _kxtj3_context
  {
    mraa_i2c_context i2c;
    KXTJ3_RESOLUTION_T res_mode;
    KXTJ3_G_RANGE_T g_range_mode;
    float acceleration_scale;
    KXTJ3_ODR_T odr;
    float odr_in_sec;
    KXTJ3_ODR_WAKEUP_T odr_wakeup;
    float odr_in_sec_wakeup;
    mraa_gpio_context interrupt_pin;
  } * kxtj3_context;

  /**
@brief KXTJ3 initialization. Sets sensor default values and put it to active state.

@param bus I2C bus to use
@param addr I2C address of the sensor
@return The sensor context, or NULL if an error occurs
*/
  kxtj3_context kxtj3_init(int bus, uint8_t addr);

  /**
@brief Initializes the sensor with the given resolution and acceleration range

This gets called during the kxtj3_init(), so it will not need to be called unless the sensor is reset

Sensor is automatically set into standby mode during the initialization
Sensor is set to active mode after initialization

Be cautious not to set resolution to LOW_RES along with g_range as 14-bits modes

@param dev The sensor context
@param odr Output Data Rate, One of the KXTJ3_ODR_T values
@param res Resolution mode, One of the KXTJ3_RESOLUTION_T values. LOW_RES valid only for ODR <= 200 Hz.
@param g_range Acceleration range, One of the KXTJ3_G_RANGE_T values
@return UPM result
*/
  upm_result_t kxtj3_sensor_init(const kxtj3_context dev, KXTJ3_ODR_T odr, KXTJ3_RESOLUTION_T res, KXTJ3_G_RANGE_T g_range);

  /**
@brief Gets "who am I" value of the sensor

@param dev The sensor context
@param data Pointer to a uint8_t variable to store the value
@return UPM result
*/
  upm_result_t kxtj3_get_who_am_i(const kxtj3_context dev, uint8_t *data);

  /**
@brief KXTJ3 destructor
Closes the I2C context, and removes interrupts
Frees memory of the kxtj3_context

@param dev The sensor context
*/
  void kxtj3_close(kxtj3_context dev);

  /**
@brief Sets the sensor to active mode

@param dev The sensor context
@return UPM result
*/
  upm_result_t kxtj3_set_sensor_active(const kxtj3_context dev);

  /**
@brief Sets the sensor to the standby mode

@param dev The sensor context
@return UPM result
*/
  upm_result_t kxtj3_set_sensor_standby(const kxtj3_context dev);

  /**
@brief Sets the acceleration range of the sensor

Sensor needs to be in standby mode when setting the acceleration range value

Be cautious not to set g_range to 14-bits modes with the resolution being on LOW_RES.
If the acceleration range is not compatible with resolution mode, the resolution is set automatically
to the compatible value.

@param dev The sensor context
@param g_range One of the KXTJ3_G_RANGE_T values
@return UPM result
*/
  upm_result_t kxtj3_set_g_range(const kxtj3_context dev, KXTJ3_G_RANGE_T g_range);

  /**
@brief Sets the resolution of the sensor. High resolution (14 bits and 12 bits) or low resolution (8 bits).

LOW_RES valid only for ODR <= 200 Hz

Be cautious not to set resolution to LOW_RES with the G_RANG being on 14-bits modes.
If the resolution mode is not compatible with acceleration range, the resolution is set automatically
to the compatible value.

Sensor needs to be in standby mode when setting the sensor resolution

@param dev The sensor context
@param resolution One of the KXTJ3_RESOLUTION_T values
@return UPM result
*/
  upm_result_t kxtj3_set_resolution(const kxtj3_context dev, KXTJ3_RESOLUTION_T resolution);

  /**
@brief Sets the ODR of the sensor

Sensor needs to be in standby mode when setting the ODR

@param dev The sensor context
@param odr One of the KXTJ3_ODR_T values
@return UPM result
*/
  upm_result_t kxtj3_set_odr(const kxtj3_context dev, KXTJ3_ODR_T odr);

  /**
@brief Sets the ODR of the wakeup function of the sensor

Sensor needs to be in standby mode when setting the ODR

@param dev The sensor context
@param odr One of the KXTJ3_ODR_WAKEUP_T values
@return UPM result
*/
  upm_result_t kxtj3_set_odr_wakeup_function(const kxtj3_context dev, KXTJ3_ODR_WAKEUP_T odr);

  /**
@brief Performs a self-test for digital communication of the sensor. The test sets DCST bit in
CTRL_REG2, then checks the DCST_RESP register for a 0xAA, after the register is read, its
value is 0x55 and DCST bit is cleared.

This function is called by kxtj3_sensor_self_test also

Sensor must be in standby mode before performing this action

@param dev The sensor context
@return UPM result
*/
  upm_result_t kxtj3_self_test_digital_communication(kxtj3_context dev);

  /**
@brief Performs a self-test of the sensor. The test applies an electrostatic force to the sensor,
simulating input acceleration. The test compares samples from all axis before and after
applying the electrostatic force to the sensor. If the amount of acceleration increases according
to the values defined in TABLE 1 of the datasheet (0.5 g), the test passes.

The function prints out the values before and during test and the average difference for each axis

See the datasheet for more information

@param dev The sensor context
@return UPM result
*/
  upm_result_t kxtj3_sensor_self_test(const kxtj3_context dev);

  /**
@brief Performs a sensor software reset. The software reset clears the RAM of the sensor and resets all registers
to pre-defined values.

You should call kxtj3_sensor_init() after the software reset

See the datasheet for more details

@param dev The sensor context
@return UPM result
*/
  upm_result_t kxtj3_sensor_software_reset(const kxtj3_context dev);

  /**
@brief Gets raw accelerometer data from the sensor

@param dev The sensor context
@param x Pointer to a floating point variable to store the x-axis value. Set to NULL if not wanted.
@param y Pointer to a floating point variable to store the y-axis value. Set to NULL if not wanted.
@param z Pointer to a floating point variable to store the z-axis value. Set to NULL if not wanted.
@return UPM result
*/
  upm_result_t kxtj3_get_acceleration_data_raw(const kxtj3_context dev, float *x, float *y, float *z);

  /**
@brief Gets converted (m/s^2) accelerometer data from the sensor

@param dev The sensor context
@param x Pointer to a floating point variable to store the x-axis value. Set to NULL if not wanted.
@param y Pointer to a floating point variable to store the y-axis value. Set to NULL if not wanted.
@param z Pointer to a floating point variable to store the z-axis value. Set to NULL if not wanted.
*/
  upm_result_t kxtj3_get_acceleration_data(const kxtj3_context dev, float *x, float *y, float *z);

  /**
@brief Gets the duration of one sample period (in seconds) for getting the acceleration data depending on
the sampling rate of the sensor

@param dev The sensor context
@return Floating point value of the sampling period
*/
  float kxtj3_get_acceleration_sampling_period(kxtj3_context dev);

  /**
Gets the duration of one sample period (in seconds) for the wakeup function depending on
the sampling rate of the sensor wakeup function

@param dev The sensor context
@return Floating point value of the sampling period
*/
  float kxtj3_get_wakeup_sampling_period(kxtj3_context dev);

  /**
@brief Enables the data ready interrupt. Availability of new acceleration data is
reflected as an interrupt.

Sensor must be in standby mode before performing this action

@param dev The sensor context
@return UPM result
 */
  upm_result_t kxtj3_enable_data_ready_interrupt(const kxtj3_context dev);

  /**
@brief Disables the data ready interrupt

Sensor must be in standby mode before performing this action

@param dev The sensor context
@return UPM result
*/
  upm_result_t kxtj3_disable_data_ready_interrupt(const kxtj3_context dev);

  /**
@brief Enables the wakeup function (motion detection)

Sensor must be in standby mode before performing this action

@param dev The sensor context
@return UPM result
 */
  upm_result_t kxtj3_enable_wakeup_interrupt(const kxtj3_context dev);

  /**
@brief Disables the wakeup function (motion detection)

Sensor must be in standby mode before performing this action

@param dev The sensor context
@return UPM result
 */
  upm_result_t kxtj3_disable_wakeup_interrupt(const kxtj3_context dev);

  /**
@brief Enables interrupts on the interrupt pin, sets polarity and response types

Polarity ACTIVE_HIGH or ACTIVE_LOW

Response either latch until cleared by reading INT_REL register, or transmit one pulse
Pulse width of 0.03-0.05ms

For Wakeup function, the response type is always latched unless set wakeup latch off

Sensor needs to be in standby mode when enabling the interrupt

See datasheet for more details

@param dev The sensor context
@param polarity Select the polarity of the interrupt. One of the KXTJ3_INTERRUPT_POLARITY_T values.
@param response_type Select the response type of the interrupt. One of the KXTJ3_INTERRUPT_RESPONSE_T values.
@return UPM result
*/
  upm_result_t kxtj3_enable_interrupt_pin(const kxtj3_context dev, KXTJ3_INTERRUPT_POLARITY_T polarity, KXTJ3_INTERRUPT_RESPONSE_T response_type);

  /**
@brief Disables interrupts on the interrupt pin of the sensor

Sensor needs to be in standby mode when disabling the interrupt pin

@param dev The sensor context
@return UPM result
*/
  upm_result_t kxtj3_disable_interrupt_pin(const kxtj3_context dev);

  /**
@brief Sets the polarity of the interrupt pin

Polarity ACTIVE_HIGH or ACTIVE_LOW

Sensor must be in standby mode before performing this action

@param dev The sensor context
@param polarity Select the polarity of the interrupt. One of the KXTJ3_INTERRUPT_POLARITY_T values.
@return UPM result
*/
  upm_result_t kxtj3_set_interrupt_polarity(const kxtj3_context dev, KXTJ3_INTERRUPT_POLARITY_T polarity);

  /**
@brief Sets the response type of the interrupt pin

Response either latch until cleared by reading INT_REL register, or transmit one pulse
Pulse width of 0.03-0.05ms

For Wakeup function, the response type is always latched unless set wakeup latch off

Sensor must be in standby mode before performing this action

See datasheet for more details

@param dev The sensor context
@param response_type Select the response type of the interrupt. One of the KXTJ3_INTERRUPT_RESPONSE_T values.
@return UPM result
*/
  upm_result_t kxtj3_set_interrupt_response(const kxtj3_context dev, KXTJ3_INTERRUPT_RESPONSE_T response_type);

  /**
@brief Gets the status of the interrupts. (Has an interrupt occurred).

See datasheet for more details

@param dev The sensor context
@return Return true if an interrupt event has occurred (DRDY or WUFS is 1), returns false if no interrupts have occurred
*/
  bool kxtj3_get_interrupt_status(const kxtj3_context dev);

  /**
@brief Gets the source of the interrupt
The value stored is one or more of the KXTJ3_INTERRUPT_SOURCE_T values

See datasheet for more details

@param dev The sensor context
@param reg_value Pointer to a uint8_t variable to store the value
@return UPM result
*/
  upm_result_t kxtj3_read_interrupt_source1_reg(const kxtj3_context dev, uint8_t *reg_value);

  /**
@brief Gets the source of the interrupt

See datasheet for more details

@param dev The sensor context
@return One of the KXTJ3_INTERRUPT_SOURCE_T values/types
*/
  KXTJ3_INTERRUPT_SOURCE_T kxtj3_get_interrupt_source(const kxtj3_context dev);

  /**
@brief Installs an interrupt handler to be executed when an interrupt is detected on the interrupt pin

@param dev The sensor context
@param edge One of the mraa_gpio_edge_t values, Interrupt trigger edge
@param pin The GPIO pin to use as the interrupt pin
@param isr Pointer to the function to be called, when the interrupt occurs
@param isr_args The arguments to be passed to the function
@return UPM result
*/
  upm_result_t kxtj3_install_isr(const kxtj3_context dev, mraa_gpio_edge_t edge, int pin, void (*isr)(void *), void *isr_args);

  /**
@brief Uninstalls a previously installed interrupt handler for interrupt pin

@param dev The sensor context
@return UPM result
*/
  void kxtj3_uninstall_isr(const kxtj3_context dev);

  /**
@brief Clears latching interrupts information of Wakeup (with axis and direction information) and Data Ready

See datasheet for more details

@param dev The sensor context
@return UPM result
*/
  upm_result_t kxtj3_clear_interrupt_information(kxtj3_context dev);

  /**
@brief Enables wakeup interrupt for the given axis (axis with direction)

Sensor must be in standby mode before performing this action

@param dev The sensor context
@param axis The axis to enable, takes one of KXTJ3_WAKEUP_SOURCE_T value
@return UPM result
 */
  upm_result_t kxtj3_enable_wakeup_single_axis_direction(kxtj3_context dev, KXTJ3_WAKEUP_SOURCE_T axis);

  /**
@brief Disables wakeup interrupt for the given axis (axis with direction)

Sensor must be in standby mode before performing this action

@param dev The sensor context
@param axis The axis to enable, takes one of KXTJ3_WAKEUP_SOURCE_T value
@return UPM result
 */
  upm_result_t kxtj3_disable_wakeup_single_axis_direction(kxtj3_context dev, KXTJ3_WAKEUP_SOURCE_T axis);

  /**
@brief Gets the source axis and direction of motion detection of the wakeup function interrupt

See datasheet for more details

@param dev The sensor context
@return A kxtj3_wakeup_axes struct with values of true/false for a wakeup for each axis and its direction
*/
  kxtj3_wakeup_axes kxtj3_get_wakeup_axis_and_direction(kxtj3_context dev);

  /**
@brief Enables the Unlatched mode motion interrupt (ULMODE). This mode is always by default enabled.

When this bit is set, the wakeup interrupt has to be cleared manually
(cannot use interrupt response with pulse)

Sensor must be in standby mode before performing this action

@param dev The sensor context
@return UPM result
*/
  upm_result_t kxtj3_enable_wakeup_latch(kxtj3_context dev);

  /**
@brief Disables the Unlatched mode motion interrupt (ULMODE). This mode is always by default enabled
(cannot use interrupt response with pulse).

The wakeup threshold is advised to not be very low to avoid interrupt being triggered in
an almost continuous manner

Sensor must be in standby mode before performing this action

When this bit is cleared, and the interrupt response type is set to Pulse, then upon a wakeup event
the wakeup interrupt signal will pulse and return low, but only once. Then, the interrupt
output will not reset until data is read or interrupt cleared.

@param dev The sensor context
@return UPM result
*/
  upm_result_t kxtj3_disable_wakeup_latch(kxtj3_context dev);

  /**
@brief Sets the timer counter of motion before sending a wakeup interrupt

The count is limited to values from 1 to 255

Every count is calculated as (1 / Wakeup_ODR_FREQUENCY) where Wakeup_ODR_FREQUENCY
is the current odr_in_Hz value from odr_map_in_Hz_wakeup value

Sensor must be in standby mode before performing this action

See datasheet for more details

@param dev The sensor context
@param count The timer count from 1 to 255
@return UPM result
 */
  upm_result_t kxtj3_set_wakeup_motion_counter(kxtj3_context dev, uint8_t count);

  /**
@brief Sets the timer of motion before sending a wakeup interrupt

the desired time should be such that (0 < desired_time * wakeup_odr_frequency <= 255)

Sensor must be in standby mode before performing this action

See datasheet for more details

@param dev The sensor context
@param desired_time The desired time in seconds
@return UPM result
 */
  upm_result_t kxtj3_set_wakeup_motion_time(kxtj3_context dev, float desired_time);

  /**
@brief Gets the current count value of the timer of motion before sending a wakeup interrupt

@param dev The sensor context
@param out_time Pointer to a float variable to store the time value in seconds
@return UPM result
 */
  upm_result_t kxtj3_get_wakeup_motion_time(kxtj3_context dev, float *out_time);

  /**
@brief Sets the timer counter of non-activity before sending another wakeup interrupt

The count is limited to values from 1 to 255

Every count is calculated as (1 / Wakeup_ODR_FREQUENCY) where Wakeup_ODR_FREQUENCY
is the current odr_in_Hz value from odr_map_in_Hz_wakeup value

Sensor must be in standby mode before performing this action

See datasheet for more details

@param dev The sensor context
@param count The timer count from 1 to 255
@return UPM result
 */
  upm_result_t kxtj3_set_wakeup_non_activity_counter(kxtj3_context dev, uint8_t count);

  /**
@brief Sets the timer of non-activity before sending another wakeup interrupt

the desired time should be such that (0 < desired_time * wakeup_odr_frequency <= 255)

Sensor must be in standby mode before performing this action

See datasheet for more details

@param dev The sensor context
@param desired_time The desired time in seconds
@return UPM result
 */
  upm_result_t kxtj3_set_wakeup_non_activity_time(kxtj3_context dev, float desired_time);

  /**
@brief Gets the current count value of the timer of non-activity before sending another wakeup interrupt

@param dev The sensor context
@param out_time Pointer to a float variable to store the time value in seconds
@return UPM result
 */
  upm_result_t kxtj3_get_wakeup_non_activity_time(kxtj3_context dev, float *out_time);

  /**
@brief Sets the threshold counter for acceleration difference before sending a wakeup interrupt

The count is limited to values from 1 to 4096, that it is 16g threshold with (3.9mg/count)
It is advised to not set the threshold to a very low value which may cause bad behaviour
in the wakeup interrupt

Sensor must be in standby mode before performing this action

See datasheet for more details

@param dev The sensor context
@param count The timer count from 1 to 4096, that it is 16g threshold with (3.9mg/count)
@return UPM result
 */
  upm_result_t kxtj3_set_wakeup_threshold_counter(kxtj3_context dev, uint16_t count);

  /**
@brief Sets the threshold g value for acceleration difference before sending a wakeup interrupt

The count is limited to values up to 16g, with steps of 3.9mg. It is advised to not set
the threshold to a very low value which may cause bad behaviour in the wakeup interrupt

Sensor must be in standby mode before performing this action

See datasheet for more details

@param dev The sensor context
@param g_threshold The acceleration threshold (in g) in g, from 3.9mg to 16g, steps of 3.9mg/count
@return UPM result
 */
  upm_result_t kxtj3_set_wakeup_threshold_g_value(kxtj3_context dev, float g_threshold);

  /**
@brief Gets the current threshold difference value before sending wakeup interrupt

@param dev The sensor context
@param out_threshold Pointer to a float variable to store the threshold value in g
@return UPM result
 */
  upm_result_t kxtj3_get_wakeup_threshold(kxtj3_context dev, float *out_threshold);

#ifdef __cplusplus
}
#endif
