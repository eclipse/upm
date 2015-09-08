/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include "mpu60x0.h"
#include "ak8975.h"

#define MPU9150_I2C_BUS 0
#define MPU9150_DEFAULT_I2C_ADDR  MPU60X0_DEFAULT_I2C_ADDR


namespace upm {

  /**
   * @brief MPU9150 accelerometer library
   * @defgroup mpu9150 libupm-mpu9150
   * @ingroup seeed i2c gpio accelerometer compass
   */

  /**
   * @library mpu9150
   * @sensor mpu9150
   * @comname MPU9150 Inertial Measurement Unit
   * @altname Grove IMU 9DOF
   * @type accelerometer compass
   * @man seeed
   * @web http://www.seeedstudio.com/wiki/Grove_-_IMU_9DOF_v1.0
   * @con i2c gpio
   *
   * @brief API for MPU9150 chip (Accelerometer, Gyro and Magnometer Sensor)
   *
   * This module defines the MPU9150 interface for libmpu9150
   *
   * @image html mpu9150.jpg
   * @snippet mpu9150.cxx Interesting
   */

  class MPU9150: public MPU60X0
  {
  public:
    /**
     * MPU9150 constructor
     *
     * @param bus I2C bus to use
     * @param address The address for this device
     * @param magAddress The address of the connected magnetometer
     */
    MPU9150 (int bus=MPU9150_I2C_BUS, int address=MPU9150_DEFAULT_I2C_ADDR,
             int magAddress=AK8975_DEFAULT_I2C_ADDR);

    /**
     * MPU9150 destructor
     */
    ~MPU9150 ();

    /**
     * Set up initial values and start operation
     *
     * @return true if successful
     */
    bool init();

    /**
     * Take a measurement and store the current sensor values
     * internally.  Note, these user facing registers are only updated
     * from the internal device sensor values when the i2c serial
     * traffic is 'idle'.  So, if you are reading the values too fast,
     * the bus may never be idle, and you will just end up reading
     * the same values over and over.
     *
     * Unfortunately, it is is not clear how long 'idle' actually
     * means, so if you see this behavior, reduce the rate at which
     * you are calling update().
     */
    void update();

    /**
     * Return the compensated values for the x, y, and z axes.  The
     * unit of measurement is in micro-teslas (uT).
     *
     * @param x Pointer to returned X axis value
     * @param y Pointer to returned Y axis value
     * @param z Pointer to returned Z axis value
     */
    void getMagnetometer(float *x, float *y, float *z);

#ifdef SWIGJAVA
    /**
     * Return the compensated values for the x, y, and z axes.  The
     * unit of measurement is in micro-teslas (uT).
     *
     * @return Array containing X, Y, Z magnetometer values
     */
    float *getMagnetometer();
#endif



  protected:
    // magnetometer instance
    AK8975* m_mag;


  private:
    int m_i2cBus;
    uint8_t m_magAddress;
  };

}
