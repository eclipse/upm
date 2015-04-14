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
#include <mraa/aio.h>

namespace upm {
  /**
   * @brief UPM library for the Grove Loudness Sensor
   * @defgroup groveloudness libupm-groveloudness
   * @ingroup seeed analog sound
   */

  /**
   * @library groveloudness
   * @sensor groveloudness
   * @comname Grove Loudness Sensor
   * @type sound
   * @man seeed
   * @con analog
   *
   * @brief C++ API for the Grove Loudness Sensor
   *
   * UPM module for the Grove Loudness Sensor.  This sensor
   * detects how loud the surrounding environment is.
   * The higher the output analog value, the louder the sound.
   *
   * @snippet groveloudness.cxx Interesting
   */
  class GroveLoudness {
  public:
    /**
     * Grove analog loudness sensor constructor
     *
     * @param pin analog pin to use
     */
    GroveLoudness(int pin);
    /**
     * GroveLoudness Destructor
     */
    ~GroveLoudness();
    /**
     * Get the loudness value from the sensor
     *
     * @return the loudness reading
     */
    int value();

  private:
    mraa_aio_context m_aio;
  };
}


