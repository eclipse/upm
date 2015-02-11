/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Adapted from the seeedstudio example
 * https://github.com/Seeed-Studio/Accelerometer_ADXL335
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
#include <mraa/aio.h>

#define ADXL335_DEFAULT_AREF 5.0
#define ADXL335_SENSITIVITY 0.25 // 0.25v/g

namespace upm {

  /**
   * @brief C++ API for the ADXL335 3-axis analog acclerometer
   *
   * UPM module for the ADXL335 3-axis analog accelerometer.  This
   * was tested on a Grove 3-axis Analog Accelerometer.  It uses 3
   * analog pins, one each for X, Y, and Z axis.
   *
   * @ingroup grove analog
   * @defgroup adxl335 libupm-adxl335
   * @snippet adxl335.cxx Interesting
   */
  class ADXL335 {
  public:
    /**
     * ADXL335 constructor
     *
     * @param pinX analog pin to use for axis X
     * @param pinY analog pin to use for axis Y
     * @param pinZ analog pin to use for axis Z
     * @param aref analog voltage reference, default 5.0
     */
    ADXL335(int pinX, int pinY, int pinZ, float aref=ADXL335_DEFAULT_AREF);

    /**
     * ADXL335 Destructor
     */
    ~ADXL335();

    /**
     * Set the "zero" value of the X axis, determined through calibration
     *
     * @param zeroX  The "zero" value of the X axis
     */
    void setZeroX(float zeroX) { m_zeroX = zeroX; };

    /**
     * Set the "zero" value of the Y axis, determined through calibration
     *
     * @param zeroX  The "zero" value of the Y axis
     */
    void setZeroY(float zeroY) { m_zeroY = zeroY; };

    /**
     * Set the "zero" value of the Z axis, determined through calibration
     *
     * @param zeroX  The "zero" value of the Z axis
     */
    void setZeroZ(float zeroZ) { m_zeroZ = zeroZ; };

    /**
     * Get the analog values for the 3 axes
     *
     * @param xVal pointer to returned X value
     * @param yVal pointer to returned Y value
     * @param zVal pointer to returned Z value
     */
    void values(int *xVal, int *yVal, int *zVal);

    /**
     * Get the acceleration along all 3 axes
     *
     * @param xAccel pointer to returned X value
     * @param yAccel pointer to returned Y value
     * @param zAccel pointer to returned Z value
     */
    void acceleration(float *xAccel, float *yAccel, float *zAccel);

    /**
     * While the sensor is still, measure the X, Y, and Z values and
     * use those values as our zero values.
     *
     */
    void calibrate();

  private:
    mraa_aio_context m_aioX;
    mraa_aio_context m_aioY;
    mraa_aio_context m_aioZ;
    float m_aref;
    float m_zeroX, m_zeroY, m_zeroZ;
  };
}


