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
   * @brief INA132 Differential Amplifier Sensor
   * @defgroup ina132 libupm-ina132
   * @ingroup seeed analog electric
   */
  /**
   * @library ina132
   * @sensor ina132
   * @comname Differential Amplifier
   * @altname INA132
   * @type electric
   * @man seeed
   * @con analog
   * @web http://wiki.seeed.cc/Grove-Differential_Amplifier_v1.2/
   *
   * @brief API for the INA132 Differential Amplifier Sensor
   *
   * The INA132 Differential Amplifier Sensor is designed
   * for precise differential-input amplification.
   *
   * This sensor was tested amplifying the signal
   * from a Weight Sensor (Load Cell) 0-500g
   *
   * @image html ina132.jpg
   * @snippet ina132.cxx Interesting
   */
  class INA132 {
  public:
    /**
     * INA132 constructor
     *
     * @param pin Analog pin to use
     */
    INA132(int pin);
    /**
     * INA132 destructor
     */
    ~INA132();

    /**
     * Measures precise differential-input amplification from the sensor
     *
     * @return Precise differential-input amplification
     */
    float value();

  private:
    mraa_aio_context m_aio;
  };
}


