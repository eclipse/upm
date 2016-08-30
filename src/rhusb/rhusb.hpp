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
#include <mraa/uart.hpp>

namespace upm {

  /**
   * @brief Omega RH-USB Temperature and Humidity Sensor
   * @defgroup rhusb libupm-rhusb
   * @ingroup uart temp
   */

  /**
   * @library rhusb
   * @sensor rhusb
   * @comname Omega RH-USB Temperature and Humidity Sensor
   * @type temp
   * @man omega
   * @con uart
   * @web http://www.omega.com/pptst/RH-USB.html
   *
   * @brief UPM API for the Omega RH-USB Temperature and Humidity Sensor
   *
   * This module implements support for the Omega RH-USB Temperature
   * and Humidity Sensor Probe.  It connects via an integrated USB
   * cable, and is accessed via a serial port.  It is suitable for
   * wall or duct mounting.
   *
   * @snippet rhusb.cxx Interesting
   */

  class RHUSB {
  public:
    /**
     * RHUSB constructor
     *
     * @param device Path to the serial device
     */
    RHUSB(std::string device);

    /**
     * RHUSB Destructor
     */
    ~RHUSB();

    /**
     * Read current values from the sensor and update internal stored
     * values.  This method must be called prior to querying any
     * values, such as temperature or humidity.
     */
    void update();

    /**
     * Get the current temperature.  update() must have been called
     * prior to calling this method.
     *
     * @param fahrenheit true to return the temperature in degrees
     * fahrenheit, false to return the temperature in degrees celsius.
     * The default is false (degrees Celsius).
     * @return The last temperature reading in Celsius or Fahrenheit
     */
    float getTemperature(bool fahrenheit=false);

    /**
     * Get the current relative humidity.  update() must have been called
     * prior to calling this method.
     *
     * @return The last humidity reading
     */
    float getHumidity();

    /**
     * Get the firmware identification string.
     *
     * @return The firmware identification
     */
    std::string getFirmwareID();


  protected:
    // serial i/o support
    bool dataAvailable(unsigned int millis);
    int writeStr(std::string data);
    std::string readStr(int len);
    std::string sendCommand(std::string cmd);

    mraa::Uart m_uart;

  private:
    // data
    float m_temperature;
    float m_humidity;
  };
}
