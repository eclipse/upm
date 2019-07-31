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
#include <interfaces/iTemperature.hpp>

namespace upm {
/**
 * @brief NTC Temperature Sensors Library
 * @defgroup temperature libupm-temperature
 * @ingroup seeed analog temp gsk
 */
/**
 * @library temperature
 * @sensor temperature
 * @comname Analog Temperature Sensor
 * @altname Grove Temperature Sensor
 * @type temp
 * @man seeed
 * @web http://wiki.seeed.cc/Grove-Temperature_Sensor_V1.2/
 * @con analog
 * @kit gsk
 *
 * @brief API for NTC Temperature Sensors
 *
 * Basic UPM module for analog temperature sensors. These sensors use a NTC
 * thermistor to measure ambient temperature. This driver was developed using
 * the Grove Temperature Sensor. The conversion formula has been updated
 * to work with versions 1.1 and 1.2 of the sensor. For the older v1.0
 * sensor you will have to specify R0 and B values when initializing the
 * device. The range of this sensor is -40 to 125 C and accuracy is +/- 1.5 C.
 *
 * @image html temp.jpg
 * @snippet temperature.cxx Interesting
 */
class Temperature : virtual public iTemperature {
    public:
        /**
         * Analog temperature sensor constructor
         *
         * @param pin Analog pin to use
         * @param scale Scaling factor for raw analog value from the ADC,
         * useful for mixed 3.3V/5V boards, default 1.0
         * @param r0 zero power resistance, this is 100K (default) for
         * v1.1-v1.2 and 10K for v1.0 of the sensor
         * @param b thermistor nominal B constant, this is 4275 (default) for
         * v1.1-v1.2 and 3975 for v1.0 of the sensor
         */
        Temperature(unsigned int pin, float scale = 1.0, int r0 = 100000, int b = 4275);
        /**
         * Analog temperature sensor destructor
         */
        ~Temperature();
        /**
         * Gets the raw value from the AIO pin
         *
         * @return Raw value from the ADC
         */
        float raw_value();
        /** Provided for compatibility with old grove base class
         *
         * @return Sensor name
         */
        std::string name()
        {
            return "Temperature sensor";
        }
        /**
         * Gets the temperature in Celsius from the sensor
         *
         * @return Normalized temperature in Celsius
         */
        int value();

        /**
         * Gets the temperature in Celsius from the sensor
         *
         * @return Normalized temperature in Celsius
         */
        virtual float getTemperature();
    private:
        mraa_aio_context m_aio;
        float m_scale;
        int m_r0;
        int m_b;

};
}
