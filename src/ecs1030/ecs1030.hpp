/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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
#include <vector>
#include <math.h>
#include <mraa/aio.h>
#include <mraa/gpio.h>
#include <mraa/initio.hpp>

namespace upm {

#define NUMBER_OF_SAMPLES  500
#define ADC_RESOLUTION     1024
#define SUPPLYVOLTAGE      5100
#define CURRENT_RATIO      2000.0

#define HIGH               1
#define LOW                0

#define TRUE               HIGH
#define FALSE              LOW

  /**
   * @brief ECS1030 Electricity Sensor
   * @defgroup ecs1030 libupm-ecs1030
   * @ingroup sparkfun analog electric
   */

  /**
   * @library ecs1030
   * @sensor ecs1030
   * @comname Non-invasive Current Sensor
   * @type electric
   * @man sparkfun
   * @web https://www.sparkfun.com/products/11005
   * @con analog
   *
   * @brief API for the ECS1030 Non-Invasive Current/Electricity Sensor
   *
   * This non-invasive current sensor can be clamped around the supply line of
   * an electrical load to tell you how much current is passing through it. It
   * does this by acting as an inductor and responding to the magnetic field
   * around a current-carrying conductor. This particular current sensor
   * measures a load up to 30 A, which makes it great for building your own
   * energy monitors.
   *
   * @image html ecs1030.jpg
   * <br><em>ECS1030 Sensor image provided by SparkFun* under
   * <a href=https://creativecommons.org/licenses/by/2.0/>
   * CC BY 2.0</a>.</em>
   *
   * @snippet ecs1030.cxx Interesting
   */
class ECS1030 {
    public:
        static const uint8_t DELAY_MS  = 20000 / NUMBER_OF_SAMPLES; /* 1/50Hz is 20ms period */
        static const uint8_t VOLT_M    = 5.1 / 1023;
        static const uint8_t R_LOAD    = 2000.0 / CURRENT_RATIO;

        /**
         * Instantiates an ECS1030 object
         *
         * @param pinNumber Number of the data pin
         */
        ECS1030 (int pinNumber);

        /**
         * Instantiates ECS1030 sensor object based on a given string.
         *
         * @param initStr string containing specific information for ECS1030 sensor initialization.
         */
        ECS1030 (std::string initStr);

        /**
         * ECS1030 object destructor; basically, it closes the GPIO.
         */
        ~ECS1030 ();

        /**
         * Returns electric current data for a sampled period
         */
        double getCurrency_A ();

        /**
         * Returns power data for a sampled period
         */
        double getPower_A ();

        /**
         * Returns electric current data for a sampled period
         */
        double getCurrency_B ();

        /**
         * Returns power data for a sampled period
         */
        double getPower_B ();

        /**
         * Returns the name of the component
         */
        std::string name() {
            return m_name;
        }
    private:
        std::string         m_name;
        mraa_aio_context    m_dataPinCtx;
        mraa::MraaIo        mraaIo;

        double              m_calibration;
        int                 m_lastSample;
        double              m_lastFilter;
        int                 m_sample;
        double              m_filteredSample;
};
}
