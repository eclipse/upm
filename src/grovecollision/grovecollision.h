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
   * @brief Grove Collision Sensor library
   * @defgroup grovecollision libupm-grovecollision
   * @ingroup seeed gpio accelerometer
   */

  /**
   * @library grovecollision
   * @sensor grovecollision
   * @comname Grove Collision Sensor
   * @type accelerometer
   * @man seeed
   * @con gpio
   *
   * @brief API for the Grove Collision Sensor
   * 
   * The Grove Collision Sensor can detect whether any
   * collision movement or vibration happens.
   * It outputs a low pulse signal when vibration is detected. 
   *
   * @image html grovecollision.jpg
   * @snippet grovecollision.cxx Interesting
   */
  class GroveCollision {
  public:
    /**
     * Grove collision sensor constructor
     *
     * @param pin Digital pin to use
     */
    GroveCollision(int pin);
    /**
     * GroveCollision destructor
     */
    ~GroveCollision();
    /**
     * @return bool  Defines whether something is colliding with sensor
     */
     bool isColliding();

  private:
        mraa_gpio_context m_gpio;
	};
}
