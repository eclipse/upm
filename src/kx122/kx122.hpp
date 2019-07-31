/*
* The MIT License (MIT)
*
* Author: Samuli Rissanen <samuli.rissanen@hotmail.com>
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

#include "kx122.h"

/**
 * @file kx122.hpp
 * @library kx122
 * @brief C++ API for the kx122 driver
 *
 * @include kx122.cxx
 */

namespace upm{

  class KX122{
    public:

      /**
      KX122 constructor
      Set addr to -1 if using SPI.
      When using I2C, set chip_select_pin to -1

      If no errors occur, the device gets initialized with default values and gets set to active state.

      @param bus I2C or SPI bus to use.
      @param addr I2C address of the sensor.
      @param chip_select Chip select pin for SPI.
      @param spi_bus_frequency Speed of the SPI communication bus in Hz.
      @throws std::runtime_error on initialization failure.
      */
      KX122(int bus, int addr, int chip_select, int spi_bus_frequency = DEFAULT_SPI_FREQUENCY);

      /**
      KX122 destructor
      */
      ~KX122();

      /**
      Initializes the sensor with given sampling rate, resolution and acceleration range.
      This gets called in the constructor, so it will not need to be called unless the device is reset.

      Sensor is automatically set into standby mode during the initialization.
      Sensor is set to active mode after initialization.

      @param odr One of the KX122_ODR_T values.
      @param res One of the KX122_RES_T values.
      @param grange One of the KX122_RANGE_T values.
      @throws std::runtime_error on failure.
      */
      void deviceInit(KX122_ODR_T odr, KX122_RES_T res, KX122_RANGE_T grange);

      /**
      Gets the length of one sample period depending on the sampling rate of the sensor.
      @return Floating point value of the sampling period, or if error occurs returns -1.
      */
      float getSamplePeriod();

      /**
      Gets who am i value of the sensor.

      @return Who am I value of the sensor.
      @throws std::runtime_error on failure.
      */
      uint8_t getWhoAmI();

      /**
      Gets raw accelerometer data from the sensor.
      @param x Pointer to a floating point variable to store the x-axis value. Can be set to NULL if not wanted.
      @param y Pointer to a floating point variable to store the y-axis value. Can be set to NULL if not wanted.
      @param z Pointer to a floating point variable to store the z-axis value. Can be set to NULL if not wanted.
      @throws std::runtime_error on failure.
      */
      void getRawAccelerationData(float *x, float *y, float *z);

      /**
      Gets converted (m/s^2) accelerometer data from the sensor.

      @param x Pointer to a floating point variable to store the x-axis value. Can be set to NULL if not wanted.
      @param y Pointer to a floating point variable to store the y-axis value. Can be set to NULL if not wanted.
      @param z Pointer to a floating point variable to store the z-axis value. Can be set to NULL if not wanted.
      @throws std::runtime_error on failure.
      */
      void getAccelerationData(float *x, float *y, float *z);

      /**
      Gets converted (m/s^2) accelerometer data from the sensor.

      @return Acceleration vector [X, Y, Z]
      @throws std::runtime_error on failure.
      */
      std::vector<float> getAccelerationDataVector();

      /**
      Performs a sensor software reset. The software reset clears the RAM of the sensor and resets all registers
      to pre-defined values.

      You should call deviceInit() after the software reset.

      See the datasheet for more details.

      @throws std::runtime_error on failure.
      */
      void softwareReset();

      /**
      Enables sensor filter bypass.

      Sensor needs to be in standby mode when enabling the filter bypass.

      @throws std::runtime_error on failure.
      */
      void enableIIR();

      /**
      Disables sensor filter bypass.

      Sensor needs to be in standby mode when enabling the filter bypass.

      @throws std::runtime_error on failure.
      */
      void disableIIR();

      /**
      Performs a self-test of the sensor. The test applies an electrostatic force to the sensor,
      simulating input acceleration. The test compares samples from all axis before and after
      applying the electrostatic force to the sensor. If the amount of acceleration increases according
      to the values defined in TABLE 1 of the datasheet, the test passes.

      The function prints out the minimum, maximum and values calculated during the test
      for each axis, and the result of the test for each axis.

      See the datasheet for more information.

      @throws std::runtime_error on failure.
      */
      void selfTest();

      /**
      Sets the sensor to the standby mode.

      @throws std::runtime_error on failure.
      */
      void setSensorStandby();

      /**
      Sets the sensor to the active mode.

      @throws std::runtime_error on failure.
      */
      void setSensorActive();

      /**
      Sets the ODR of the sensor.

      Sensor needs to be in standby mode when setting the ODR.

      @param odr One of the KX122_ODR_T values.
      @throws std::runtime_error on failure.
      */
      void setODR(KX122_ODR_T odr);

      /**
      Sets the acceleration range of the sensor.

      Sensor needs to be in standby mode when setting the acceleration range value.

      @param grange One of the KX122_RANGE_T values.
      @throws std::runtime_error on failure.
      */
      void setGrange(KX122_RANGE_T grange);

      /**
      Sets the resolution of the sensor. High resolution (16 bits) or low resolution (8 bits).

      Sensor needs to be in standby mode when setting the sensor resolution.

      When sensor is set to low resolution mode, the sensor runs in low power mode, which in turn
      enables features such as averaging.(setAverage()).

      @param res One of the KX122_RES_T values.
      @throws std::runtime_error on failure.
      */
      void setResolution(KX122_RES_T res);

      /**
      Sets the low pass filter roll off

      Sensor needs to be in standby mode when setting the filter roll off value.

      @param lpro One of the LPRO_STATE_T values.
      @throws std::runtime_error on failure.
      */
      void setBW(LPRO_STATE_T lpro);

      /**
      Set the amount of samples to be averaged in low power mode.

      Sensor needs to be in standby mode when setting the average value.

      @param avg One of the KX122_AVG_T values.
      @throws std::runtime_error on failure.
      */
      void setAverage(KX122_AVG_T avg);

      /**
      Installs an interrupt handler to be executed when an interrupt is detected on the interrupt pin.

      @param edge One of the mraa::Edge values. Interrupt trigger edge.
      @param intp One of the KX122_INTERRUPT_PIN_T values. Specifies which interrupt pin you are setting.
      @param pin The GPIO pin to use as the interrupt pin.
      @param isr Pointer to the function to be called, when the interrupt occurs.
      @param arg The arguments to be passed to the function.
      @throws std::runtime_error on failure.
      */
      void installISR(mraa::Edge edge, KX122_INTERRUPT_PIN_T intp, int pin, void (*isr)(void*), void *arg);

      /**
      Uninstalls a previously installed interrupt handler.

      @param intp One of the KX122_INTERRUPT_PIN_T values. Specifies which interrupt pin handler is uninstalled.
      @throws std::runtime_error on failure.
      */
      void uninstallISR(KX122_INTERRUPT_PIN_T intp);

      /**
      Enables interrupts on the interrupt pin 1 of the sensor.

      Pulse width = 50us (10us if data sampling rate > 1600Hz).
      Using pulse mode.

      Sensor needs to be in standby mode when enabling the interrupt.

      See datasheet for more details.

      @param polarity One of the KX122_INTERRUPT_POLARITY_T values. Select the polarity of the interrupt.
      @throws std::runtime_error on failure.
      */
      void enableInterrupt1(KX122_INTERRUPT_POLARITY_T polarity);

      /**
      Enables interrupts on the interrupt pin 2 of the sensor.

      Pulse width = 50us (10us if data sampling rate > 1600Hz).
      Using pulse mode.

      Sensor needs to be in standby mode when enabling the interrupt.

      See datasheet for more details.

      @param polarity One of the KX122_INTERRUPT_POLARITY_T values. Select the polarity of the interrupt.
      @throws std::runtime_error on failure.
      */
      void enableInterrupt2(KX122_INTERRUPT_POLARITY_T polarity);

      /**
      Disables interrupts on the interrupt pin 1 of the sensor.

      Sensor needs to be in standby mode when disabling the interrupt pin 1.

      @throws std::runtime_error on failure.
      */
      void disableInterrupt1();

      /**
      Disables interrupts on the interrupt pin 2 of the sensor.

      Sensor needs to be in standby mode when disabling the interrupt pin 2.

      @throws std::runtime_error on failure.
      */
      void disableInterrupt2();

      /**
      Routes the interrupts to the interrupt pin 1 of the sensor.

      Sensor needs to be in standby mode when routing the interrupts.

      See datasheet for more details.

      @param bits One or more of the KX122_INTERRUPT_T values. Combine with bitwise OR (|)
      @throws std::runtime_error on failure.
      */
      void routeInterrupt1(uint8_t bits);

      /**
      Routes the interrupts to the interrupt pin 2 of the sensor.

      Sensor needs to be in standby mode when routing the interrupts.

      See datasheet for more details.

      @param bits One or more of the KX122_INTERRUPT_T values. Combine with bitwise OR (|)
      @throws std::runtime_error on failure.
      */
      void routeInterrupt2(uint8_t bits);

      /**
      Gets the status of the interrupts. (Has an interrupt occurred)

      See datasheet for more details.

      @return Return true if an interrupt event has occurred, returns false if no interrupts have occurred.
      */
      bool getInterruptStatus();

      /**
      Gets the source of the interrupt.

      The value stored is one or more of the KX122_INTERRUPT_T values, depending on the interrupts
      that have occurred.

      If multiple interrupts have occurred, and you need to determine a specific interrupt,
      you can use masking to get the state of the specific interrupt:
      (int_source & KX122_DATA_READY_INT) == KX122_DATA_READY_INT)

      See datasheet for more details.

      @return Interrupt source value.
      @throws std::runtime_error on failure.
      */
      uint8_t getInterruptSource();

      /**
      Clears latching interrupts (Wakeup, Data Ready).

      See datasheet for more details.

      @throws std::runtime_error on failure.
      */
      void clearInterrupt();

      /**
      Enables the data ready interrupt. Availability of new acceleration data is
      reflected as an interrupt.

      Sensor needs to be in standby mode when enabling the interrupt.

      @throws std::runtime_error on failure.
      */
      void enableDataReadyInterrupt();

      /**
      Disables the data ready interrupt.

      Sensor needs to be in standby mode when disabling the interrupt.

      @throws std::runtime_error on failure.
      */
      void disableDataReadyInterrupt();

      /**
      Enables the buffer full interrupt.
      Buffer can hold 681 sets of values (8 bit, low resolution mode) or
      340 sets of values (16 bit, high resolution mode).

      Sensor needs to be in standby mode when enabling the interrupt.

      @throws std::runtime_error on failure.
      */
      void enableBufferFullInterrupt();

      /**
      Disables the buffer full interrupt.

      Sensor needs to be in standby mode when disabling the interrupt.

      @throws std::runtime_error on failure.
      */
      void disableBufferFullInterrupt();

      /**
      Enables the buffer.

      Sensor needs to be in standby mode when enabling the buffer.

      @throws std::runtime_error on failure.
      */
      void enableBuffer();

      /**
      Disables the buffer.

      Sensor needs to be in standby mode when disabling the buffer.

      @throws std::runtime_error on failure.
      */
      void disableBuffer();

      /**
      Initializes the buffer with the given sample watermark level, buffer resolution and buffer operating mode.
      Buffer is enabled after the initialization.

      Sensor is automatically set into standby mode during the buffer initialization.
      Sensor is set to active mode after initialization.

      See the other buffer functions for details about the different parameter values.

      @param samples Amount of samples to trigger the watermark interrupt.
      @param res One of the KX122_RES_T values.
      @param mode One of the KX122_BUFFER_MODE_T values.
      @throws std::runtime_error on failure.
      */
      void bufferInit(uint samples, KX122_RES_T res, KX122_BUFFER_MODE_T mode);

      /**
      Sets the buffer resolution.
      Buffer resolution is independent of the sensor resolution.

      Sensor needs to be in standby mode when setting the buffer resolution.

      @throws std::runtime_error on failure.
      */
      void setBufferResolution(KX122_RES_T res);

      /**
      Sets the buffer watermark interrupt threshold.
      When the buffer sample count reaches the watermark, watermark interrupt will be given.

      In low resolution mode, maximum number of samples is 681. In high resolution, the maximum
      number is 340.

      See datasheet for more details.

      Sensor needs to be in standby mode when setting the buffer threshold.

      @param samples Amount of samples to trigger the watermark interrupt.
      @throws std::runtime_error on failure.
      */
      void setBufferThreshold(uint samples);

      /**
      Sets the buffer operating mode.
      The buffer can operate in FIFO,FILO or Stream mode.
      The buffer gathers data, reports data and interacts with the status indicators
      in a slightly different way depending on the operating mode.

      See datasheet for more details.

      Sensor needs to be in standby mode when setting the buffer mode.

      @param mode One of the KX122_BUFFER_MODE_T values.
      @throws std::runtime_error on failure.
      */
      void setBufferMode(KX122_BUFFER_MODE_T mode);

      /**
      Gets the current amount of samples in the buffer.

      @return number of samples in the buffer.
      @throws std::runtime_error on failure.
      */
      uint getBufferStatus();

      /**
      Gets the specified amount of raw acceleration samples from the buffer.

      @param len The amount of samples to read from the buffer.
      @return vector containing x, y & z-axis data
      @throws std::runtime_error on failure.
      */
      std::vector<float> getRawBufferSamples(uint len);

      /**
      Gets the specified amount of converted (m/s^2) acceleration samples from the buffer.

      @param len The amount of samples to read from the buffer.
      @return vector containing x, y & z-axis data
      @throws std::runtime_error on failure.
      */
      std::vector<float> getBufferSamples(uint len);

      /**
      Clears the buffer, removing all existing samples from the buffer.

      @throws std::runtime_error on failure.
      */
      void clearBuffer();
    private:
      //Device context
      kx122_context m_kx122;

      /* Disable implicit copy and assignment operators */
      KX122(const KX122&) = delete;
      KX122 &operator=(const KX122&) = delete;
  };
}
