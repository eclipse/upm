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
#include <sys/time.h>
#include <mraa/aio.h>

// default ADC resolution. 
#define TA12200_ADC_RES 1024

namespace upm {
/**
 * @brief TA12-200 Current Transformer
 * @defgroup ta12200 libupm-ta12200
 * @ingroup seeed analog electric
 */
/**
 * @library ta12200
 * @sensor ta12200
 * @comname AC Current Sensor
 * @altname Grove Electricity Sensor
 * @type electric
 * @man seeed
 * @web http://www.seeedstudio.com/wiki/Grove_-_Electricity_Sensor
 * @con analog
 *
 * @brief API for the TA12-200 Current Transformer
 * 
 *   UPM module for the TA12-200 current transformer found,
 *   for instance, in the Grove Electricity Sensor. 
 *   This module can measure AC moving through a wire at up 
 *   to 5 A.
 *
 * @image html ta12200.jpg
 * @snippet ta12200.cxx Interesting
 */
  class TA12200 {
  public:
    /**
     * TA12200 constructor
     *
     * @param pin Analog pin to use
     */
    TA12200(int pin);

    /**
     * TA12200 destructor
     */
    ~TA12200();

    /**
     * Returns the number of milliseconds elapsed since initClock()
     * was last called.
     *
     * @return Elapsed milliseconds
     */
    uint32_t getMillis();

    /**
     * Resets the clock
     *
     */
    void initClock();

    /**
     * Gets the conversion value from the sensor
     *
     * @return Highest value obtained over 1 second of measuring or -1 if error
     */
    int highestValue();

    /**
     * Computes the measured voltage
     *
     * @param val Value measured by highestValue()
     * @param res ADC resolution
     *
     * @return Measured current in mA
     */
    float milliAmps(unsigned int val, int res=TA12200_ADC_RES);

  private:
    struct timeval m_startTime;
    mraa_aio_context m_aio;
  };
}


