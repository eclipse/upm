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

#include "bma250e.hpp"
#include "bmg160.hpp"
#include "bmm150.hpp"

namespace upm {

  /**
   * @brief BMX055 9-axis Sensor Module
   * @defgroup bmx055 libupm-bmx055
   * @ingroup i2c spi accelerometer compass
   */

  /**
   * @library bmx055
   * @sensor bmx055
   * @comname BMX055 9-axis Sensor Module
   * @type accelerometer compass
   * @man mouser
   * @con i2c gpio spi
   * @web http://www.mouser.com/ProductDetail/Bosch-Sensortec/0330SB0179/?qs=sGAEpiMZZMsrChSOYEGTCZo8d3KRE6KPUk8gVuWS2Ho%3d
   *
   * @brief API for the BMX055 9-axis Sensor Module
   *
   * The BMX055 is an integrated 9-axis sensor for the detection of
   * movements and rotations and magnetic heading. It comprises the
   * full functionality of a triaxial, low-g acceleration sensor, a
   * triaxial angular rate sensor and a triaxial geomagnetic sensor.
   *
   * The BMX055 senses orientation, tilt, motion, acceleration,
   * rotation, shock, vibration and heading in cell phones, handhelds,
   * computer peripherals, man-machine interfaces, virtual reality
   * features and game controllers.
   *
   * The BMX055 is essentially 3 separate devices in one: the BMA250E
   * Accelerometer, the BMG160 Gyroscope, and the BMM150 Magnetometer.
   * They are completely independant of each other.
   *
   * This driver provides a very simple interface to these 3 devices.
   * If finer control is desired, you should just use the separate
   * BMA25E, BMG160, and BMM150 device classes directly.  This driver
   * simply initializes all three devices, and provides a mechanism to
   * read accelerometer, gyroscope and magnetometer data from them.
   *
   * @snippet bmx055.cxx Interesting
   */

  class BMX055 {
  public:
    /**
     * BMX055 constructor.
     *
     * This device can support both I2C and SPI. For SPI, set the addr
     * to -1, and specify a positive integer representing the Chip
     * Select (CS) pin for the cs argument.  If you are using a
     * hardware CS pin (like edison with arduino breakout), then you
     * can connect the proper pin to the hardware CS pin on your MCU
     * and supply -1 for cs.  The default operating mode is I2C.
     *
     * @param accelBus I2C or SPI bus to use. -1 to skip initializing
     * this device.
     * @param accelAddr The address for this device.  -1 for SPI.
     * @param accelCS The gpio pin to use for the SPI Chip Select.  -1 for
     * I2C or for SPI with a hardware controlled pin.
     * @param gyroBus I2C or SPI bus to use. -1 to skip initializing
     * this device.
     * @param gyroAddr The address for this device.  -1 for SPI.
     * @param gyroCS The gpio pin to use for the SPI Chip Select.  -1 for
     * I2C or for SPI with a hardware controlled pin.
     * @param magBus I2C or SPI bus to use. -1 to skip initializing
     * this device.
     * @param magAddr The address for this device.  -1 for SPI.
     * @param magCS The gpio pin to use for the SPI Chip Select.  -1 for
     * I2C or for SPI with a hardware controlled pin.
     */
    BMX055(int accelBus=BMA250E_I2C_BUS,
           int accelAddr=BMA250E_DEFAULT_ADDR,
           int accelCS=-1,
           int gyroBus=BMG160_I2C_BUS,
           int gyroAddr=BMG160_DEFAULT_ADDR,
           int gyroCS=-1,
           int magBus=BMM150_I2C_BUS,
           int magAddr=BMM150_DEFAULT_ADDR,
           int magCS=-1);

    /**
     * BMX055 Destructor.
     */
    ~BMX055();

    /**
     * Update the internal stored values from sensor data.
     */
    void update();

    /**
     * Initialize the accelerometer and start operation.  This
     * function is called from the constructor so will not typically
     * need to be called by a user unless the device is reset or you
     * want to change these values.
     *
     * @param pwr One of the BMA250E::POWER_MODE_T values.  The default is
     * BMA250E::POWER_MODE_NORMAL.
     * @param range One of the BMA250E::RANGE_T values.  The default is
     * BMA250E::RANGE_2G.
     * @param bw One of the filtering BMA250E::BW_T values.  The default is
     * BMA250E::BW_250.
     */
    void initAccelerometer(BMA250E::POWER_MODE_T pwr=BMA250E::POWER_MODE_NORMAL,
                           BMA250E::RANGE_T range=BMA250E::RANGE_2G,
                           BMA250E::BW_T bw=BMA250E::BW_250);

    /**
     * Initialize the gyroscope and start operation.  This function is
     * called from the constructor so will not typically need to be
     * called by a user unless the device is reset or you want to
     * change these values.
     *
     * @param pwr One of the BMG160::POWER_MODE_T values.  The default is
     * BMG160::POWER_MODE_NORMAL.
     * @param range One of the BMG160::RANGE_T values.  The default is
     * BMG160::RANGE_250.
     * @param bw One of the filtering BMG160::BW_T values.  The default is
     * BMG160::BW_400_47.
     */
    void initGyroscope(BMG160::POWER_MODE_T pwr=BMG160::POWER_MODE_NORMAL,
                       BMG160::RANGE_T range=BMG160::RANGE_250,
                       BMG160::BW_T bw=BMG160::BW_400_47);

    /**
     * Initialize the magnetometer and start operation.  This function
     * is called from the constructor so will not typically need to be
     * called by a user unless the device is reset or you want to
     * change these values.  This method will call
     * BMM150::setPresetMode() with the passed parameter.
     *
     * @param usage One of the BMM150::USAGE_PRESETS_T values.  The default is
     * BMM150::USAGE_HIGH_ACCURACY.
     */
    void initMagnetometer(BMM150::USAGE_PRESETS_T usage=BMM150::USAGE_HIGH_ACCURACY);

    /**
     * Return accelerometer data in gravities.  update() must have
     * been called prior to calling this method.
     *
     * @param x Pointer to a floating point value that will have the
     * current x component placed into it.
     * @param y Pointer to a floating point value that will have the
     * current y component placed into it.
     * @param z Pointer to a floating point value that will have the
     * current z component placed into it.
     */
    void getAccelerometer(float *x, float *y, float *z);

    /**
     * Return accelerometer data in gravities in the form of a
     * floating point array.  The pointer returned by this function is
     * statically allocated and will be rewritten on each call.
     * update() must have been called prior to calling this method.
     *
     * @return A floating point array containing x, y, and z in
     * that order.
     */
    float *getAccelerometer();

    /**
     * Return gyroscope data in degrees per second.  update() must
     * have been called prior to calling this method.
     *
     * @param x Pointer to a floating point value that will have the
     * current x component placed into it.
     * @param y Pointer to a floating point value that will have the
     * current y component placed into it.
     * @param z Pointer to a floating point value that will have the
     * current z component placed into it.
     */
    void getGyroscope(float *x, float *y, float *z);

    /**
     * Return gyroscope data in degrees per second in the form of a
     * floating point array.  The pointer returned by this function is
     * statically allocated and will be rewritten on each call.
     * update() must have been called prior to calling this method.
     *
     * @return A floating point array containing x, y, and z in
     * that order.
     */
    float *getGyroscope();

    /**
     * Return magnetometer data in micro-Teslas (uT).  update() must
     * have been called prior to calling this method.
     *
     * @param x Pointer to a floating point value that will have the
     * current x component placed into it.
     * @param y Pointer to a floating point value that will have the
     * current y component placed into it.
     * @param z Pointer to a floating point value that will have the
     * current z component placed into it.
     */
    void getMagnetometer(float *x, float *y, float *z);

    /**
     * Return magnetometer data in micro-Teslas (uT) in the form of a
     * floating point array.  The pointer returned by this function is
     * statically allocated and will be rewritten on each call.
     * update() must have been called prior to calling this method.
     *
     * @return A floating point array containing x, y, and z in
     * that order.
     */
    float *getMagnetometer();


  protected:
    BMA250E *m_accel;
    BMG160 *m_gyro;
    BMM150 *m_mag;

  private:
  };
}
