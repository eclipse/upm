/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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
#include <iostream>

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

    class MB704X {
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


    protected:
        // mb704x device context
        mb704x_context m_mb704x;

    private:
        /* Disable implicit copy and assignment operators */
        MB704X(const MB704X&) = delete;
        MB704X &operator=(const MB704X&) = delete;
    };
}
