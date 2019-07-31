/*
 * Authors: Brendan Le Foll <brendan.le.foll@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *          Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 - 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <mraa/aio.hpp>

namespace upm {
 /**
  * @brief Slide Sensor Library
  * @defgroup slide libupm-slide
  * @ingroup seeed analog ainput
  */

 /**
  * @library slide
  * @sensor slide
  * @comname Slide Potentiometer
  * @altname Grove Slide
  * @type ainput
  * @man seeed
  * @web http://wiki.seeed.cc/Grove-Slide_Potentiometer/
  * @con analog
  *
  * @brief API for the Slide Potentiometer
  *
  * Basic UPM module for the slide potentiometer on analog that
  * returns either a raw value or a scaled voltage value.
  *
  * @image html slide.jpg
  * @snippet slide.cxx Interesting
  */
class Slide {
    public:
        /**
         * Analog slide potentiometer constructor
         *
         * @param pin Number of the analog pin to use
         *
         * @param ref_voltage Reference voltage the board is set to, as a floating-point value; default is 5.0V
         */
        Slide(unsigned int pin, float ref_voltage = 5.0);
        /**
         * Slide destructor
         */
        ~Slide();
        /**
         * Gets the raw value from the AIO pin
         *
         * @return Raw value from the ADC
         */
        float raw_value();
        /**
         * Gets the voltage value from the pin
         *
         * @return Voltage reading based on the reference voltage
         */
        float voltage_value();
        /**
         * Gets the board's reference voltage passed on object initialization
         *
         * @return Reference voltage the class was set for
         */
        float ref_voltage();

        /* Gets the sensor name
         *
         * @return sensor name
         */
        std::string name() {return "Slide Potentiometer";}
    private:
        mraa_aio_context m_aio;
        float m_ref_voltage;
};
}
