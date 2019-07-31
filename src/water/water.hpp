/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include "water.h"
#include <interfaces/iWater.hpp>

namespace upm {
  /**
   * @brief Water Sensor
   * @defgroup water libupm-water
   * @ingroup seeed gpio liquid eak
   */

  /**
   * @library water
   * @sensor water
   * @comname Water Detection Sensor
   * @type liquid
   * @man seeed
   * @con gpio
   * @kit eak
   *
   * @brief API for the Water Sensor
   *
   * UPM module for the Water sensor
   *
   * @image html water.jpg
   * @snippet water.cxx Interesting
   */
  class Water : virtual public iWater {
  public:
    /**
     * digital water sensor constructor
     *
     * @param pin Digital pin to use
     */
    Water(unsigned int pin);
    /**
     * Water destructor
     */
    ~Water();
    /**
     * Gets the water (wet/not wet) value from the sensor
     *
     * @return True if the sensor is wet, false otherwise
     */
    virtual bool isWet();

  private:
    /* Disable implicit copy and assignment operators */
    Water(const Water&) = delete;
    Water &operator=(const Water&) = delete;

    water_context m_water;
  };
}
