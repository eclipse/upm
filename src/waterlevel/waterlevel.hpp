/*
 * Author: Zion Orent <sorent@ics.com>
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

#include <mraa/gpio.h>

namespace upm {
  /**
   * @brief Water Level Switch Library
   * @defgroup waterlevel libupm-waterlevel
   * @ingroup seeed gpio liquid
   */
  /**
   * @library waterlevel
   * @sensor waterlevel
   * @comname Grove Water Level Sensor
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
   * @snippet apds9002.cxx Interesting
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
