/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
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
#include "water.h"

namespace upm {
  /**
   * @brief Water Sensor
   * @defgroup water libupm-water
   * @ingroup seeed gpio liquid eak
   */

  /**
   * @library water
   * @sensor water
   * @comname Water Detection Sensor
   * @type liquid
   * @man seeed
   * @con gpio
   * @kit eak
   *
   * @brief API for the Water Sensor
   *
   * UPM module for the Water sensor
   *
   * @image html water.jpg
   * @snippet water.cxx Interesting
   */
  class Water {
  public:
    /**
     * digital water sensor constructor
     *
     * @param pin Digital pin to use
     */
    Water(unsigned int pin);
    /**
     * Water destructor
     */
    ~Water();
    /**
     * Gets the water (wet/not wet) value from the sensor
     *
     * @return True if the sensor is wet, false otherwise
     */
    bool isWet();

  private:
    /* Disable implicit copy and assignment operators */
    Water(const Water&) = delete;
    Water &operator=(const Water&) = delete;

    water_context m_water;
  };
}
