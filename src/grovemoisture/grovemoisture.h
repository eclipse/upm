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
   * @brief Grove Moisture Sensor library
   * @defgroup grovemoisture libupm-grovemoisture
   * @ingroup seeed analog liquid eak hak
   */

  /**
   * @library grovemoisture
   * @sensor grovemoisture
   * @comname Grove Moisture Sensor
   * @type liquid
   * @man seeed
   * @con analog
   * @kit eak hak
   *
   * @brief API for the Grove Moisture Sensor
   *
   * UPM module for the Grove Moisture Sensor.
   * This sensor can be used to detect the moisture content
   * of soil or whether there is water around the sensor.
   * As the moisture content increases, so does the value that is read.
   * Note: this sensor is not designed to be left in soil
   * nor to be used outdoors.
   *
   * @image html grovemoisture.jpg
   * @snippet grovemoisture.cxx Interesting
   */
  class GroveMoisture {
  public:
    /**
     * Grove analog moisture sensor constructor
     *
     * @param pin Analog pin to use
     */
    GroveMoisture(int pin);
    /**
     * GroveMoisture destructor
     */
    ~GroveMoisture();
    /**
     * Gets the moisture value from the sensor
     *
     * @return Moisture reading
     */
    int value();

  private:
    mraa_aio_context m_aio;
  };
}


