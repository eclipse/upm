/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <mraa/uart.hpp>
#include <interfaces/iHumidity.hpp>
#include <interfaces/iTemperature.hpp>

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

  class RHUSB : virtual public iHumidity, virtual public iTemperature {
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
    float getTemperature(bool fahrenheit);

    /**
     * Get the current temperature.  update() must have been called
     * prior to calling this method.
     *
     * @return The last temperature reading in Celsius
     */
    virtual float getTemperature();

    /**
     * Get the current relative humidity.  update() must have been called
     * prior to calling this method.
     *
     * @return The last humidity reading
     */
    virtual float getHumidity();

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
