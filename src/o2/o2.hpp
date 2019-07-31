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

#include <interfaces/iGas.hpp>

namespace upm {
  /**
   * @brief Oxygen Gas Sensor
   * @defgroup o2 libupm-o2
   * @ingroup seeed analog gaseous
   */

  /**
   * @library o2
   * @sensor o2
   * @comname Oxygen (O2) Concentration Sensor
   * @altname Grove O2 Sensor
   * @type gaseous
   * @man seeed
   * @con analog
   * @web http://wiki.seeed.cc/Grove-Gas_Sensor-O2/
   *
   * @brief API for the Grove O2 Oxygen Gas Sensor
   *
   * The Grove O2 Oxygen Gas sensor measures the oxygen concentration in the air
   *
   * @image html o2.jpg
   * @snippet o2.cxx Interesting
   */
  class O2: virtual public iGas {
  public:
    /**
     * Grove O2 Oxygen Gas sensor constructor
     *
     * @param pin Analog pin to use
     */
    O2(int pin);
    /**
     * O2 destructor
     */
    ~O2();
    /**
     * Measures O2 from the sensor
     *
     * @return Oxygen concentration as voltage
     */
    float voltageValue();

    /**
     * Measures O2 from the sensor
     *
     * @return Oxygen concentration as PPM
     */
    float getConcentration();

  private:
    mraa_aio_context m_aio;
  };
}
