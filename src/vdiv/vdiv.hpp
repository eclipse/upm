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
#include <iostream>
#include <stdint.h>
#include <mraa/aio.h>
#include <interfaces/iVDiv.hpp>

// reference voltage in millivolts
#define VDIV_VREF  4980

// default ADC resolution
#define VDIV_ADC_RES 1024

namespace upm {
  /**
   * @brief Voltage Divider Sensor
   * @defgroup vdiv libupm-vdiv
   * @ingroup seeed analog electric robok
   */

  /**
   * @library vdiv
   * @sensor vdiv
   * @comname Analog Voltage Divider Sensor
   * @altname Grove Voltage Divider
   * @type electric
   * @man seeed
   * @con analog
   * @kit robok
   *
   * @brief API for the Voltage Divider Sensor
   *
   * UPM module for the Voltage Divider sensor
   *
   * @image html vdiv.jpg
   * @snippet vdiv.cxx Interesting
   */
  class VDiv : virtual public iVDiv {
  public:
    /**
     * Voltage Divider sensor constructor
     *
     * @param pin Analog pin to use
     */
    VDiv(int pin);

    /**
     * Voltage Divider destructor
     */
    ~VDiv();

    /**
     * Gets the conversion value from the sensor
     *
     * @param samples Specifies how many samples to average over
     * @return Average ADC conversion value
     */
    unsigned int value(unsigned int samples);

    /**
     * Gets the conversion value from the sensor
     *
     * @return ADC conversion value
     */
    virtual unsigned int getValue();

    /**
     * Computes the measured voltage
     *
     * @param gain Gain switch, example: either 3 or 10 grove vdiv
     * @param val Measured voltage (from value())
     * @param vref Reference voltage in millivolts
     * @param res ADC resolution
     *
     * @return Measured voltage
     */
    float computedValue(uint8_t gain, unsigned int val, int vref=VDIV_VREF,
                        int res=VDIV_ADC_RES);

  private:
    mraa_aio_context m_aio;
  };
}
