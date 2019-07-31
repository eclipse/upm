/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Adapted from the seeedstudio example
 * https://github.com/Seeed-Studio/Accelerometer_ADXL335
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <vector>
#include <mraa/aio.h>
#include <mraa/initio.hpp>

#include <interfaces/iAcceleration.hpp>

#define ADXL335_DEFAULT_AREF 5.0
#define ADXL335_SENSITIVITY 0.25 // 0.25v/g

namespace upm {

  /**
   * @brief ADXL335 Accelerometer
   * @defgroup adxl335 libupm-adxl335
   * @ingroup seeed analog accelerometer
   */

  /**
   * @library adxl335
   * @sensor adxl335
   * @comname Low-power, 3-axis +/- 3 g Accelerometer
   * @altname Grove 3-Axis Analog Accelerometer
   * @type accelerometer
   * @man seeed
   * @con analog
   * @web http://www.analog.com/en/products/mems/accelerometers/adxl335.html
   *
   * @brief API for the ADXL335 3-Axis Analog Accelerometer
   *
   * UPM module for the ADXL335 3-axis analog accelerometer. This
   * was tested on a Grove 3-axis Analog Accelerometer. It uses 3
   * analog pins, one for each axis: X, Y, and Z.
   *
   * @image html adxl335.jpg
   * @snippet adxl335.cxx Interesting
   */
  class ADXL335: virtual public iAcceleration {
  public:
    /**
     * ADXL335 constructor
     *
     * @param pinX Analog pin to use for X-axis
     * @param pinY Analog pin to use for Y-axis
     * @param pinZ Analog pin to use for Z-axis
     * @param aref Analog reference voltage; default is 5.0v
     */
    ADXL335(int pinX, int pinY, int pinZ, float aref=ADXL335_DEFAULT_AREF);

    /**
     * Instantiates ADXL335 Accelerometer based on a given string.
     *
     * @param initStr string containing specific information for ADXL335 initialization.
     */
    ADXL335(std::string initStr);

    /**
     * ADXL335 destructor
     */
    ~ADXL335();

    /**
     * Sets the "zero" value of the X-axis, determined through calibration
     *
     * @param zeroX  "Zero" value of the X-axis
     */
    void setZeroX(float zeroX) { m_zeroX = zeroX; };

    /**
     * Sets the "zero" value of the Y-axis, determined through calibration
     *
     * @param zeroY  "Zero" value of the Y-axis
     */
    void setZeroY(float zeroY) { m_zeroY = zeroY; };

    /**
     * Sets the "zero" value of the Z-axis, determined through calibration
     *
     * @param zeroZ  "Zero" value of the Z-axis
     */
    void setZeroZ(float zeroZ) { m_zeroZ = zeroZ; };

    /**
     * Gets the analog values for the 3 axes
     *
     * @param xVal Pointer to the returned X-axis value
     * @param yVal Pointer to the returned Y-axis value
     * @param zVal Pointer to the returned Z-axis value
     */
    void values(int *xVal, int *yVal, int *zVal);

    /**
     * Gets the analog values for the 3 axes
     *
     * @return std::vector of x, y, z analog acceleration values
     */
    std::vector<int> values();

    /**
     * Gets the acceleration along all 3 axes
     *
     * @param xAccel Pointer to returned X-axis value
     * @param yAccel Pointer to returned Y-axis value
     * @param zAccel Pointer to returned Z-axis value
     */
    void acceleration(float *xAccel, float *yAccel, float *zAccel);

    /**
     * Gets the acceleration along all 3 axes
     *
     * @return std::vector of x, y, z acceleration values
     */
    std::vector<float> acceleration();

    /**
     * get acceleration values
     * 
     * @return stl vector of size 3 representing the 3 axis
     */
    virtual std::vector<float> getAcceleration();

    /**
     * While the sensor is still, measures the X-axis, Y-axis, and Z-axis
     * values and uses those values as the zero values.
     */
    void calibrate();

  private:
    mraa::MraaIo mraaIo;
    mraa_aio_context m_aioX;
    mraa_aio_context m_aioY;
    mraa_aio_context m_aioZ;
    float m_aref;
    float m_zeroX, m_zeroY, m_zeroZ;
  };
}


