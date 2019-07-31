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

#include <interfaces/iGyroscope.hpp>

#define READ_BUFFER_LENGTH 8

namespace upm {

/**
 * @brief ITG3200 Gyroscope
 * @defgroup itg3200 libupm-itg3200
 * @ingroup seeed i2c compass
 */

/**
 * @library itg3200
 * @sensor itg3200
 * @comname 3-axis Digital Gyroscope
 * @altname Grove 3-Axis Digital Gyroscope
 * @type compass
 * @man seeed
 * @con i2c
 * @web http://wiki.seeed.cc/Grove-3-Axis_Digital_Gyro/
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
class Itg3200: virtual public iGyroscope {
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
     * Return gyroscope data in degrees per second in the form of
     * a floating point vector.  update() must have been called
     * prior to calling this method.
     *
     * @return A floating point vector containing x, y, and z in
     * that order.
     */
    std::vector<float> getGyroscope();

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
