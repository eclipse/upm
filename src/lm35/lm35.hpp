/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <iostream>
#include <string>
#include "lm35.h"
#include <interfaces/iTemperature.hpp>

namespace upm {
    /**
     * @brief LM35 Linear Temperature Sensor
     * @defgroup lm35 libupm-lm35
     * @ingroup dfrobot analog temp
     */

    /**
     * @library lm35
     * @sensor lm35
     * @comname Analog Temperature Sensor
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

    class LM35 : virtual public iTemperature {
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
        virtual float getTemperature();

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
        /* Disable implicit copy and assignment operators */
        LM35(const LM35&) = delete;
        LM35 &operator=(const LM35&) = delete;
    };
}
