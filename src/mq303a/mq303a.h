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
   * @brief C++ API for the MQ303A Alcohol Sensor
   *
   * UPM module for the MQ303A Alcohol Sensor
   *
   * @ingroup analog gas alcohol
   * @snippet mq303a.cxx Interesting
   */
  class MQ303A {
  public:
    /**
     * MQ303A sensor constructor
     *
     * @param pin analog pin to use
     * @param heaterPin digital pin mapped to analog pin to use
     */
    MQ303A(int pin, int heaterPin);

    /**
     * MQ303A Destructor
     */
    ~MQ303A();

    /**
     * Get the alcohol detected from the sensor.  This value is inverted,
     * a lower value means more alcohol detected.
     *
     * @return the moisture reading
     */
    int value();

    /**
     * Enable the heater.
     *
     * @param enable if true, enable the heater else disable
     * @return the alcohol reading
     */
    void heaterEnable(bool enable);

  private:
    mraa_aio_context m_aio;
    mraa_gpio_context m_gpio;
  };
}


