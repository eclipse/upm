/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * Ported based on original C++ code by:
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
