/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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
#include <interfaces/iDistance.hpp>

#include <stdlib.h>
#include <unistd.h>

#include "mb704x.h"

namespace upm {
    /**
     * @brief MB704x MaxSonar-WR Ultrasonic Ranger
     * @defgroup mb704x libupm-mb704x
     * @ingroup i2c sound
     */

    /**
     * @library mb704x
     * @sensor mb704x
     * @comname I2C MaxSonar WR Ultrasonic Ranger
     * @altname MB7040 MB7047 MB7247
     * @type sound
     * @man maxbotix
     * @con i2c
     * @web http://www.maxbotix.com/Ultrasonic_Sensors/I2C_Distance_Sensors.htm
     *
     * @brief API for the MB704x MaxSonar-WR Ultrasonic Ranger
     *
     * This driver was tested with the MB704x MaxSonar-WR Ultrasonic
     * Ranger using the long distance, weather resistant horn.
     * Depending on your sensor type and horn, tanges from 20cm to
     * 750cm are possible.
     *
     * @snippet mb704x.cxx Interesting
     */

    class MB704X : virtual public iDistance {
    public:

        /**
         * Initialize a device context.
         *
         * @param bus Specify which the I2C bus to use.  The default is 0.
         * @param addr Specify the I2C address to use.  The default is 112.
         */
        MB704X(unsigned int bus=0, unsigned int addr=112);

        /**
         * MB704X object destructor
         */
        ~MB704X();

        /**
         * Query the device for a range reading.  The range will be
         * reported in centimeters (cm).
         *
         * @param dev Device context
         * @return Measured range, -1 on error. The range is reported in
         * centimeters (cm).
         */
        int getRange();

        /**
         * Gets the distance to the object in cm
         *
         * @return Distance to the object in cm
         */
        virtual float getDistance();
    protected:
        // mb704x device context
        mb704x_context m_mb704x;

    private:
        /* Disable implicit copy and assignment operators */
        MB704X(const MB704X&) = delete;
        MB704X &operator=(const MB704X&) = delete;
    };
}
