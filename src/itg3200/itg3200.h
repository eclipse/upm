/*
 * Author: Mihai Tudor Panu <mihai.t.panu@intel.com>
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

#include <mraa/i2c.h>

#define READ_BUFFER_LENGTH 8

namespace upm {

/**
 * @brief Itg3200 gyroscope library
 * @defgroup itg3200 libupm-itg3200
 */

/**
 * @brief C++ API for Itg3200 (3-axis digital gyroscope)
 *
 * The InvenSense Itg3200 is a 3-axis digital gyroscope.
 * (https://www.sparkfun.com/datasheets/Sensors/Gyro/PS-ITG-3200-00-01.4.pdf)
 * This sensor has been tested and can run at either 3V3 or 5V on the Intel Galileo.<br>
 * <strong>However</strong>, it is incompatible and will not be detected on the I2C bus
 * by the Intel Edison using the Arduino breakout board.
 *
 * @ingroup itg3200 i2c
 * @snippet itg3200.cxx Interesting
 * @image html itg3200.jpeg
 */
class Itg3200 {
public:
    /**
     * Creates an Itg3200 object
     *
     * @param bus number of used i2c bus
     */
    Itg3200(int bus);

    /**
     * Itg3200 object destructor
     */
    ~Itg3200();

    /**
     * Calibrates the sensor to 0 on all axes. Sensor needs to be resting for accurate calibration.
     * Takes about 3 seconds and is also called by constructor on object creation.
     *
     * @return 0 for successful calibration
     */
    mraa_result_t calibrate();

    /**
     * Returns the temperature reading from the integrated temperature sensor in Celsius degrees
     *
     * @return float temperature in Celsius degrees
     */
    float getTemperature();

    /**
     * Returns a pointer to an float[3] that contains computed rotational speeds (angular velocities)
     *
     * @return float* to an float[3]
     */
    float* getRotation();

    /**
     * Returns a pointer to an int[3] that contains the raw register values for X, Y and Z
     *
     * @return int* to an int[3]
     */
    int16_t* getRawValues();

    /**
     * Returns an int that contains the raw register value for the temperature
     *
     * @return int raw temperature
     */
    int16_t getRawTemp();

    /**
     * Updates the rotational values and temperature by reading from i2c bus
     *
     * @return 0 for success
     */
    mraa_result_t update();
private:
    float m_angle[3];
    int16_t m_rotation[3];
    int16_t m_offsets[3];
    int16_t m_temperature;
    uint8_t m_buffer[READ_BUFFER_LENGTH];
    mraa_i2c_context m_i2c;
};

}
