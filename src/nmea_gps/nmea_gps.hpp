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

#include "nmea_gps.h"

namespace upm {
  /**
   * @brief UPM C++ API for a generic GPS serial device reporting NMEA data
   * @defgroup nmea_gps libupm-nmea_gps
   * @ingroup uart gpio gps
   */
  
  /**
   * @library nmea_gps
   * @sensor nmea_gps
   * @comname Generic Serial GPS NMEA device
   * @type gps
   * @man dfrobot seed
   * @con uart gpio
   * @altname VK2828u7 ublox LEA-6H
   *
   * @brief API for the NMEA GPS Module
   *
   * This driver was tested with a number of GPS devices that emit
   * NMEA data via a serial interface of some sort (typically a UART).
   *
   * The I2C capablity was tested with a UBLOX LEA-6H based GPS shield
   * from DFRobot.  Currently, the I2C capability is only supported
   * for UBLOX devices (or compatibles) that conform to the
   * specifications outlined in the u-blox6 Receiver Description
   * Protocol Specification, Chapter 4, DDC Port.
   *
   * An example using the UART.
   * @snippet nmea_gps.cxx Interesting
   * An example using I2C.
   * @snippet nmea_gps_i2c.cxx Interesting
   */

  class NMEAGPS {
  public:

    /**
     * NMEAGPS object constructor for a UART
     *
     * @param uart Specify which uart to use.
     * @param baudrate Specify the baudrate to use.  The device defaults
     * to 9600 baud.
     * @param enable_pin Specify the GPIO pin to use for the enable pin,
     * -1 to not use an enable pin.
     */
    NMEAGPS(unsigned int uart, unsigned int baudrate,
            int enable_pin);

    /**
     * NMEAGPS object constructor for a UBLOX I2C interface
     *
     * @param bus Specify which the I2C bus to use.
     * @param addr Specify the I2C address to use.  For UBLOX devices,
     * this typically defaults to 0x42.
     */
    NMEAGPS(unsigned int bus, uint8_t addr);

    /**
     * NMEAGPS object destructor
     */
    ~NMEAGPS();

    /**
     * Read character data from the device.
     *
     * @param size The maximum number of characters to read.
     * @return string containing the data read.
     */
    std::string readStr(size_t size);

    /**
     * Write character data to the device.  This is only valid for a
     * UART device.
     *
     * @param buffer The string containing the data to write.
     * @return The number of bytes written.
     */
    int writeStr(std::string buffer);

    /**
     * Enable or disable the device.  When disabled, the device enters a
     * low power mode and does not emit NMEA data.  It will still
     * maintain location data however.
     *
     * @param enable true to enable the device, false otherwise.
     */
    void enable(bool enable);

    /**
     * Set the baudrate of the device.  By default, the constructor
     * will set the baudrate to 9600.  This is only valid for UART
     * devices.
     *
     * @param baudrate The baud rate to set for the device.
     */
    void setBaudrate(unsigned int baudrate);

    /**
     * Determine whether there is data available to be read.  In the
     * case of a UART, this function will wait up to "millis"
     * milliseconds for data to become available.  In the case of an I2C
     * device, the millis argument is ignored and the function will
     * return immediately, indicating whether data is available.
     *
     * @param millis The number of milliseconds to wait for data to
     * become available.
     * @return true if data is available to be read, false otherwise.
     */
    bool dataAvailable(unsigned int millis);

  protected:
    // nmeaGPS device context
    nmea_gps_context m_nmea_gps;

  private:
  };
}


