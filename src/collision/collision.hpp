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

#include <mraa/initio.hpp>
#include <mraa/gpio.h>
#include <interfaces/iCollision.hpp>

namespace upm {
  /**
   * @brief Collision Sensor
   * @defgroup collision libupm-collision
   * @ingroup seeed gpio accelerometer
   */

  /**
   * @library collision
   * @sensor collision
   * @comname Collision Sensor
   * @altname Grove Collision Sensor
   * @type accelerometer
   * @man seeed
   * @con gpio
   *
   * @brief API for the Collision Sensor
   *
   * The Collision Sensor can detect whether any
   * collision movement or vibration happens.
   * It outputs a low pulse signal when vibration is detected.
   *
   * @image html collision.jpg
   * @snippet collision.cxx Interesting
   */
  class Collision : virtual public iCollision {
  public:
    /**
     * Collision sensor constructor
     *
     * @param pin Digital pin to use
     */
    Collision(int pin);
    /**
     * Instantiates Collision Sensor based on a given string.
     *
     * @param initStr string containing specific information for  Collision Sensor.
     */
    Collision(std::string initStr);
    /**
     * Collision destructor
     */
    ~Collision();
    /**
     * @return boolean value defining whether something is colliding with sensor
     */
    virtual bool isColliding();

  private:
        mraa_gpio_context m_gpio;
        mraa::MraaIo mraaIo;
    };
}
