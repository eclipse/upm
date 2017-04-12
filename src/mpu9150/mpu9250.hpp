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

#include "mpu9150.hpp"

#define MPU9250_I2C_BUS 0
#define MPU9250_DEFAULT_I2C_ADDR  MPU9150_DEFAULT_I2C_ADDR


namespace upm {

  /**
   * @library mpu9150
   * @sensor mpu9250
   * @comname Inertial Measurement Unit
   * @altname Grove IMU 9DOF V2
   * @type accelerometer compass
   * @man seeed
   * @web http://www.seeedstudio.com/wiki/Grove_-_IMU_9DOF_v2.0
   * @con i2c gpio
   *
   * @brief API for MPU9250 chip (Accelerometer, Gyro and Magnetometer Sensor)
   *
   * This module defines the MPU9250 interface for libmpu9150
   *
   * @image html mpu9250.jpg
   * @snippet mpu9150-mpu9250.cxx Interesting
   */

  class MPU9250: public MPU9150
  {
  public:
    /**
     * MPU9250 constructor
     *
     * @param bus I2C bus to use
     * @param address The address for this device
     * @param magAddress The address of the connected magnetometer
     * @param enableAk8975 Enables i2c bypass mode for magnetometer, default
     * is true
     */
    MPU9250 (int bus=MPU9250_I2C_BUS, int address=MPU9250_DEFAULT_I2C_ADDR,
             int magAddress=AK8975_DEFAULT_I2C_ADDR, bool enableAk8975=true);

    /**
     * MPU9250 destructor
     */
    ~MPU9250 ();

    /**
     * get the temperature value
     *
     * @return the temperature value in degrees Celsius
     */
    float getTemperature();

  protected:

  private:
  };

}
