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

namespace upm {

  /**
   * @brief Spectra Symbol Flex Sensor
   * @defgroup flex libupm-flex
   * @ingroup sparkfun analog flexfor
   */

  /**
   * @library flex
   * @sensor flex
   * @comname Resistive Flex Sensor
   * @type flexfor
   * @man sparkfun
   * @web https://www.sparkfun.com/products/8606
   * @con analog
   *
   * @brief API for the Spectra Symbol Flex Sensor
   *
   * A simple flex sensor. The resistance across the sensor increases when
   * flexed. Patented technology by Spectra Symbol, these sensors were used in
   * the original Nintendo* Power Glove.
   *
   * @image html flex.jpg
   * <br><em>Flex Sensor image provided by SparkFun* under
   * <a href=https://creativecommons.org/licenses/by/2.0/>
   * CC BY 2.0</a>.</em>
   *
   * @snippet flex.cxx Interesting
   */
  class Flex {
  public:
    /**
     * Flex sensor constructor
     *
     * @param pin Analog pin to use
     */
    Flex(int pin);
    /**
     * Flex sensor destructor
     */
    ~Flex();
    /**
     * @return Analog flex value
     */
    int value();

  private:
    mraa_aio_context m_aio;
  };
}


