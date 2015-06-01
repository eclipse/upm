/*
 * Author: Zion Orent <zorent@ics.com>
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
#include <mraa/aio.h>

namespace upm {

  /**
   * @brief Spectra Symbol Flex sensor library
   * @defgroup flex libupm-flex
   * @ingroup sparkfun analog flexfor
   */

  /**
   * @library flex
   * @sensor flex
   * @comname Flex Sensor
   * @type flexfor
   * @man sparkfun
   * @con analog
   *
   * @brief API for the Spectra Symbol Flex sensor
   *
   * A simple flex sensor, the resistance across the sensor increases when
   * bent. Patented technology by Spectra Symbol, these sensors were used in
   * the original Nintendo Power Glove.
   *
   * @image html flex.jpg
   * @snippet flex.cxx Interesting
   */
  class Flex {
  public:
    /**
     * Flex sensor constructor
     *
     * @param pin analog pin to use
     */
    Flex(int pin);
    /**
     * Flex Destructor
     */
    ~Flex();
    /**
     * @return the analog flex value
     */
    int value();

  private:
    mraa_aio_context m_aio;
  };
}


