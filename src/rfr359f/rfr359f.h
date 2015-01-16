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
#include <mraa/gpio.h>

namespace upm {

  /**
   * @brief C++ API for the RFR359F based Grove Distance Interrupter. 
   *
   * UPM module for the Grove Distance Interrupter.  The sensitivity
   * can be adjusted with the potentiometer on the sensor module.  It
   * has a range of approximately 4 inches, and a fast response time.
   *
   * @ingroup grove gpio
   * @snippet rfr359f.cxx Interesting
   */
  class RFR359F {
  public:
    /**
     * Grove Distance Interrupter constructor
     *
     * @param pin digital pin to use
     */
    RFR359F(int pin);

    /**
     * RFR359F Destructor
     */
    ~RFR359F();

    /**
     * Get the status of the pin, true means object detected
     *
     * @return true if the sensor detected an object
     */
    bool objectDetected();

  private:
    mraa_gpio_context m_gpio;
  };
}


