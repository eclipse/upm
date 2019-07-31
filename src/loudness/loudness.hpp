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

#include <iostream>
#include <string>
#include <mraa/aio.hpp>

namespace upm {
  /**
   * @brief Loudness Sensors Library
   * @defgroup loudness libupm-loudness
   * @ingroup dfrobot seeed analog sound
   */

  /**
   * @library loudness
   * @sensor loudness
   * @comname Loudness Sensor
   * @altname Grove loudness hyld9767
   * @type sound
   * @man dfrobot seeed
   * @web http://www.dfrobot.com/index.php?route=product/product&product_id=83
   * @con analog
   *
   * @brief API for the Loudness Sensor
   *
   * This sensor family returns an analog voltage proportional to the
   * loudness of the ambient environment.  It's output does not
   * correspond to a particular sound level in decibels.  The higher
   * the output voltage, the louder the ambient noise level.
   *
   * This device uses an electret microphone for sound input.
   *
   * This driver was developed using the DFRobot Loudness Sensor V2
   * and the Grove Loudness sensor.
   *
   * @image html groveloudness.jpg
   * @snippet loudness.cxx Interesting
   */

  class Loudness {
  public:

    /**
     * Loudness constructor
     *
     * @param pin Analog pin to use
     * @param aref Analog reference voltage; default is 5.0 V
     */
    Loudness(int pin, float aref=5.0);

    /**
     * Loudness destructor
     */
    ~Loudness();

    /**
     * Returns the voltage detected on the analog pin
     *
     * @return The detected voltage
     */
    float loudness();

  protected:
    mraa::Aio m_aio;

  private:
    float m_aref;
    // ADC resolution
    int m_aRes;
  };
}


