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

#include <stdint.h>

#define BMI160_I2C_BUS 0
#define BMI160_DEFAULT_I2C_ADDR 0x69

namespace upm {

  /**
   * @brief BMI160 3-axis Accelerometer, Gyroscope and Magnetometer
   * @defgroup bmi160 libupm-bmi160
   * @ingroup i2c accelerometer compass
   */


  /**
   * @library bmi160
   * @sensor bmi160
   * @comname UPM API for the BMI160 3-axis Accelerometer, Gyroscope
   * and Magnetometer
   * @type accelerometer compass
   * @man mouser
   * @con i2c
   * @web http://www.mouser.com/ProductDetail/Bosch-Sensortec/0330SB2187/?qs=sGAEpiMZZMvi6wO7nhr1L9JELKA6cYRX60mAGNTn0fQ%3d
   *
   * @brief UPM API for the BMI160 3-axis Accelerometer, Gyroscope and
   * Magnetometer
   *
   * The Bosch BMI160 is a 3-axis Accelerometer and Gyroscope.
   * Additionally it supports an external Magnetometer, accessed
   * through the BMI160's register interface.  This driver was
   * developed with a BMI160 "Shuttle" board, which included a BMM150
   * Magnetometer.
   *
   * The device is driven by either 1.8v or 3.3vdc.  This driver
   * incorporates the Bosch BMI160 driver code at
   * https://github.com/BoschSensortec/BMI160_driver .
   *
   * While not all of the functionality of this device is supported
   * initially, the inclusion of the Bosch driver in the source code
   * makes it possible to support whatever features are required that
   * the driver can support.
   *
   * @snippet bmi160.cxx Interesting
   */
  class BMI160 {
  public:

    typedef enum {
      ACCEL_RANGE_2G                      = 0, // 2 Gravities
      ACCEL_RANGE_4G,
      ACCEL_RANGE_8G,
      ACCEL_RANGE_16G
    } ACCEL_RANGE_T;

    typedef enum {
      GYRO_RANGE_125                      = 0, // 125 degrees/sec
      GYRO_RANGE_250,
      GYRO_RANGE_500,
      GYRO_RANGE_1000,
      GYRO_RANGE_2000
    } GYRO_RANGE_T;

    /**
     * bmi160 constructor
     *
     * @param bus i2c bus to use
     * @param address the address for this device
     */
    BMI160(int bus=BMI160_I2C_BUS, uint8_t address=BMI160_DEFAULT_I2C_ADDR);

    /**
     * BMI160 Destructor
     */
    ~BMI160();

    /**
     * Take a measurement and store the current sensor values
     * internally.  This function must be called prior to retrieving
     * any sensor values, for example getAccelerometer().
     *
     */
    void update();

    /**
     * set the scaling mode of the accelerometer
     *
     * @param scale one of the ACCEL_RANGE_T values
     */
    void setAccelerometerScale(ACCEL_RANGE_T scale);

    /**
     * set the scaling mode of the gyroscope
     *
     * @param scale one of the GYRO_RANGE_T values
     */
    void setGyroscopeScale(GYRO_RANGE_T scale);

    /**
     * Get the Accelerometer values.  This function returns a pointer
     * to 3 floating point values: X, Y, and Z, in that order.  The
     * values returned are in gravities.  update() must have been
     * called prior to calling this method.
     *
     * The caller is reponsible for freeing the returned pointer.
     *
     * @return Pointer to 3 floating point values: X, Y, and Z in
     * gravities.
     */
    float *getAccelerometer();

    /**
     * Get the Accelerometer values.  The values returned are in
     * gravities.  update() must have been called prior to calling
     * this method.
     *
     * @param x A pointer into which the X value will be returned
     * @param y A pointer into which the Y value will be returned
     * @param z A pointer into which the Z value will be returned
     */
    void getAccelerometer(float *x, float *y, float *z);

    /**
     * Get the Gyroscope values.  This function returns a pointer to 3
     * floating point values: X, Y, and Z, in that order.  The values
     * values returned are in degrees per second.  update() must have
     * been called prior to calling this method.
     *
     * The caller is reponsible for freeing the returned pointer.
     *
     * @return Pointer to 3 floating point values: X, Y, and Z in
     * degrees per second.
     */
    float *getGyroscope();

    /**
     * Get the Gyroscope values.  The values returned are in degrees
     * per second.  update() must have been called prior to calling
     * this method.
     *
     * @param x A pointer into which the X value will be returned
     * @param y A pointer into which the Y value will be returned
     * @param z A pointer into which the Z value will be returned
     */
    void getGyroscope(float *x, float *y, float *z);

    /**
     * Get the Magnetometer values.  This function returns a pointer
     * to 3 floating point values: X, Y, and Z, in that order.  The
     * values values returned are in micro Teslas.  update() must have
     * been called prior to calling this method.  If the Magnetometer
     * has been disabled, the return values will always be 0, 0, and
     * 0.
     *
     * The caller is reponsible for freeing the returned pointer.
     *
     * @return Pointer to 3 floating point values: X, Y, and Z in
     * micro Teslas.
     */
    float *getMagnetometer();

    /**
     * Get the Magnetometer values.  The values returned are in micro
     * Teslas.  update() must have been called prior to calling this
     * method.
     *
     * @param x A pointer into which the X value will be returned
     * @param y A pointer into which the Y value will be returned
     * @param z A pointer into which the Z value will be returned
     */
    void getMagnetometer(float *x, float *y, float *z);

    /**
     * Enable or disable the Magnetometer.  By default, the
     * magnetometer is enabled.
     *
     * @param enable true to enable the magnetometer, false to disable.
     */
    void enableMagnetometer(bool enable);

    /**
     * Return the sensor time.  This is a 24bit value that increments
     * every 39us.  It will wrap around once the 24b resolution is
     * exceeded.
     *
     * @return The current sensor time.
     */
    unsigned int getSensorTime();

  protected:
    // uncompensated accelerometer and gyroscope values
    float m_accelX;
    float m_accelY;
    float m_accelZ;

    float m_gyroX;
    float m_gyroY;
    float m_gyroZ;

    float m_magX;
    float m_magY;
    float m_magZ;

    unsigned int m_sensorTime;

    // accelerometer and gyro scaling factors, depending on their Full
    // Scale (Range) settings.
    float m_accelScale;
    float m_gyroScale;

    // is the magnetometer enabled?
    bool m_magEnabled;

    /**
     * set up initial values and start operation
     *
     * @return true if successful
     */
    virtual bool init();

  private:
    // due to the way we need to 'hook' into the bmi driver, the i2c
    // context is a static variable defined in the .cxx implmentation.

    uint8_t m_addr;
  };
}
