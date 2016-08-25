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

#define READ_BUFFER_LENGTH 8

namespace upm {

/**
 * @brief ITG-3200 Gyroscope library
 * @defgroup itg3200 libupm-itg3200
 * @ingroup seeed i2c compass
 */

/**
 * @library itg3200
 * @sensor itg3200
 * @comname ITG-3200 3-Axis Digital Gyroscope
 * @altname Grove 3-Axis Digital Gyroscope
 * @type compass
 * @man seeed
 * @con i2c
 *
 * @brief API for the ITG-3200 3-Axis Digital Gyroscope
 *
 * InvenSense* ITG-3200 is a 3-axis digital gyroscope.
 * (https://www.sparkfun.com/datasheets/Sensors/Gyro/PS-ITG-3200-00-01.4.pdf)
 * This sensor has been tested and can run at either 3.3V or 5V on Intel(R) Galileo.<br>
 * <strong>However</strong>, it is incompatible with and not detected on the I2C bus
 * by Intel(R) Edison using the Arduino* breakout board.
 *
 * @image html itg3200.jpeg
 * @snippet itg3200.cxx Interesting
 */
class Itg3200 {
public:
    /**
     * Creates an Itg3200 object
     *
     * @param bus Number of the used I2C bus
     */
    Itg3200(int bus);

    /**
     * Calibrates the sensor to 0 on all axes. The sensor needs to be resting for accurate calibration.
     * It takes about 3 seconds and is also called by the constructor on object creation.
     *
     */
    void calibrate();

    /**
     * Returns the temperature reading, in Celsius, from the integrated temperature sensor
     *
     * @return float Temperature in Celsius
     */
    float getTemperature();

    /**
     * Returns a pointer to a float[3] that contains computed rotational speeds (angular velocities)
     *
     * @return float* to a float[3]
     */
    float* getRotation();

    /**
     * Returns a pointer to an int[3] that contains raw register values for X, Y, and Z
     *
     * @return int* to an int[3]
     */
    int16_t* getRawValues();

    /**
     * Returns an int that contains the raw register value for the temperature
     *
     * @return int Raw temperature
     */
    int16_t getRawTemp();

    /**
     * Updates the rotational values and temperature by reading from the I2C bus
     *
     * @return 0 if successful
     */
    mraa::Result update();
private:
    float m_angle[3];
    int16_t m_rotation[3];
    int16_t m_offsets[3];
    int16_t m_temperature;
    uint8_t m_buffer[READ_BUFFER_LENGTH];
    mraa::I2c m_i2c;
};

}
