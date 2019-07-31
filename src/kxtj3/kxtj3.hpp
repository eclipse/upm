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

#include <vector>
#include <mraa/gpio.hpp>

#include "kxtj3.h"

/**
 * @brief C API for the kxtj3 driver
 * @defgroup kxtj3 libupm-kxtj3
 * @ingroup Kionix i2c accelerometer
 *
 * @include kxtj3.cxx
 */

/**
 * @library kxtj3
 * @sensor kxtj3
 * @comname tri-axis accelerometer
 * @type accelerometer
 * @man Kionix
 * @con i2c
 *
 * @brief C++ API for the kxtj3 driver
 *
 * @image html kxtj3.png
 */

namespace upm
{
class KXTJ3
{
public:
  /**
   * @brief KXTJ3 constructor
   *
   * If no errors occur, the device is initialized with default values and set to active state
   *
   * @param bus I2C bus
   * @param addr I2C address of the sensor
   * @throws std::runtime_error on initialization failure
    */
  KXTJ3(int bus, uint8_t addr = 0x0f);

  /**
   * @brief KXTJ3 destructor
   *
   * Closes the I2C context, and removes interrupts
   * Frees memory of the kxtj3_context
   *
   * @throws std::runtime_error on initialization failure
    */
  virtual ~KXTJ3();

  /**
   * @brief Initializes the sensor with given sampling rate, resolution and acceleration range.
   * This gets called in the constructor, so it will not need to be called unless the device is reset.
   *
   * Sensor is set to standby mode during the initialization and back to active after initialization.
   *
   * @param odr One of the KXTJ3_ODR_T values
   * @param resolution One of the KXTJ3_RESOLUTION_T values
   * @param g_range One of the KXTJ3_G_RANGE_T values
   * @throws std::runtime_error on failure
    */
  void SensorInit(KXTJ3_ODR_T odr, KXTJ3_RESOLUTION_T resolution, KXTJ3_G_RANGE_T g_range);

  /**
   * @brief Gets "who am I" value of the sensor
   *
   * @return Who am I value of the sensor
   * @throws std::runtime_error on failure
   */
  uint8_t GetWhoAmI();

  /**
   * @brief Sets the sensor to active mode
   *
   * @throws std::runtime_error on failure
    */
  void SensorActive();

  /**
   * @brief Sets the sensor to standby mode

   * @throws std::runtime_error on failure
    */
  void SensorStandby();

  /**
   * @brief Sets the acceleration range of the sensor
   *
   * Sensor needs to be in standby mode when setting the acceleration range value
   *
   * Be cautious not to set g_range to 14-bits modes with the resolution being on LOW_RES
   *
   * @param g_range One of the KXTJ3_G_RANGE_T values
   * @throws std::runtime_error on failure
   */
  void SetGRange(KXTJ3_G_RANGE_T g_range);

  /**
   * @brief Sets the resolution of the sensor. High resolution (14 bits and 12 bits) or low resolution (8 bits).
   *
   * LOW_RES valid only for ODR <= 200 Hz
   *
   * Be cautious not to set resolution to LOW_RES with the G_RANG being on 14-bits modes
   *
   * Sensor needs to be in standby mode when setting the sensor resolution
   *
   * @param resolution One of the KXTJ3_RESOLUTION_T values
   * @throws std::runtime_error on failure
   */
  void SetResolution(KXTJ3_RESOLUTION_T resolution);

  /**
   * @brief Sets the ODR of the sensor
   *
   * Sensor needs to be in standby mode when setting the ODR
   *
   * @param odr One of the KXTJ3_ODR_T values
   * @throws std::runtime_error on failure
   */
  void SetOdr(KXTJ3_ODR_T odr);

  /**
   * @brief Sets the ODR of the wakeup function of the sensor
   *
   * Sensor needs to be in standby mode when setting the ODR
   *
   * @param odr One of the KXTJ3_ODR_WAKEUP_T values
   * @throws std::runtime_error on failure
   */
  void SetOdrForWakeup(KXTJ3_ODR_WAKEUP_T odr);

  /**
   * @brief Performs a self-test for digital communication of the sensor. The test sets DCST bit in
   * CTRL_REG2, then checks the DCST_RESP register for a 0xAA, after the register is read, its
   * value is 0x55 and DCST bit is cleared.
   *
   * This method is called by SensorSelfTest also
   *
   * Sensor must be in standby mode before performing this action
   *
   * @throws std::runtime_error on failure
   */
  void SelfTestDigitalCommunication();

  /**
   * @brief Performs a self-test of the sensor. The test applies an electrostatic force to the sensor,
   * simulating input acceleration. The test compares samples from all axis before and after
   * applying the electrostatic force to the sensor. If the amount of acceleration increases according
   * to the values defined in TABLE 1 of the datasheet (0.5 g), the test passes.
   *
   * The method prints out the values before and during test and the average difference for each axis
   *
   * See the datasheet for more information
   *
   * @throws std::runtime_error on failure
   */
  void SensorSelfTest();

  /**
   * @brief Performs a sensor software reset. The software reset clears the RAM of the sensor and resets all registers
   * to pre-defined values.
   *
   * You should call kxtj3_sensor_init() after the software reset
   *
   * See the datasheet for more details
   *
   * @throws std::runtime_error on failure
    */
  void SensorSoftwareReset();

  /**
   * Gets raw acceleration data from the sensor.
   *
   * @return Acceleration vector [x, y, z]
   * @throws std::runtime_error on failure.
   */
  std::vector<float> GetAccelerationRawVector();

  /**
   * Gets acceleration data in (m/s^2) from the sensor.
   *
   * @return Acceleration vector [x, y, z]
   * @throws std::runtime_error on failure.
   */
  std::vector<float> GetAccelerationVector();

  /**
   * @brief Gets the duration of one sample period (in seconds) for getting the acceleration data depending on
   * the sampling rate of the sensor
   *
   * @return Floating point value of the sampling period
   * @throws std::runtime_error on failure
   */
  float GetAccelerationSamplePeriod();

  /**
   * @brief Gets the duration of one sample period (in seconds) for the wakeup function depending on
   * the sampling rate of the sensor wakeup function
   *
   * @return Floating point value of the sampling period
   * @throws std::runtime_error on failure
   */
  float GetWakeUpSamplePeriod();

  /**
   * @brief Enables the data ready interrupt. Availability of new acceleration data is
   * reflected as an interrupt.
   *
   * Sensor must be in standby mode before performing this action
   *
   * @throws std::runtime_error on failure
   */
  void EnableDataReadyInterrupt();

  /**
   * @brief Disables the data ready interrupt
   *
   * Sensor must be in standby mode before performing this action
   *
   * @throws std::runtime_error on failure
   */
  void DisableDataReadyInterrupt();

  /**
   * @brief Enables the wakeup function (motion detection)
   *
   * Sensor must be in standby mode before performing this action
   *
   * @throws std::runtime_error on failure
   */
  void EnableWakeUpInterrupt();

  /**
   * @brief Disables the wakeup function (motion detection)
   *
   * Sensor must be in standby mode before performing this action
   *
   * @throws std::runtime_error on failure
   */
  void DisableWakeUpInterrupt();

  /**
   * @brief Enables interrupts on the interrupt pin, sets polarity and response types.
   * Polarity ACTIVE_HIGH or ACTIVE_LOW.
   * Response either latch until cleared by reading INT_REL register, or transmit one pulse
   * Pulse width of 0.03-0.05ms.
   *
   * For Wakeup function, the response type is always latched unless set wakeup latch off
   * Sensor needs to be in standby mode when enabling the interrupt
   *
   * See datasheet for more details
   *
   * @param polarity Select the polarity of the interrupt. One of the KXTJ3_INTERRUPT_POLARITY_T values.
   * @param response_type Select the response type of the interrupt. One of the KXTJ3_INTERRUPT_RESPONSE_T values.
   * @throws std::runtime_error on failure
   */
  void EnableInterruptPin(KXTJ3_INTERRUPT_POLARITY_T polarity, KXTJ3_INTERRUPT_RESPONSE_T response_type);

  /**
   * @brief Disables interrupts on the interrupt pin of the sensor
   *
   * Sensor needs to be in standby mode when disabling the interrupt pin
   *
   * @throws std::runtime_error on failure
   */
  void DisableInterruptPin();

  /**
   * @brief Sets the polarity of the interrupt pin
   *
   * Polarity ACTIVE_HIGH or ACTIVE_LOW
   *
   * Sensor must be in standby mode before performing this action
   *
   * @param polarity Select the polarity of the interrupt. One of the KXTJ3_INTERRUPT_POLARITY_T values.
   * @throws std::runtime_error on failure
   */
  void SetInterruptPolarity(KXTJ3_INTERRUPT_POLARITY_T polarity);

  /**
   * @brief Sets the response type of the interrupt pin
   *
   * Response either latch until cleared by reading INT_REL register, or transmit one pulse
   * Pulse width of 0.03-0.05ms
   *
   * For Wakeup function, the response type is always latched unless set wakeup latch off
   *
   * Sensor must be in standby mode before performing this action
   *
   * See datasheet for more details
   *
   * @param response_type Select the response type of the interrupt. One of the KXTJ3_INTERRUPT_RESPONSE_T values
   * @throws std::runtime_error on failure
   */
  void SetInerruptResponse(KXTJ3_INTERRUPT_RESPONSE_T response_type);

  /**
   * @brief Gets the status of the interrupts. (Has an interrupt occurred)
   *
   * See datasheet for more details
   *
   * @return Return true if an interrupt event has occurred (DRDY or WUFS is 1),
   * returns false if no interrupts have occurred
   */
  bool GetInterruptStatus();

  /**
   * @brief Gets the source of the interrupt
   * The value stored is one or more of the KXTJ3_INTERRUPT_SOURCE_T values
   *
   * See datasheet for more details
   *
   * @return Value of the interrupt source register
   * @throws std::runtime_error on failure
   */
  uint8_t ReadInterruptSource1();

  /**
   * @brief Gets the source of the interrupt
   *
   * See datasheet for more details
   *
   * @return One of the KXTJ3_INTERRUPT_SOURCE_T values/types
   */
  KXTJ3_INTERRUPT_SOURCE_T GetInterruptSource();

  /**
   * @brief Installs an interrupt handler to be executed when an interrupt is detected on the interrupt pin

   * @param edge One of the mraa_gpio_edge_t values. Interrupt trigger edge.
   * @param pin The GPIO pin to use as the interrupt pin
   * @param isr Pointer to the method to be called, when the interrupt occurs
   * @param isr_args The arguments to be passed to the method
   * @return std::runtime_error on failure
   */
  void InstallIsr(mraa_gpio_edge_t edge, int pin, void (*isr)(void *), void *isr_args);

  /**
   * @brief Uninstalls a previously installed interrupt handler for interrupt pin
   *
   * @throws std::runtime_error on failure
   */
  void UninstallIsr();

  /**
   * @brief Clears latching interrupts information of Wakeup (with axis and direction information) and Data Ready
   *
   * See datasheet for more details
   *
   * @throws std::runtime_error on failure
   */
  void ClearInterrupt();

  /**
   * @brief Enables wakeup interrupt for the given axis (axis with direction)
   *
   * Sensor must be in standby mode before performing this action
   *
   * @param axis The axis to enable, takes one of KXTJ3_WAKEUP_SOURCE_T value
   * @throws std::runtime_error on failure
   */
  void EnableWakeUpSingleAxisDirection(KXTJ3_WAKEUP_SOURCE_T axis);

  /**
   * @brief Disables wakeup interrupt for the given axis (axis with direction)
   *
   * Sensor must be in standby mode before performing this action
   *
   * @param axis The axis to enable, takes one of KXTJ3_WAKEUP_SOURCE_T value
   * @throws std::runtime_error on failure
   */
  void DisableWakeUpSingleAxisDirection(KXTJ3_WAKEUP_SOURCE_T axis);

  /**
   * @brief Gets the source axis and direction of motion detection of the wakeup function interrupt
   *
   * See datasheet for more details
   *
   * @return A kxtj3_wakeup_axes struct with values of true/false
   * for a wakeup for each axis and its direction
   */
  kxtj3_wakeup_axes GetWakeUpAxisDirection();

  /**
   * @brief Enables the Unlatched mode motion interrupt (ULMODE). This mode is always by default enabled.
   *
   * When this bit is set, the wakeup interrupt has to be cleared manually
   * (cannot use interrupt response with pulse)
   *
   * Sensor must be in standby mode before performing this action
   *
   * @throws std::runtime_error on failure
   */
  void EnableWakeUpLatch();

  /**
   * @brief Disables the Unlatched mode motion interrupt (ULMODE). This mode is always by default enabled.
   * (cannot use interrupt response with pulse)
   *
   * The wakeup threshold is advised to not be very low to avoid interrupt being triggered in
   * an almost continuous manner
   *
   * Sensor must be in standby mode before performing this action
   *
   * When this bit is cleared, and the interrupt response type is set to Pulse, then upon a wakeup event
   * the wakeup interrupt signal will pulse and return low, but only once. Then, the interrupt
   * output will not reset until data is read or interrupt cleared.

   * @throws std::runtime_error on failure
   */
  void DisableWakeUpLatch();

  /**
   * @brief Sets the timer counter of motion before sending a wakeup interrupt
   *
   * The count is limited to values from 1 to 255
   *
   * Every count is calculated as (1 / Wakeup_ODR_FREQUENCY) where Wakeup_ODR_FREQUENCY
   * is the current odr_in_Hz value from odr_map_in_Hz_wakeup value
   *
   * Sensor must be in standby mode before performing this action
   *
   * See datasheet for more details
   *
   * @param count The timer count from 1 to 255
   * @throws std::runtime_error on failure
   */
  void SetWakeUpMotionCounter(uint8_t count);

  /**
   * @brief Sets the timer of motion before sending a wakeup interrupt
   *
   * the desired time should be such that (0 < desired_time * wakeup_odr_frequency <= 255)
   *
   * Sensor must be in standby mode before performing this action
   *
   * See datasheet for more details
   *
   * @param desired_time The desired time in seconds
   * @throws std::runtime_error on failure
   */
  void SetWakeUpMotionTime(float desired_time);

  /**
   * @brief Get the current count value of the timer of motion before sending a wakeup interrupt
   *
   * @return Time value in seconds
   * @throws std::runtime_error on failure
   */
  float GetWakeUpMotionTime();

  /**
   * @brief Sets the timer counter of non-activity before sending another wakeup interrupt
   *
   * The count is limited to values from 1 to 255
   *
   * Every count is calculated as (1 / Wakeup_ODR_FREQUENCY) where Wakeup_ODR_FREQUENCY
   * is the current odr_in_Hz value from odr_map_in_Hz_wakeup value
   *
   * Sensor must be in standby mode before performing this action
   *
   * See datasheet for more details
   *
   * @param count The timer count from 1 to 255
   * @throws std::runtime_error on failure
   */
  void SetWakeUpNonActivityCounter(uint8_t count);

  /**
   * @brief Sets the timer of non-activity before sending another wakeup interrupt
   *
   * the desired time should be such that (0 < desired_time * wakeup_odr_frequency <= 255)
   *
   * Sensor must be in standby mode before performing this action
   *
   * See datasheet for more details
   *
   * @param desired_time The desired time in seconds
   * @throws std::runtime_error on failure
   */
  void SetWakeUpNonActivityTime(float desired_time);

  /**
   * @brief Get the current count value of the timer of non-activity before sending another wakeup interrupt
   *
   * @return Time value in seconds
   * @throws std::runtime_error on failure
   */
  float GetWakeUpNonActivityTime();

  /**
   * @brief Sets the threshold counter for acceleration difference before sending a wakeup interrupt
   *
   * The count is limited to values from 1 to 4096, that it is 16g threshold with (3.9mg/count)
   * It is advised to not set the threshold to a very low value which may cause bad behaviour
   * in the wakeup interrupt
   *
   * Sensor must be in standby mode before performing this action
   *
   * See datasheet for more details
   *
   * @param count The timer count from 1 to 4096, that it is 16g threshold with (3.9mg/count)
   * @throws std::runtime_error on failure
   */
  void SetWakeUpThresholdCounter(uint16_t count);

  /**
   * @brief Sets the threshold g value for acceleration difference before sending a wakeup interrupt
   *
   * The count is limited to values up to 16g, with steps of 3.9mg. It is advised to not set
   * the threshold to a very low value which may cause bad behaviour in the wakeup interrupt
   *
   * Sensor must be in standby mode before performing this action
   *
   * See datasheet for more details
   *
   * @param g_threshold The acceleration threshold (in g) in g, from 3.9mg to 16g, steps of 3.9mg/count.
   * @throws std::runtime_error on failure
   */
  void SetWakeUpThresholdGRange(float g_threshold);

  /**
   * @brief Get the current threshold difference value before sending wakeup interrupt
   *
   * @return Threshold value in g
   * @throws std::runtime_error on failure
   */
  float GetWakeUpThresholdGRange();

private:
  kxtj3_context m_kxtj3;

  /* Disable implicit copy and assignment operators */
  KXTJ3(const KXTJ3 &) = delete;
  KXTJ3 &operator=(const KXTJ3 &) = delete;
};
} // namespace upm
