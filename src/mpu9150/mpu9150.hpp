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
#include <vector>

#include "mpu60x0.hpp"
#include "ak8975.hpp"

#define MPU9150_I2C_BUS 0
#define MPU9150_DEFAULT_I2C_ADDR  MPU60X0_DEFAULT_I2C_ADDR


namespace upm {

  /**
   * @brief MPU and AK Accelerometer/Compass/Gyro Library
   * @defgroup mpu9150 libupm-mpu9150
   * @ingroup seeed i2c gpio accelerometer compass
   */

  /**
   * @library mpu9150
   * @sensor mpu9150
   * @comname Inertial Measurement Unit
   * @altname Grove IMU 9DOF
   * @type accelerometer compass
   * @man seeed
   * @web http://www.seeedstudio.com/wiki/Grove_-_IMU_9DOF_v1.0
   * @con i2c gpio
   *
   * @brief API for MPU9150 chip (Accelerometer, Gyro and Magnetometer Sensor)
   *
   * This module defines the MPU9150 interface for libmpu9150
   *
   * @image html mpu9150.jpg
   * @snippet mpu9150.cxx Interesting
   */

  class MPU9150: public MPU60X0, virtual public iMagnetometer
  {
  public:
    /**
     * MPU9150 constructor
     *
     * @param bus I2C bus to use
     * @param address The address for this device
     * @param magAddress The address of the connected magnetometer
     * @param enableAk8975 Enables i2c bypass mode for magnetometer, default
     * is true
     */
    MPU9150 (int bus=MPU9150_I2C_BUS, int address=MPU9150_DEFAULT_I2C_ADDR,
             int magAddress=AK8975_DEFAULT_I2C_ADDR, bool enableAk8975=true);

    /**
     * MPU9150 destructor
     */
    virtual ~MPU9150 ();

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

    /**
     * Return the compensated values for the x, y, and z axes.  The
     * unit of measurement is in micro-teslas (uT).
     *
     * @return std::vector containing X, Y, Z magnetometer values
     */
    std::vector<float> getMagnetometer();

  protected:
    // magnetometer instance
    AK8975* m_mag;


  private:
      /* Disable implicit copy and assignment operators */
      MPU9150(const MPU9150&) = delete;
      MPU9150 &operator=(const MPU9150&) = delete;

    int m_i2cBus;
    uint8_t m_magAddress;
    bool m_enableAk8975;
  };

}
