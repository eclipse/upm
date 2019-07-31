/*
 * Author: Zion Orent <sorent@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <mraa/gpio.h>

namespace upm {
  /**
   * @brief Water Level Switch
   * @defgroup waterlevel libupm-waterlevel
   * @ingroup seeed gpio liquid
   */
  /**
   * @library waterlevel
   * @sensor waterlevel
   * @comname Water Level Sensor
   * @type liquid
   * @man seeed
   * @con gpio
   * 
   * @brief UPM module for the Grove Water Level Switch
   *
   * This switch is a device used to sense the level of liquid within a tank,
   * it may actuate a pump, an indicator, an alarm, or other devices.
   *
   * @image html waterlevel.jpg 
   * @snippet waterlevel.cxx Interesting
   */
  class WaterLevel {
  public:
    /**
     * WaterLevel constructor
     *
     * @param pin Digital pin to use
     */
    WaterLevel(int pin);
    /**
     * WaterLevel destructor
     */
    ~WaterLevel();
    /**
     * @return bool Determines whether liquid has risen above the sensor
     */
     bool isSubmerged();

  private:
        mraa_gpio_context m_gpio;
	};
}
