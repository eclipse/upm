/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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
#include <iostream>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "mma7361.h"

namespace upm {
  /**
   * @brief UPM C++ API for the DFRobot MMA7361 Analog Accelerometer
   * @defgroup mma7361 libupm-mma7361
   * @ingroup dfrobot gpio ainput accelerometer
   */
  
  /**
   * @library mma7361
   * @sensor mma7361
   * @comname DFRobot MMA7361 Analog Accelerometer
   * @type accelerometer
   * @man dfrobot
   * @con ainput gpio
   * @web http://www.dfrobot.com/index.php?route=product/product&path=36&product_id=507#.V7YEj99ytNJ
   *
   * @brief API for the DFRobot MMA7361 Analog Accelerometer
   *
   * The driver was tested with the DFRobot MMA7361 Analog
   * Accelerometer.  It supports 3 Axes with a selectable 1.5G and 6G
   * sensitivity.
   *
   * @snippet mma7361.cxx Interesting
   */

  class MMA7361 {
  public:

    /**
     * MMA7361 object constructor
     *
     * @param x_pin Analog pin to use for X axis. -1 to disable.
     * @param y_pin Analog pin to use for Y axis. -1 to disable.
     * @param z_pin Analog pin to use for Z axis. -1 to disable.
     * @param selftest_pin GPIO pin to use for self test. -1 to disable.
     * @param sleep_pin GPIO pin to use for sleep function. -1 to disable.
     * @param freefall_pin GPIO pin to use for free fall (0g)
     * detection. -1 to disable.
     * @param range_pin GPIO pin to select range (1.5g or 6g).  -1 to
     * disable.
     * @param a_ref The analog reference voltage in use.  Default 5.0.
     */
    MMA7361(int x_pin, int y_pin, int z_pin,
            int selftest_pin, int sleep_pin,
            int freefall_pin, int range_pin,
            float a_ref=5.0);

    /**
     * MMA7361 object destructor
     */
    ~MMA7361();

    /**
     * Set the range of the device.  This device supports two G ranges:
     * 1.5 and 6.  The default is 1.5G.
     *
     * @param range true for 6G, false for 1.5G
     */
    void setRange(bool range);

    /**
     * Set sleep mode.  When in sleep mode the sensor uses minimal power.
     *
     * @param sleep true to go into sleep mode, false to wake up
     */
    void setSleep(bool sleep);

    /**
     * Get freefall detection status.
     *
     * @return true if a freefall condition is detected, false otherwise.
     */
    bool isInFreefall();

    /**
     * Enable self test mode.
     *
     * @param enable true to enable the self test mode, false otherwise.
     */
    void enableSelftest(bool enable);

    /**
     * Read the sensor status an update internal state.
     * update() must have been called before calling
     * getAcceleration(), getNormalized(), or getVolts().
     */
    void update();

    /**
     * Set sensor offset.  This offset is applied to the return values
     * before scaling.  Default is 0.0.
     *
     * @param x Offset to apply to X value
     * @param y Offset to apply to Y value
     * @param z Offset to apply to Z value
     */
    void setOffset(float x, float y, float z);

    /**
     * Set sensor scale.  The acceleration return values are scaled by
     * this value before the offset is applied.  Default is 1.0.
     *
     * @param x Scale to apply to X value
     * @param y Scale to apply to Y value
     * @param z Scale to apply to Z value
     */
    void setScale(float x, float y, float z);

    /**
     * Get computed acceleration from the sensor. update() must have
     * been called prior to calling this function.
     *
     * @param x a pointer in which X acceleration data will be returned
     * @param y a pointer in which Y acceleration data will be returned
     * @param z a pointer in which Z acceleration data will be returned
     */
    void getAcceleration(float *x, float *y, float *z);

    /**
     * Get computed acceleration from the sensor. update() must have
     * been called prior to calling this function.
     *
     * @return a pointer to a statically allocated array of 3 floats
     * containing the X, Y, and Z componenets.
     */
    float *getAcceleration();

    /**
     * Get the measured volts from the sensor. update() must have been
     * called prior to calling this function.
     *
     * @param x a pointer in which X volt data will be returned
     * @param y a pointer in which Y volt data will be returned
     * @param z a pointer in which Z volt data will be returned
     */
    void getVolts(float *x, float *y, float *z);

    /**
     * Get the measured volts from the sensor. update() must have been
     * called prior to calling this function.
     *
     * @return a pointer to a statically allocated array of 3 floats
     * containing the X, Y, and Z componenets.
     */
    float *getVolts();

    /**
     * Get the normalized ADC values from the sensor.  update() must have
     * been called prior to calling this function.
     *
     * @param x a pointer in which X normalized ADC data will be returned
     * @param y a pointer in which Y normalized ADC data will be returned
     * @param z a pointer in which Z normalized ADC data will be returned
     */
    void getNormalized(float *x, float *y, float *z);

    /**
     * Get the normalized ADC values from the sensor.  update() must have
     * been called prior to calling this function.
     *
     * @return a pointer to a statically allocated array of 3 ints
     * containing the X, Y, and Z componenets.
     */
    float *getNormalized();

  protected:
    // mma7361 device context
    mma7361_context m_mma7361;

  private:
  };
}


