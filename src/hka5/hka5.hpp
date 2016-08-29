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

#include "hka5.h"

namespace upm {
  /**
   * @brief UPM C++ API for the DFRobot Laser PM2.5 Sensor
   * @defgroup hka5 libupm-hka5
   * @ingroup uart gpio gaseous
   */

  /**
   * @library hka5
   * @sensor hka5
   * @comname DFRobot Laser PM2.5 Sensor
   * @type gaseous
   * @man dfrobot
   * @con uart gpio
   *
   * @brief API for theDFRobot Laser PM2.5 Sensor
   *
   * This driver was tested with a DFRobot Laser PM2.5 Sensor.  It
   * connects to a UART at 9600 baud.  This is the only baud rate
   * supported.  It optionally supports Reset and Set/Sleep gpios as
   * well.
   *
   * @snippet hka5.cxx Interesting
   */

  class HKA5 {
  public:

    /**
     * HKA5 object constructor
     *
     * @param uart Specify which uart to use.
     * @param set_pin Specify the GPIO pin to use for Set.  -1 to not
     * use a set pin.
     * @param reset_pin Specify the GPIO pin to use for Reset.  -1 to
     * not use a reset pin.
     * @return an initialized device context on success, NULL on error.
     */
    HKA5(unsigned int uart, int set_pin, int reset_pin);

    /**
     * HKA5 object destructor
     */
    ~HKA5();

    /**
     * Enable or disable the device.  When disabled, the device enters a
     * low power mode and does not emit data.  You must have initialized
     * and connected the Set pin for this function to work.
     *
     * @param enable true to enable the device, false otherwise.
     */
    void enable(bool enable);

    /**
     * Reset the device.  You must have initialized and connected the
     * Reset pin for this function to work.
     */
    void reset();

    /**
     * Query the device and store the latest values.  You must call this
     * function before calling any of the get*() functions.
     */
    void update();

    /**
     * Return the last read PM 1.0 value.  The value is in micrograms
     * per cubic meter (ug/m3).  You must have called the update()
     * function before calling this function.
     *
     * @return The value in ug/m3
     */
    unsigned int getPM1();

    /**
     * Return the last read PM 2.5 value.  The value is in micrograms
     * per cubic meter (ug/m3).  You must have called the update()
     * function before calling this function.
     *
     * @return The value in ug/m3
     */
    unsigned int getPM2_5();

    /**
     * Return the last read PM 10.0 value.  The value is in micrograms
     * per cubic meter (ug/m3).  You must have called the update()
     * function before calling this function.
     *
     * @return The value in ug/m3
     */
    unsigned int getPM10();


  protected:
    // device context
    hka5_context m_hka5;

  private:
  };
}
