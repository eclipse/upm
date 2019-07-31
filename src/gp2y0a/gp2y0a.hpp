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
#include <mraa/aio.h>
#include <interfaces/iProximity.hpp>

namespace upm {
  /**
   * @brief GP2Y0A-based IR Proximity Sensors
   * @defgroup gp2y0a libupm-gp2y0a
   * @ingroup seeed analog light
   */

  /**
   * @library gp2y0a
   * @sensor gp2y0a
   * @comname Analog Infrared (IR) Based Distance Sensor
   * @altname Grove 80cm IR Proximity Sensor
   * @type light
   * @man seeed
   * @con analog
   * @web http://wiki.seeed.cc/Grove-80cm_Infrared_Proximity_Sensor/
   *
   * @brief API for the GP2Y0A family of IR Proximity Sensors
   *
   * Sensors of this family return an analog voltage corresponding to the distance
   * of an object from the sensor. The voltage is lower when objects
   * are far away; the voltage increases as objects get closer
   * to the sensor.
   *
   * @image html gp2y0a.jpg
   * @snippet gp2y0a.cxx Interesting
   */
  class GP2Y0A : virtual public iProximity {
  public:

    /**
     * GP2Y0A sensor constructor
     *
     * @param pin Analog pin to use
     */
    GP2Y0A(int pin);

    /**
     * GP2Y0A destructor
     */
    ~GP2Y0A();

    /**
     * Gets an average voltage value from the sensor
     *
     * @param aref Reference voltage in use (usually 5.0V or 3.3V)
     * @param samples Number of samples to average over
     * @return Average voltage reading
     */
    float value(float aref, uint8_t samples);

    /**
     * Gets the voltage value from the sensor from a single sample
     * A refernce voltage of 5.0V is used for the reading.
     *
     * @return Voltage reading from a single sample
     */
    virtual float getValue();



  private:
    mraa_aio_context m_aio;
    // ADC resolution
    int m_aRes;
  };
}
