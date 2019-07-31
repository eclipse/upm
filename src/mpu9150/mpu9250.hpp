/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
