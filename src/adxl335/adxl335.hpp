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
   * @brief ADXL335 Accelerometer library
   * @defgroup adxl335 libupm-adxl335
   * @ingroup seeed analog accelerometer
   */

  /**
   * @library adxl335
   * @sensor adxl335
   * @comname ADXL335 3-Axis Analog Accelerometer
   * @altname Grove 3-Axis Analog Accelerometer
   * @type accelerometer
   * @man seeed
   * @con analog
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
  class ADXL335 {
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

#ifdef SWIGJAVA
    /**
     * Gets the analog values for the 3 axes
     *
     * @return Array containing value of X, Y, Z axes
     */
    int *values();
#endif

    /**
     * Gets the acceleration along all 3 axes
     *
     * @param xAccel Pointer to returned X-axis value
     * @param yAccel Pointer to returned Y-axis value
     * @param zAccel Pointer to returned Z-axis value
     */
    void acceleration(float *xAccel, float *yAccel, float *zAccel);

#ifdef SWIGJAVA
    /**
     * Gets the acceleration along all 3 axes
     *
     * @return Array containing acceleration on X, Y, Z axes
     */
    float *acceleration();
#endif

    /**
     * While the sensor is still, measures the X-axis, Y-axis, and Z-axis
     * values and uses those values as the zero values.
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


