/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <string>
#include <math.h>
#include <mraa/aio.h>
#include <mraa/gpio.h>

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
   * @brief ECS1030 Electricity Sensor library
   * @defgroup ecs1030 libupm-ecs1030
   * @ingroup sparkfun analog electric
   */

  /**
   * @library ecs1030
   * @sensor ecs1030
   * @comname ECS1030 Non-Invasive Current Sensor
   * @type electric
   * @man sparkfun
   * @con analog
   *
   * @brief C++ API for the ECS1030 Non-Invasive Current/Electricity Sensor
   *
   * This non-invasive current sensor can be clamped around the supply line of
   * an electrical load to tell you how much current is passing through it. It
   * does this by acting as an inductor and responding to the magnetic field
   * around a current-carrying conductor. This particular current sensor will
   * measure a load up to 30 Amps which makes it great for building your own
   * energy monitors.
   *
   * @snippet ecs1030.cxx Interesting
   */
class ECS1030 {
    public:
        static const uint8_t DELAY_MS  = 20000 / NUMBER_OF_SAMPLES; /* 1/50Hz is 20ms period */
        static const uint8_t VOLT_M    = 5.1 / 1023;
        static const uint8_t R_LOAD    = 2000.0 / CURRENT_RATIO;

        /**
         * Instanciates a ECS1030 (current sensor) object
         *
         * @param pinNumber number of the data pin
         */
        ECS1030 (uint8_t pinNumber);

        /**
         * ECS1030 object destructor, basicaly it close the GPIO.
         */
        ~ECS1030 ();

        /**
         * Return currency data for the sampled period
         */
        double getCurrency_A ();

        /**
         * Return power data for the sampled period
         */
        double getPower_A ();

        /**
         * Return currency data for the sampled period
         */
        double getCurrency_B ();

        /**
         * Return power data for the sampled period
         */
        double getPower_B ();

        /**
         * Return name of the component
         */
        std::string name() {
            return m_name;
        }
    private:
        std::string         m_name;
        mraa_aio_context    m_dataPinCtx;

        double              m_calibration;
        int                 m_lastSample;
        double              m_lastFilter;
        int                 m_sample;
        double              m_filteredSample;
};
}
