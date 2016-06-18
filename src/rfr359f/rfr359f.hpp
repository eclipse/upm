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
   * @brief RFR359 Distance Interrupter library
   * @defgroup rfr359f libupm-rfr359f
   * @ingroup seeed gpio light robok tsk
   */
  /**
   * @library rfr359f
   * @sensor rfr359f
   * @comname RFR359F Distance Interrupter
   * @altname Grove Distance Interrupter
   * @type light
   * @man seeed
   * @web http://www.seeedstudio.com/wiki/Grove_-_IR_Distance_Interrupt
   * @con gpio
   * @kit robok tsk
   *
   * @brief API for the RFR359F-based Grove Distance Interrupter. 
   *
   * UPM module for the Grove distance interrupter. The sensitivity
   * can be adjusted with the potentiometer on the sensor module. It
   * has a range of approximately 4 inches and a quick response time.
   *
   * @image html rfr359f.jpg
   * @snippet rfr359f.cxx Interesting
   */
  class RFR359F {
  public:
    /**
     * RFR359F constructor
     *
     * @param pin Digital pin to use
     */
    RFR359F(int pin);

    /**
     * RFR359F destructor
     */
    ~RFR359F();

    /**
     * Gets the status of the pin; true means an object has been detected
     *
     * @return True if the sensor has detected an object
     */
    bool objectDetected();

  private:
    mraa_gpio_context m_gpio;
  };
}


