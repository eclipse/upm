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
#include <interfaces/iMoisture.hpp>

namespace upm {
  /**
   * @brief Moisture Sensors Library
   * @defgroup grovemoisture libupm-grovemoisture
   * @ingroup seeed analog liquid eak hak
   */

  /**
   * @library moisture
   * @sensor moisture
   * @comname Analog Moisture Sensor
   * @altname Grove Moisture Sensor
   * @type liquid
   * @man seeed
   * @con analog
   * @kit eak hak
   *
   * @brief API for the Moisture Sensor
   *
   * UPM module for the Moisture Sensor.
   * This sensor can be used to detect the moisture content
   * of soil or whether there is water around the sensor.
   * As the moisture content increases, so does the value that is read.
   * Note: this sensor is not designed to be left in soil
   * nor to be used outdoors.
   *
   * @image html moisture.jpg
   * @snippet moisture.cxx Interesting
   */
  class Moisture : virtual public iMoisture {
  public:
    /**
     * Analog moisture sensor constructor
     *
     * @param pin Analog pin to use
     */
    Moisture(int pin);
    /**
     * Moisture destructor
     */
    ~Moisture();
    /**
     * Gets the moisture value from the sensor
     *
     * @return Moisture reading
     */
    int value();

    /**
     * Gets the moisture value from the sensor
     *
     * @return Moisture reading
     */
    virtual int getMoisture();

  private:
    mraa_aio_context m_aio;
  };
}


