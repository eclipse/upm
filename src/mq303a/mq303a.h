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
   * @brief MQ303A Alcohol Sensor library
   * @defgroup mq303a libupm-mq303a
   * @ingroup seeed analog gpio gaseous tsk
   */
  /**
   * @library mq303a
   * @sensor mq303a
   * @comname MQ303A Alcohol Sensor
   * @altname Grove Alcohol Sensor
   * @type gaseous
   * @man seeed
   * @web http://www.seeedstudio.com/wiki/Grove_-_Alcohol_Sensor
   * @con analog gpio
   * @kit tsk
   *
   * @brief API for the MQ303A Alcohol Sensor
   *
   * UPM module for the MQ303A alcohol sensor.
   * This sensor needs to be warmed up before stable results are
   * obtained. The higher the value returned from value(),
   * the higher the amount of alcohol detected.
   *
   * @image html mq303a.jpg
   * @snippet mq303a.cxx Interesting
   */
  class MQ303A {
  public:
    /**
     * MQ303A constructor
     *
     * @param pin Analog pin to use
     * @param heaterPin Digital pin mapped to the analog pin to use
     */
    MQ303A(int pin, int heaterPin);

    /**
     * MQ303A destructor
     */
    ~MQ303A();

    /**
     * Gets the alcohol reading from the sensor.  
     * The value read from the analog pin is inverted.  
     * A higher returned value means a higher amount of alcohol detected.
     *
     * @return Alcohol reading
     */
    int value();

    /**
     * Enables the heater
     *
     * @param enable Enables the heater if true; otherwise, disables it
     */
    void heaterEnable(bool enable);

  private:
    mraa_aio_context m_aio;
    mraa_gpio_context m_gpio;
  };
}


