/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <string>
#include <iostream>

#include <stdlib.h>
#include <unistd.h>

#include "sht1x.h"

namespace upm {
  /**
   * @brief SHT1X Temperature and Humidity Sensor
   * @defgroup sht1x libupm-sht1x
   * @ingroup gpio temp
   */

  /**
   * @library sht1x
   * @sensor sht1x
   * @comname SHT1X Temperature and Humidity Sensor
   * @type temp
   * @man dfrobot
   * @con gpio
   *
   * @brief SHT1X Temperature and Humidity Sensor
   *
   * This driver was tested with a DFRobot SHT10 Temperature and
   * Humidity sensor.  This driver should work on all SHT1X devices.
   *
   * It requires a 10K pull-up resistor connected to the data pin.
   * The sensor can be run at differing voltages from 2.5v to 5v.
   *
   * @snippet sht1x.cxx Interesting
   */

  class SHT1X {
  public:

    /**
     * SHT1X object constructor
     *
     * @param clk_pin Specify the GPIO pin to use for the clock.
     * @param data_pin Specify the GPIO pin to use for data.
     */
    SHT1X(unsigned int clk_pin, unsigned int data_pin);

    /**
     * SHT1X object destructor
     */
    ~SHT1X();

    /**
     * Perform a soft reset of the device.
     */
    void reset();

    /**
     * Query the device and store the latest values.  You must call this
     * function before querying the temperature or the humidity.
     */
    void update();

    /**
     * Query the temperature in degrees Celsius.  update() must have
     * been called prior to calling this function.
     *
     * @return The temperature in Celsius
     */
    float getTemperature();

    /**
     * Query the relative humidity.  update() must have been called
     * prior to calling this function.
     *
     * @return The relative humidity.
     */
    float getHumidity();

    /**
     * Read the status register.
     *
     * @return The contents of the status register.
     */
    uint8_t readStatus();

    /**
     * Write a value to the status register.
     *
     * @param status The uint8_t to write to the register.
     * @return UPM result
     */
    upm_result_t writeStatus(uint8_t status);

    /**
     * The Vdd voltage the sensor is being driven at can affect the
     * temperature measurements.  This function allows you to specify a
     * voltage as close as you are using to power the sensor, so that
     * the appropriate compensation can be made.  By default, the
     * coefficients are set for 5v operation.
     *
     * @param volts One of the SHT1X_VOLTS_T values.
     */
    void setVolts(SHT1X_VOLTS_T volts);

  protected:
    // device context
    sht1x_context m_sht1x;

    /**
     * Send a command to the device.  This is a low level command that
     * should not be used directly unless you know exactly what you are
     * doing.
     *
     * @param cmd One of the SHT1X_CMD_T values.
     * @return UPM result
     */
    upm_result_t sendCommand(SHT1X_CMD_T cmd);

    /**
     * Wait for the appropriate response when starting a temperature or
     * humidity measurement.  This is a low level command that should
     * not be used directly unless you know exactly what you are doing.
     *
     * @return UPM result
     */
    upm_result_t waitForResponse();

    /**
     * Issue the start transmission sequence.  This is a low level
     * command that should not be used directly unless you know exactly
     * what you are doing.
     *
     * @return UPM result
     */
    void startXmit();

    /**
     * Read 8 bits (a byte) from the device.  This is a low level
     * command that should not be used directly unless you know exactly
     * what you are doing.
     *
     * @param value Pointer to a uint8_t that will hold the received byte.
     * @return UPM result
     */
    uint8_t read8Bits();

    /**
     * Write 8 bits (a byte) to the device.  This is a low level command
     * that should not be used directly unless you know exactly what you
     * are doing.
     *
     * @param value A uint8_t that will be sent to the device.
     * @return UPM result
     */
    upm_result_t write8Bits(uint8_t byte);

  private:
  };
}
