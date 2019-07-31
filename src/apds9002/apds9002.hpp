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
#include <mraa/initio.hpp>
#include <interfaces/iLight.hpp>

namespace upm {
  /**
   * @brief APDS-9002 Luminance Sensor
   * @defgroup apds9002 libupm-apds9002
   * @ingroup seeed analog light
   */

  /**
   * @library apds9002
   * @sensor apds9002
   * @comname Ambient Light Photo Sensor
   * @altname Grove Luminance Sensor
   * @type light
   * @man seeed
   * @con analog
   * @web https://www.seeedstudio.com/Grove-Luminance-Sensor-p-1941.html
   *
   * @brief UPM module for the APDS-9002 Luminance Sensor
   *
   * This sensor transforms luminous intensity to output analog values.
   * It uses an APDS-9002 ambient light sensor.
   *
   * @image html apds9002.jpg
   * @snippet apds9002.cxx Interesting
   */

  class APDS9002 : virtual public iLight {
  public:
    /**
     * APDS-9002 luminance sensor constructor
     *
     * @param pin Analog pin to use
     */
    APDS9002(int pin);
    /**
     * Instantiates APDS-9002 Luminance Sensor based on a given string.
     *
     * @param initStr string containing specific information for APDS-9002 initialization.
     */
    APDS9002(std::string initStr);
    /**
     * APDS9002 destructor
     */
    ~APDS9002();
    /**
     * Gets the luminance value from the sensor
     *
     * @return Luminance value
     */
    int value();

    /**
     * Gets the luminance value from the sensor
     *
     * @return The measured light intensity value in Lux
     */
    virtual float getLuminance();

  private:
    mraa_aio_context m_aio;
    mraa::MraaIo mraaIo;
  };
}


