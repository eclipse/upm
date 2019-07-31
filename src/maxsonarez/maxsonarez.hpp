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
#include <interfaces/iDistance.hpp>

// EZ series is volts/512
#define MAXSONAREZ_RES  512

namespace upm {
  /**
   * @brief MaxSonar-EZ Family of Ultrasonic Rangers
   * @defgroup maxsonarez libupm-maxsonarez
   * @ingroup seeed analog sound
   */

  /**
   * @library maxsonarez
   * @sensor maxsonarez
   * @comname LV-MaxSonar-EZ Ultrasonic Ranger
   * @altname EZ1, EZ2, EZ3, EZ4
   * @type sound
   * @man sparkfun
   * @web https://www.sparkfun.com/products/8502
   * @con analog
   *
   * @brief API for the LV-MaxSonar-EZ Family of Ultrasonic Rangers
   *
   * Sensors of this family return an analog voltage corresponding to the
   * distance of an object from the sensor in inches. They have a
   * resolution of about 9.7 millivolts per inch with an analog reference voltage of 5.0 V. The
   * differences between various versions (EZ1, EZ2, etc.) are
   * related to the narrowness of the beam angle.
   *
   * This class supports these sensors with an analog input only.
   *
   * This driver was developed using an LV-MaxSonar-EZ3 ultrasonic ranger.
   *
   * @image html maxsonarez.jpg
   * <br><em>LV-MaxSonar-EZ Ultrasonic Ranger image provided by SparkFun* under
   * <a href=https://creativecommons.org/licenses/by/2.0/>
   * CC BY 2.0</a>.</em>
   *
   * @snippet maxsonarez.cxx Interesting
   */

  class MAXSONAREZ : virtual public iDistance {
  public:

    /**
     * MAXSONAREZ constructor
     *
     * @param pin Analog pin to use
     * @param aref Analog reference voltage; default is 5.0 V
     */
    MAXSONAREZ(int pin, float aref=5.0);

    /**
     * MAXSONAREZ destructor
     */
    ~MAXSONAREZ();

    /**
     * Gets the distance to the object in inches
     *
     * @return Distance to the object in inches
     */
    int inches();

    /**
     * Gets the distance to the object in cm
     *
     * @return Distance to the object in cm
     */
    virtual float getDistance();

  private:
    mraa_aio_context m_aio;
    float m_aref;
    // ADC resolution
    int m_aRes;
    // computed volts per inch
    float m_vI;
  };
}


