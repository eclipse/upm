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
#include "bmi160.h"

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
     * @comname BMI160 3-axis Accelerometer, Gyroscope
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
     * https://github.com/BoschSensortec/BMI160_driver
     *
     * The Bosch driver code does not provide a mechanism for passing
     * user data around (like the device context).  For this reason,
     * only one instance of this driver can be used in a given process,
     * due to the use of static data in the driver.
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

        /**
         * BMI160 constructor.  The default arguments inititialize I2C
         * operation and the default I2C address.
         *
         * @param bus i2c bus to use
         * @param address The address for this device if using I2C.  If
         * using SPI, supply -1 for this parameter.
         * @param cs_pin The GPIO to use for Chip Select (CS).  This is
         * only needed for SPI, and only if your SPI implementation
         * requires it.  Otherwise, just pass -1 if not using SPI, or your
         * CS is handled automatically by your SPI implementation.
         * @param enableMag true to enable the magnetometer, false otherwise
         */
        BMI160(int bus=BMI160_I2C_BUS, int address=BMI160_DEFAULT_I2C_ADDR,
               int csPin=-1, bool enableMag=true);

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
        void setAccelerometerScale(BMI160_ACC_RANGE_T scale);

        /**
         * set the scaling mode of the gyroscope
         *
         * @param scale one of the GYRO_RANGE_T values
         */
        void setGyroscopeScale(BMI160_GYRO_RANGE_T scale);

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
        bmi160_context m_bmi160;

        /**
         * Perform a bus read.  This function is bus agnostic, and is used
         * by the bosch code to perform bus reads.  It is exposed here for
         * those users wishing to perform their own low level accesses.
         * This is a low level function, and should not be used unless you
         * know what you are doing.
         *
         * @param addr For I2C operation, this is the I2C address.
         * For SPI, this argument is ignored.
         * @param reg The register address to access.
         * @param cnt The number of bytes to read.
         * @return The data read
         */
        std::string busRead(int addr, int reg, int len);

        /**
         * Perform a bus write.  This function is bus agnostic, and is used
         * by the bosch code to perform bus writes.  It is exposed here for
         * those users wishing to perform their own low level accesses.
         * This is a low level function, and should not be used unless you
         * know what you are doing.
         *
         * @param addr For I2C operation, this is the I2C address.
         * For SPI, this argument is ignored.
         * @param addr The register address to access.
         * @param data The string containing the data to write
         */
        void busWrite(int addr, int reg, std::string data);

    private:
    };
}
