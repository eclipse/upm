/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <mraa/gpio.h>
#include <interfaces/iDistanceInterrupter.hpp>

namespace upm {

  /**
   * @brief RFR359 Distance Interrupter
   * @defgroup rfr359f libupm-rfr359f
   * @ingroup seeed gpio light robok tsk
   */
  /**
   * @library rfr359f
   * @sensor rfr359f
   * @comname Infrared Distance Interrupter
   * @altname Grove Distance Interrupter
   * @type light
   * @man seeed
   * @web https://www.seeedstudio.com/Grove-IR-Distance-Interrupter-p-1278.html
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
  class RFR359F : virtual public iDistanceInterrupter {
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
    virtual bool objectDetected();

  private:
    mraa_gpio_context m_gpio;
  };
}


