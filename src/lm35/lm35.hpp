/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
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

#include <iostream>
#include <string>
#include "lm35.h"

namespace upm {
    /**
     * @brief DFRobot LM35 Linear Temperature Sensor
     * @defgroup lm35 libupm-lm35
     * @ingroup dfrobot analog temp
     */

    /**
     * @library lm35
     * @sensor lm35
     * @comname DFRobot LM35 Linear Temperature Sensor
     * @altname LM35
     * @type temp
     * @man dfrobot
     * @web http://www.dfrobot.com/index.php?route=product/product&product_id=76
     * @con analog
     *
     * @brief API for the DFRobot LM35 Linear Temperature Sensor
     *
     * This sensor returns an analog voltage proportional to the
     * temperature of the ambient environment.
     *
     * It has a range of 2C to 150C.
     *
     * This driver was developed using the DFRobot LM35 Linear
     * Temperature Sensor
     *
     * @image html lm35.jpg
     * @snippet lm35.cxx Interesting
     */

    class LM35 {
    public:

        /**
         * LM35 constructor
         *
         * @param pin Analog pin to use
         * @param aref Analog reference voltage; default is 5.0 V
         */
        LM35(int pin, float aref=5.0);

        /**
         * LM35 destructor
         */
        ~LM35();

        /**
         * Returns the temperature in degrees Celsius
         *
         * @return The Temperature in degrees Celsius
         */
        float getTemperature();

        /**
         * Set sensor scale.  This scale is applied to the return values
         * before the offset is applied.  Default is 1.0.
         *
         * @param scale Scale to apply.
         */
        void setScale(float scale);

        /**
         * Set sensor offset.  This offset is applied to the return values
         * before scaling.  Default is 0.0.
         *
         * @param offset Offset to apply.
         */
        void setOffset(float offset);

    protected:
        lm35_context m_lm35;

    private:
    };
}


