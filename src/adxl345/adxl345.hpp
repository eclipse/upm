/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <mraa/i2c.hpp>
#include <mraa/initio.hpp>

// #include <interfaces/iAcceleration.hpp>

#define READ_BUFFER_LENGTH 6

namespace upm {

/**
 * @brief ADXL345 Accelerometer
 * @defgroup adxl345 libupm-adxl345
 * @ingroup seeed i2c accelerometer
 */

/**
 * @library adxl345
 * @sensor adxl345
 * @comname 3-axis, +/- 2/4/8/16 g Digital Accelerometer
 * @altname Grove 3-Axis Digital Accelerometer (16g)
 * @type accelerometer
 * @man seeed
 * @con i2c
 * @web http://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf
 *
 * @brief API for the ADXL345 3-Axis Digital Accelerometer
 *
 * ADXL345 is a 3-axis digital accelerometer.
 * (http://www.seeedstudio.com/wiki/images/2/2c/ADXL345_datasheet.pdf)
 * The sensor has configurable resolutions to measure +/- 2g, +/- 4g, +/- 8g, or +/- 16g.
 * Note: The Grove* version of the sensor is incompatible with and not detected
 * on the I2C bus by the Intel(R) Edison using an Arduino* breakout board at 5V
 * (3V works fine).
 *
 * @image html adxl345.jpeg
 * @snippet adxl345.cxx Interesting
 */
// class Adxl345: virtual public iAcceleration {
class Adxl345 {
public:
    /**
     * Creates an ADXL345 object
     *
     * @param bus Number of the used I2C bus
     */
    Adxl345(int bus);

    /**
     * Instantiates ADXL345 Accelerometer based on a given string.
     *
     * @param initStr string containing specific information for ADXL345 initialization.
     */
    Adxl345(std::string initStr);

    /**
     * there is no need for a ADXL345 object destructor
     * ~Adxl345();
     */
    ~Adxl345();

    /**
     * Returns a pointer to a float[3] that contains acceleration (g) forces
     *
     * @return float* to a float[3]
     */
    float* getAcceleration();

    /**
     * get acceleration values
     * 
     * @return stl vector of size 3 representing the 3 axis
     */
    // virtual std::vector<float> getAcceleration();

    /**
     * Returns a pointer to an int[3] that contains the raw register values
     * for X, Y, and Z
     *
     * @return int* to an int[3]
     */
    int16_t* getRawValues();

    /**
     * Returns the scale the accelerometer is currently set up to: 2, 4, 8,
     * or 16
     *
     * @return uint with the current scale value
     */
    uint8_t getScale();

    /**
     * Updates the acceleration values from the I2C bus
     *
     * @return 0 if successful
     */
    mraa::Result update();
private:
    float m_accel[3];
    float m_offsets[3];
    int16_t m_rawaccel[3];
    uint8_t m_buffer[READ_BUFFER_LENGTH];
    mraa::I2c m_i2c;
    mraa::MraaIo mraaIo;
};

}
