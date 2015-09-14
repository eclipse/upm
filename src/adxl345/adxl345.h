/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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

#include <mraa/i2c.hpp>

#define READ_BUFFER_LENGTH 6

namespace upm {

/**
 * @brief ADXL345 Accelerometer library
 * @defgroup adxl345 libupm-adxl345
 * @ingroup seeed i2c accelerometer
 */

/**
 * @library adxl345
 * @sensor adxl345
 * @comname ADXL345 3-Axis Digital Accelerometer
 * @altname Grove 3-Axis Digital Accelerometer (16g)
 * @type accelerometer
 * @man seeed
 * @con i2c
 *
 * @brief API for the ADXL345 3-Axis Digital Accelerometer
 *
 * ADXL345 is a 3-axis digital accelerometer.
 * (http://www.seeedstudio.com/wiki/images/2/2c/ADXL345_datasheet.pdf)
 * The sensor has configurable resolutions to measure ±2g, ±4g, ±8g, or ±16g.
 * Note: The Grove* version of the sensor is incompatible with and not detected
 * on the I2C bus by the Intel(R) Edison using an Arduino* breakout board at 5V
 * (3V works fine).
 *
 * @image html adxl345.jpeg
 * @snippet adxl345.cxx Interesting
 */
class Adxl345 {
public:
    /**
     * Creates an ADXL345 object
     *
     * @param bus Number of the used I2C bus
     */
    Adxl345(int bus);

    /**
     * there is no need for a ADXL345 object destructor
     * ~Adxl345();
     */

    /**
     * Returns a pointer to a float[3] that contains acceleration (g) forces
     *
     * @return float* to a float[3]
     */
    float* getAcceleration();

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
};

}
