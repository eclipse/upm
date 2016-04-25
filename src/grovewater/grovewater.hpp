/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <mraa/gpio.h>

namespace upm {
  /**
   * @brief Grove Water Sensor library
   * @defgroup grovewater libupm-grovewater
   * @ingroup seeed gpio liquid eak
   */

  /**
   * @library grovewater
   * @sensor grovewater
   * @comname Grove Water Sensor
   * @type liquid
   * @man seeed
   * @con gpio
   * @kit eak
   *
   * @brief API for the Grove Water Sensor
   *
   * UPM module for the Grove Water sensor
   *
   * @image html grovewater.jpg
   * @snippet grovewater.cxx Interesting
   */
  class GroveWater {
  public:
    /**
     * Grove digital water sensor constructor
     *
     * @param pin Digital pin to use
     */
    GroveWater(int pin);
    /**
     * GroveWater destructor
     */
    ~GroveWater();
    /**
     * Gets the water (wet/not wet) value from the sensor
     *
     * @return True if the sensor is wet, false otherwise
     */
    bool isWet();

  private:
    mraa_gpio_context m_gpio;
  };
}
