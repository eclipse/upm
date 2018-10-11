/*
 * Authors: Brendan Le Foll <brendan.le.foll@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *          Sarah Knepper <sarah.knepper@intel.com>
 *          Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 - 2017 Intel Corporation.
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
#include <interfaces/iLight.hpp>

#include "light.h"

namespace upm {
    /**
     * @brief Analog Light Sensors Library
     * @defgroup alight libupm-light
     * @ingroup seeed analog light gsk
     */

    /**
     * @library alight
     * @sensor light
     * @comname Analog Light Sensor
     * @type light
     * @man seeed
     * @con analog
     * @kit gsk
     *
     * @brief API for the  Light Sensor
     *
     * The light sensor detects the intensity of the ambient light.
     * As the light intensity of the environment increases, the resistance
     * of the sensor decreases. This means the raw value from the
     * analog pin is larger in bright light and smaller in the dark.
     * A very approximate lux value can also be returned.
     *
     * @image html light.jpg
     * @snippet light.cxx Interesting
     */
    class Light : virtual public iLight {
    public:
        /**
         * Analog light sensor constructor
         *
         * @param pin Analog pin to use
         * @throws std::runtime_error on initialization error
         */
        Light(unsigned int pin);

        /**
         * Light destructor
         */
        ~Light();

        /**
         * Gets the raw value from the AIO pin
         *
         * @deprecated This function is deprecated use
         * getNormalized() instead.
         * @return Raw value from the ADC
         * @throws std::runtime_error on error
         */
        float raw_value();

        /**
         * Gets an approximate light value in lux from the sensor
         *
         * @return Approximate light reading in lux
         * @throws std::runtime_error on error
         */
        int value();

        /**
         * Gets an approximate light value in lux from the sensor
         *
         * @return Approximate light reading in lux
         * @throws std::runtime_error on error
         */
        virtual float getLuminance();

        /**
         * Set ADC reference voltage
         *
         * @param aref ADC reference voltage
         */
        void setAref(float aref);

        /**
         * Set sensor scale.  This scale is applied to the return value:
         *     counts = counts * scale
         *
         * @param scale count scale value used
         */
        void setScale(float scale);

        /**
         * Set sensor offset.  This offset is applied to the return value:
         *     value = value + offset
         *
         * @param offset count offset value used
         * @return Function result code
         */
        void setOffset(float offset);

        /**
         * Get sensor aref
         *
         * @param dev sensor context pointer
         * @return Sensor ADC reference voltage
         */
        float getAref();

        /**
         * Get sensor scale
         *
         * @param dev sensor context pointer
         * @return Sensor scale
         */
        float getScale();

        /**
         * Get sensor offset
         *
         * @param dev sensor context pointer
         * @return Sensor offset
         */
        float getOffset();

        /**
         * Read normalized value for sensor
         *
         * @param dev sensor context pointer
         * @param *value Normalized value (0.0 -> 1.0)
         * @return Function result code
         * @throws std::runtime_error on error
         */
        float getNormalized();

        /**
         * Read raw voltage from the sensor
         *
         * @param dev sensor context pointer
         * @param *value Raw sensor voltage
         * @return Function result code
         * @throws std::runtime_error on error
         */
        float getRawVolts();

        /* Sensor name
         *
         * @return Sensor name
         */
        std::string name()
        {
            return std::string("Light Sensor");
        }

    protected:
        light_context m_light;

    private:
        /* Disable implicit copy and assignment operators */
        Light(const Light&) = delete;
        Light &operator=(const Light&) = delete;
    };
}
