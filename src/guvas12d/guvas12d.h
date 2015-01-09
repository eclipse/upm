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
   * @brief C++ API for the GUVAS12D UV sensor module
   *
   * UPM module for the GUVAS12D UV Sensor
   *
   * @ingroup grove analog
   * @snippet guvas12d.cxx Interesting
   */
  class GUVAS12D {
  public:
    /**
     * GUVAS12D sensor constructor
     *
     * @param pin analog pin to use
     */
    GUVAS12D(int pin);
    /**
     * GUVAS12D Destructor
     */
    ~GUVAS12D();
    /**
     * Get the averaged voltage value from the sensor
     *
     * @param aref the reference voltage in use (5.0 or 3.3 usually)
     * @param samples number of samples to average over
     * @return the averaged voltage reading
     */
    float value(float aref, unsigned int samples);

  private:
    mraa_aio_context m_aio;
  };
}


