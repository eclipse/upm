/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * Ported based on original C++ code by:
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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

#include "hmc5883l.h"

namespace upm {

/**
 * @brief HMC5883L Magnetometer
 * @defgroup hmc5883l libupm-hmc5883l
 * @ingroup seeed i2c compass robok
 */

/**
 * @library hmc5883l
 * @sensor hmc5883l
 * @comname 3-axis Digital Compass
 * @altname Grove 3-Axis Digital Compass
 * @type compass
 * @man seeed
 * @con i2c
 * @web http://wiki.seeed.cc/Grove-3-Axis_Compass_V1.0/
 * @kit robok
 *
 * @brief API for the HMC5883L 3-Axis Digital Compass
 *
 * Honeywell [HMC5883L]
 * (http://www.adafruit.com/datasheets/HMC5883L_3-Axis_Digital_Compass_IC.pdf)
 * is a 3-axis digital compass. Communication with HMC5883L is simple and
 * all done through an I2C interface. Different breakout boards are available.
 * Typically, a 3V supply is all that is needed to power the sensor.
 *
 * @image html hmc5883l.jpeg
 * @snippet hmc5883l.cxx Interesting
 */
class Hmc5883l {
    public:
        /**
            * Creates an Hmc5883l object. The I2C address cannot be
            * changed, and is always 0x1e (7-bit).
            *
            * @param bus I2C bus number
            */
        Hmc5883l(int bus);

        /*
            * Returns the direction
            */
        float direction();

        /*
            * Returns the heading
            */
        float heading();

        /**
            * Returns a pointer to an int[3] that contains the coordinates as ints
            *
            * @return *int to an int[3]
            */
        const int16_t* coordinates();

        /**
            * Updates the values by reading from I2C
            *
            * @return 0 if successful
            */
        upm_result_t update();

        /**
            * Sets the magnetic declination for better calibration
            */
        void set_declination(float dec);

        /**
            * Gets the current magnetic declination value
            *
            * @return Magnetic declination as a floating-point value
            */
        float get_declination();

    protected:
        hmc5883l_context m_hmc5883l;

    private:
};
}
