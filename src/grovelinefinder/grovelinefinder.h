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
   * @brief Grove Line Finder Sensor library
   * @defgroup grovelinefinder libupm-grovelinefinder
   * @ingroup seeed gpio color robok
   */

  /**
   * @library grovelinefinder
   * @sensor grovelinefinder
   * @comname Grove Line Finder
   * @type color
   * @man seeed
   * @con gpio
   * @kit robok
   *
   * @brief API for the Grove Line Finder Sensor
   *
   * UPM module for the Grove Line Finder sensor.  It outputs a
   * digital signal indicating whether it is detecting black on a
   * white background, or white on a black background.
   *
   * @image html grovelinefinder.jpg
   * @snippet grovelinefinder.cxx Interesting
   */
  class GroveLineFinder {
  public:
    /**
     * Grove Line Finder digital sensor constructor
     *
     * @param pin Digital pin to use
     */
    GroveLineFinder(int pin);
    /**
     * GroveLineFinder destructor
     */
    ~GroveLineFinder();
    /**
     * Determines whether white has been detected
     *
     * @return True if white is detected
     */
    bool whiteDetected();
    /**
     * Determines whether black has been detected
     *
     * @return True if black is detected
     */
    bool blackDetected();

  private:
    mraa_gpio_context m_gpio;
  };
}


