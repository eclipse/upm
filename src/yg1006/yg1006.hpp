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
#include "yg1006.h"

namespace upm {
    /**
     * @brief YG1006 Flame Sensor library
     * @defgroup yg1006 libupm-yg1006
     * @ingroup seeed gpio light hak
     */

    /**
     * @library yg1006
     * @sensor yg1006
     * @comname YG1006 Flame Sensor
     * @altname Grove Flame Sensor
     * @type light
     * @man seeed
     * @web http://www.seeedstudio.com/wiki/Grove_-_Flame_Sensor
     * @con gpio
     * @kit hak
     *
     * @brief API for the YG1006 Flame Sensor
     *
     *   UPM module for the YG1006 flame sensor. It detects a flame or any
     *   other light source in the 760-1,100 nm wavelength range.
     *
     * @image html yg1006.jpg
     * @snippet yg1006.cxx Interesting
     */
  class YG1006 {
  public:

    /**
     * YG1006 constructor
     *
     * @param pin Digital pin to use
     */
    YG1006(unsigned int pin);

    /**
     * YG1006 destructor
     */
    ~YG1006();

    /**
     * Determines whether a flame has been detected
     *
     * @return true if a flame or another comparable light source has
     * been detected
     */
    bool flameDetected();

  private:
    yg1006_context m_yg1006;
  };
}


