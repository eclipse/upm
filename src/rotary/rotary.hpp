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

#include "rotary.hpp"
#include <interfaces/iAngle.hpp>

namespace upm {

/**
 * @brief Rotary Angle Sensor
 * @defgroup rotary libupm-rotary
 * @ingroup seeed analog ainput gsk
 */

/**
 * @library rotary
 * @sensor rotary
 * @comname Rotary Angle Sensor
 * @altname Rotary Potentiometer, Grove Rotary Angle Sensor
 * @type ainput
 * @man seeed
 * @web http://wiki.seeed.cc/Grove-Rotary_Angle_Sensor/
 * @con analog
 * @kit gsk
 *
 * @brief API for the Rotary Angle Sensor (Knob)
 *
 * Basic UPM module for the rotary angle sensor (knob) on analog. Provides a
 * set of functions to read the absolute pin value, degrees or radians, and
 * another set to do the same relative to the center of the knob's range.
 *
 * @image html rotary.jpg
 * @snippet rotary.cxx Interesting
 */
class Rotary : virtual public iAngle {
    public:
        /**
         * Rotary angle sensor constructor
         *
         * @param pin Number of the analog pin to use
         */
        Rotary(unsigned int pin);
        /**
         * Rotary destructor
         */
        ~Rotary();
        /**
         * Gets the absolute raw value from the AIO pin
         *
         * @return Unsigned value from the ADC
         */
        float abs_value();

        /**
         * Gets absolute raw degrees from the AIO pin
         *
         * @return Unsigned degrees from the ADC
         */
        float abs_deg();
        /**
         * Gets absolute raw radians from the AIO pin
         *
         * @return Unsigned radians from the ADC
         */
        float abs_rad();
        /**
         * Gets the relative value from the AIO pin
         *
         * @return Signed value from the ADC
         */
        float rel_value();
        /**
         * Gets relative degrees from the AIO pin
         *
         * @return Signed degrees from the ADC
         */
        float rel_deg();
        /**
         * Gets relative radians from the AIO pin
         *
         * @return Signed radians from the ADC
         */
        float rel_rad();

        /**
         * Get rotation value as raw degrees from the AIO pin.
         *
         * @return rotation value.
         */
        virtual float getAngle();

        std::string name(){ return "Rotary Angle Sensor";}
    private:
        mraa_aio_context m_aio;
        static const int m_max_angle = 300;
};
}
