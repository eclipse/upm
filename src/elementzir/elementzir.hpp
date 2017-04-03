/*
 * Author: Saloni Jain <saloni.jain@tcs.com>
 * Author: Niti Rohilla <niti.rohilla@tcs.com>
 * Copyright (c) 2014-2015 Intel Corporation.
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
#include <mraa/aio.h>
#include <mraa/gpio.h>
#include <mraa/pwm.h>
#include <sys/time.h>

namespace upm {
/**
 * @brief ELEMENTZ IR Proximity Sensor library
 * @defgroup elementzir libupm-elementzir
 * @ingroup generic gpio light
 */

/**
 * @library elementzir
 * @sensor elementzir
 * @comname Infrared Proximity Sensor
 * @type light
 * @man elementz
 * @con gpio
 * @web https://elementzonline.com/ir-infrared-proximity-obstacle-sensor-module-95
 *
 * @brief API for the Elementz Infrared Proximity Sensor
 *
 * This module defines the Infrared interface for libelementz
 *
 * @image html elementzir.jpg
 * @snippet elementzir.cxx Interesting
 */
class ElementzIR {
    public:
        /**
         * Instantiates an ElementzIR object
         *
         * @param pin Digital pin to use
         * @param fptr Function pointer to handle rising-edge
         * interrupts
         */
        ElementzIR (int pin);
        /**
         * ElementzIR object destructor
         */
        ~ElementzIR ();

        /**
         * Gets the status of the pin; true means black has been detected
         *
         * @Return True if sensor has detected black
         */
        bool obstacleDetected ();

        /**
         * Returns the name of the sensor
         */
        std::string name()
        {
            return m_name;
        }

    private:

        mraa_gpio_context   m_pinCtx;

        std::string         m_name;
};

}
