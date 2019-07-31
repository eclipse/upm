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

#include <stdint.h>
#include <sys/time.h>

#include <mraa/gpio.hpp>
#include <mraa/aio.hpp>
#include <mraa/initio.hpp>

#define AD8232_DEFAULT_AREF  3.3

namespace upm {

  /**
   * @brief AD8232 Heart Rate Monitor
   * @defgroup ad8232 libupm-ad8232
   * @ingroup sparkfun gpio analog medical
   */

  /**
   * @library ad8232
   * @sensor ad8232
   * @comname Single Lead Heart Rate Monitor
   * @type medical
   * @man sparkfun
   * @web https://www.sparkfun.com/products/12650
   * @con gpio analog
   *
   * @brief UPM module for the AD8232 Heart Rate Monitor
   *
   * Note: this sensor must be driven at 3.3V only.
   *
   * This module simply spits out the ADC values reported by the sensor, with
   * the intent to send that data, via serial or network port, somewhere to
   * another piece of software running on a computer that plots the data for
   * you, like an EKG.
   *
   * Processing (https://www.processing.org/) is software
   * that should work, using information from the SparkFun* website.
   *
   * This example just dumps the raw data:
   *
   * @image html ad8232.jpg
   * <br><em>AD8232 Heart Rate Monitor image provided by SparkFun under
   * <a href=https://creativecommons.org/licenses/by/2.0/>
   * CC BY 2.0</a>.</em>
   *
   * @snippet ad8232.cxx Interesting
   */

  class AD8232 {
  public:

    /**
     * AD8232 constructor
     *
     * @param loPlus Digital pin to use for LO+
     * @param loMinus Digital pin to use for LO-
     * @param output Analog pin to read the data
     * @param aref Analog voltage reference
     */
    AD8232(int loPlus, int loMinus, int output, float aref=AD8232_DEFAULT_AREF);

    /**
     * Instantiates AD8232 sensor object based on a given string.
     *
     * @param initStr string containing specific information for AD8232 sensor initialization.
     */
    AD8232(std::string initStr);

    /**
     * AD8232 destructor
     */
    ~AD8232();

    /**
     * Returns the current ADC value for the device output pin.  If an
     * LO (leads off) event is detected, 0 is returned.
     *
     * @return ADC value
     */
    int value();

  private:
    mraa::Gpio m_gpioLOPlus;
    mraa::Gpio m_gpioLOMinus;
    mraa::Aio m_aioOUT;
    mraa::MraaIo mraaIo;

    float m_aref;
    int m_ares;
  };
}


