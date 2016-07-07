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
#include "bmm150.hpp"

#define BMC150_DEFAULT_BUS 0
#define BMC150_DEFAULT_ACC_ADDR 0x10
#define BMC150_DEFAULT_MAG_ADDR 0x12

namespace upm {

  /**
   * @library bmx055
   * @sensor bmc150
   * @comname BMC150 6-axis eCompass
   * @type accelerometer compass
   * @man mouser
   * @con i2c gpio spi
   * @web http://www.mouser.com/ProductDetail/Bosch-Sensortec/0330SB0156/?qs=sGAEpiMZZMsrChSOYEGTCd8nwjostN1SWavW0HYOOXw%3d
   *
   * @brief API for the BMC150 6-axis eCompass
   *
   *
   * The BMC150 is an integrated electronic compass solution for
   * consumer market applications. It comprises a 12bit leading edge
   * triaxial, low-g acceleration sensor and an ultra-low power, high
   * precision triaxial magnetic field sensor. It allows measurements
   * of acceleration and magnetic field in three perpendicular
   * axes. Performance and features of both sensing technologies are
   * carefully tuned and perfectly match the demanding requirements of
   * all 6-axis mobile applications such as electronic compass,
   * navigation or augmented reality.

   * The BMC150 is essentially 2 separate devices in one: the BMA250E
   * Accelerometer and the BMM150 Magnetometer.  They are completely
   * independant of each other.
   *
   * This driver provides a very simple interface to these 2 devices.
   * If finer control is desired, you should just use the separate
   * BMA25E and BMM150 device classes directly.  This driver simply
   * initializes both devices, and provides a mechanism to read
   * accelerometer and magnetometer data from them.
   *
   * @snippet bmc150.cxx Interesting
   */

  class BMC150 {
  public:
    /**
     * BMC150 constructor.
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
     * @param magBus I2C or SPI bus to use. -1 to skip initializing
     * this device.
     * @param magAddr The address for this device.  -1 for SPI.
     * @param magCS The gpio pin to use for the SPI Chip Select.  -1 for
     * I2C or for SPI with a hardware controlled pin.
     */
    BMC150(int accelBus=BMC150_DEFAULT_BUS,
           int accelAddr=BMC150_DEFAULT_ACC_ADDR,
           int accelCS=-1,
           int magBus=BMC150_DEFAULT_BUS,
           int magAddr=BMC150_DEFAULT_MAG_ADDR,
           int magCS=-1);

    /**
     * BMC150 Destructor.
     */
    ~BMC150();

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
    BMM150 *m_mag;

  private:
  };
}
