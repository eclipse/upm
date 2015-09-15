/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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
#include <mraa/aio.hpp>

namespace upm {
  /**
   * @brief Loudness Sensor library
   * @defgroup loudness libupm-loudness
   * @ingroup seeed dfrobot analog sound
   */

  /**
   * @library loudness
   * @sensor loudness
   * @comname Loudness Sensor
   * @altname mic hyld9767 
   * @type sound
   * @man seeed dfrobot
   * @con analog
   *
   * @brief API for the Loudness Sensor
   *
   * UPM module for the Loudness Sensor. This sensor family is
   * typically composed of a electret microphone and an amplifier that
   * provides a varying analog voltage proportional to the loudness of
   * the ambient sound environment.  The higher the output analog
   * value, the louder the ambient sound.
   *
   * This module has been tested with the Grove Loudness sensor, the
   * Grove Sound sensor and the DFRobot Loudness Sensor V2.
   *
   * @image html groveloudness.jpg
   * @snippet loudness.cxx Interesting
   */

  class Loudness {
  public:
    /**
     * Loudness sensor constructor
     *
     * @param pin Analog pin to use
     */
    Loudness(int pin);

    /**
     * Loudness destructor
     */
    ~Loudness();

    /**
     * Gets the loudness value from the sensor
     *
     * @return Loudness reading
     */
    int value();

  protected:
    mraa::Aio m_aio;
  };
}


