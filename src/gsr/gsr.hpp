/*
 * Author: Zion Orent <zorent@ics.com>
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
#include <mraa/aio.h>
#include <interfaces/iEC.hpp>

namespace upm {
  /**
   * @brief Grove GSR Galvanic Skin Response Sensor
   * @defgroup gsr libupm-gsr
   * @ingroup seeed analog electric
   */

  /**
   * @library gsr
   * @sensor gsr
   * @comname Galvanic Skin Response (GSR) Sensor
   * @type electric
   * @man seeed
   * @con analog
   *
   * @brief API for the Grove GSR Galvanic Skin Response Sensor
   *
   * Measures the electrical conductance of skin
   * to measure strong emotional reactions.
   * In other words, it measures sweat on your fingers
   * as an indicator of strong emotional reactions.
   *
   * @image html gsr.jpg
   * @snippet gsr.cxx Interesting
   */
  class GSR : virtual public iEC {
  public:
    /**
     * Grove GSR sensor constructor
     *
     * @param pin Analog pin to use
     */
    GSR(int pin);
    /**
     * GSR destructor
     */
    ~GSR();

    /**
     * Calibrates the Grove GSR sensor
     */
    void calibrate();

    /**
     * Gets the electrical conductance of the skin from the sensor
     *
     * @return Electrical conductance of the skin
     */
    int value();

    /**
     * Gets the electrical conductance of the skin from the sensor
     *
     * @return Electrical conductance of the skin
     */
    virtual float getECValue();

  private:
    mraa_aio_context m_aio;
  };
}
