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

#include "vk2828u7.h"

namespace upm {
  /**
   * @brief UPM C++ API for the DFRobot VK2828U7 Analog Accelerometer
   * @defgroup vk2828u7 libupm-vk2828u7
   * @ingroup dfrobot uart gpio gps
   */
  
  /**
   * @library vk2828u7
   * @sensor vk2828u7
   * @comname DFRobot VK2828U7 GPS Module
   * @type gps
   * @man dfrobot
   * @con uart gpio
   * @web http://www.dfrobot.com/index.php?route=product/product&search=gps&description=true&product_id=1302#.V7tMfN9ytNK
   *
   * @brief API for the DFRobot VK2828U7 GPS Module
   *
   * The driver was tested with the DFRobot VK2828U7 GPS module.  The
   * website claims the device can be run at 5v, however the datasheet
   * says the logic inputs and outputs can only handle 3.3v, and there
   * does not appear to be a schematic available. So this module was
   * tested only at 3.3v.
   *
   * @snippet vk2828u7.cxx Interesting
   */

  class VK2828U7 {
  public:

    /**
     * VK2828U7 object constructor
     *
     * @param uart Specify which uart to use.
     * @param baudrate Specify the baudrate to use.  The device defaults
     * to 9600 baud.
     * @param enable_pin Specify the GPIO pin to use for the enable pin,
     * -1 to not use an enable pin.
     */
    VK2828U7(unsigned int uart, unsigned int baudrate,
             int enable_pin);

    /**
     * VK2828U7 object destructor
     */
    ~VK2828U7();

    /**
     * Read character data from the device.
     *
     * @param size The maximum number of characters to read.
     * @return string containing the data read.
     */
    std::string readStr(size_t size);

    /**
     * Write character data to the device.
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
     * will set the baudrate to 9600.
     *
     * @param baudrate The baud rate to set for the device.
     */
    void setBaudrate(unsigned int baudrate);

    /**
     * Determine whether there is data available to be read.
     *
     * @param millis The number of milliseconds to wait for data to
     * become available.
     * @return true if data is available to be read, false otherwise.
     */
    bool dataAvailable(unsigned int millis);

  protected:
    // vk2828u7 device context
    vk2828u7_context m_vk2828u7;

  private:
  };
}


