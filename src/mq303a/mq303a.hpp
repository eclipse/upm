/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <mraa/aio.h>

namespace upm {

  /**
   * @brief MQ303A Alcohol Sensor
   * @defgroup mq303a libupm-mq303a
   * @ingroup seeed analog gpio gaseous tsk
   */
  /**
   * @library mq303a
   * @sensor mq303a
   * @comname Alcohol Detection Sensor
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


