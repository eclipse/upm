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
#include <interfaces/iMoisture.hpp>

namespace upm {
  /**
   * @brief Moisture Sensors Library
   * @defgroup grovemoisture libupm-grovemoisture
   * @ingroup seeed analog liquid eak hak
   */

  /**
   * @library moisture
   * @sensor moisture
   * @comname Analog Moisture Sensor
   * @altname Grove Moisture Sensor
   * @type liquid
   * @man seeed
   * @con analog
   * @kit eak hak
   *
   * @brief API for the Moisture Sensor
   *
   * UPM module for the Moisture Sensor.
   * This sensor can be used to detect the moisture content
   * of soil or whether there is water around the sensor.
   * As the moisture content increases, so does the value that is read.
   * Note: this sensor is not designed to be left in soil
   * nor to be used outdoors.
   *
   * @image html moisture.jpg
   * @snippet moisture.cxx Interesting
   */
  class Moisture : virtual public iMoisture {
  public:
    /**
     * Analog moisture sensor constructor
     *
     * @param pin Analog pin to use
     */
    Moisture(int pin);
    /**
     * Moisture destructor
     */
    ~Moisture();
    /**
     * Gets the moisture value from the sensor
     *
     * @return Moisture reading
     */
    int value();

    /**
     * Gets the moisture value from the sensor
     *
     * @return Moisture reading
     */
    virtual int getMoisture();

  private:
    mraa_aio_context m_aio;
  };
}


