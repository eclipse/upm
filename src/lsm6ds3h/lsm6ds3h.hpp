/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
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
#include <vector>

#include <mraa/gpio.hpp>
#include "lsm6ds3h.h"

#include <interfaces/iAcceleration.hpp>

namespace upm {

    /**
     * @brief LSM6DS6H 3-axis Accelerometer
     * @defgroup lsm6ds3h libupm-lsm6ds3h
     * @ingroup i2c spi gpio stmicro accelerometer
     */

    /**
     * @library lsm6ds3h
     * @sensor lsm6ds3h
     * @comname Digital 3-axis Accelerometer and Gyroscope
     * @type accelerometer gyro
     * @man stmicro
     * @con i2c spi gpio
     * @web http://www.st.com/en/mems-and-sensors/lsm6ds3h.html
     *
     * @brief API for the LSM6DS3H 3-axis Accelerometer and Gyroscope
     *
     * The LSM6DS3H is a system-in-package featuring a 3D digital
     * accelerometer and a 3D digital gyroscope performing at 1.1 mA
     * (up to 1.6 kHz ODR) in high performance mode and enabling
     * always-on low-power features for an optimal motion experience
     * for the consumer.

     * Not all functionality of this chip has been implemented in this
     * driver, however all the pieces are present to add any desired
     * functionality.  This driver supports both I2C (default) and SPI
     * operation.
     *
     * @snippet lsm6ds3h.cxx Interesting
     */

    class LSM6DS3H: virtual public iAcceleration {
    public:

        /**
         * LSM6DS3H constructor.
         *
         * This device can support both I2C and SPI. For SPI, set the addr
         * to -1, and specify a positive integer representing the Chip
         * Select (CS) pin for the cs argument.  If you are using a
         * hardware CS pin (like edison with arduino breakout), then you
         * can connect the proper pin to the hardware CS pin on your MCU
         * and supply -1 for cs.  The default operating mode is I2C.
         *
         * @param bus I2C or SPI bus to use
         * @param addr The address for this device.  -1 for SPI
         * @param cs The gpio pin to use for the SPI Chip Select.  -1 for
         * I2C or for SPI with a hardware controlled pin.
         * @throws std::runtime_error on initialization failure
         */
        LSM6DS3H(int bus=LSM6DS3H_DEFAULT_I2C_BUS,
                 int addr=LSM6DS3H_DEFAULT_I2C_ADDR,
                 int cs=-1);

        /**
         * LSM6DS3H destructor
         */
        virtual ~LSM6DS3H();

        /**
         * Update the internal stored values from sensor data
         *
         * @throws std::runtime_error on failure
         */
        void update();

        /**
         * Return the chip ID
         *
         * @return The chip ID
         */
        uint8_t getChipID();

        /**
         * Return accelerometer data in gravities.  update() must have
         * been called prior to calling this method.
         *
         * @param x Pointer to a floating point value that will have the
         * current x component placed into it
         * @param y Pointer to a floating point value that will have the
         * current y component placed into it
         * @param z Pointer to a floating point value that will have the
         * current z component placed into it
         */
        void getAccelerometer(float *x, float *y, float *z);

        /**
         * Return accelerometer data in gravities in the form of a
         * floating point vector.  update() must have been called
         * prior to calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order
         */
        std::vector<float> getAccelerometer();

        /**
         * get acceleration values
         * 
         * @return stl vector of size 3 representing the 3 axis
         */
        virtual std::vector<float> getAcceleration();

        /**
         * Return gyroscope data in degrees per second (DPS).
         * update() must have been called prior to calling this
         * method.
         *
         * @param x Pointer to a floating point value that will have the
         * current x component placed into it
         * @param y Pointer to a floating point value that will have the
         * current y component placed into it
         * @param z Pointer to a floating point value that will have the
         * current z component placed into it
         */
        void getGyroscope(float *x, float *y, float *z);

        /**
         * Return gyroscope data in degrees per second in the form of
         * a floating point vector.  update() must have been called
         * prior to calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order
         */
        std::vector<float> getGyroscope();

        /**
         * Return the current measured temperature.  Note, this is not
         * ambient temperature.  update() must have been called prior to
         * calling this method.
         *
         * @param fahrenheit true to return data in Fahrenheit, false for
         * Celicus.  Celsius is the default.
         * @return The temperature in degrees Celsius or Fahrenheit
         */
        float getTemperature(bool fahrenheit=false);

        /**
         * Initialize the device and start operation.  This function
         * is called from the constructor, so it will not need to be
         * called by a user unless the device is reset.  It sets the
         * accelerometer and gyroscope ODR and FS modes, and enables
         * BDU, register auto-increment, and high performance mode.
         *
         * @param acc_odr One of the LSM6DS3H_XL_ODR_T values
         * @param acc_fs One of the LSM6DS3H_XL_FS_T values
         * @param gyr_odr One of the LSM6DS3H_G_ODR_T values
         * @param gyr_fs One of the LSM6DS3H_G_FS_T values
         * @throws std::runtime_error on failure
         */
        void init(LSM6DS3H_XL_ODR_T acc_odr=LSM6DS3H_XL_ODR_104HZ,
                  LSM6DS3H_XL_FS_T acc_fs=LSM6DS3H_XL_FS_2G,
                  LSM6DS3H_G_ODR_T gyr_odr=LSM6DS3H_G_ODR_104HZ,
                  LSM6DS3H_G_FS_T gyr_fs=LSM6DS3H_G_FS_245DPS);

        /**
         * Reset the device as if during a power on reset.  All configured
         * values are lost when this happens.  You should call init()
         * afterwards, or at least perform the same initialization init()
         * does before continuing.
         *
         * @throws std::runtime_error on failure
         */
        void reset();

        /**
         * Set the output data rate (ODR) of the accelerometer
         *
         * @param odr One of the LSM6DS3H_XL_ODR_T values
         * @throws std::runtime_error on failure
         */
        void setAccelerometerODR(LSM6DS3H_XL_ODR_T odr);

        /**
         * Set the full scale (FS) of the accelerometer.  This device
         * supports a full scale of 2, 4, 8, and 16G.
         *
         * @param fs One of the LSM6DS3H_XL_FS_T values
         * @throws std::runtime_error on failure
         */
        void setAccelerometerFullScale(LSM6DS3H_XL_FS_T fs);

        /**
         * Set the output data rate (ODR) of the gyroscope
         *
         * @param odr One of the LSM6DS3H_G_ODR_T values
         * @throws std::runtime_error on failure
         */
        void setGyroscopeODR(LSM6DS3H_G_ODR_T odr);

        /**
         * Set the full scale (FS) of the gyroscope
         *
         * @param fs One of the LSM6DS3H_G_FS_T values
         * @throws std::runtime_error on failure
         */
        void setGyroscopeFullScale(LSM6DS3H_G_FS_T fs);

        /**
         * Enable accelerometer and gyroscope high performance modes.
         * These are further defined by the respective ODR settings to
         * allow low power, and normal/high-performance modes.  This
         * is enabled by default in init().
         *
         * @param enable true to enable high performance mode, false otherwise
         * @throws std::runtime_error on failure
         */
        void setHighPerformance(bool enable);

        /**
         * Indicate whether the interrupt should be active high (default)
         * or active low.  See the datasheet for details.
         *
         * @param high true for active high, false for active low
         * @throws std::runtime_error on failure
         */
        void setInterruptActiveHigh(bool high);

        /**
         * Indicate whether interrupts are push-pull (default) or open
         * drain.  See the datasheet for details.
         *
         * @param pp true for push-pull, false for open-drain
         * @throws std::runtime_error on failure
         */
        void setInterruptPushPull(bool pp);

        /**
         * Return the contents of the status register
         *
         * @return A bitmask of values from LSM6DS3H_STATUS_BITS_T
         */
        uint8_t getStatus();

        /**
         * install an interrupt handler
         *
         * @param intr One of the LSM6DS3H_INTERRUPT_PINS_T values
         * specifying which interrupt pin you are installing
         * @param gpio GPIO pin to use as interrupt pin
         * @param level The interrupt trigger level (one of mraa::Edge
         * values).  Make sure that you have configured the interrupt pin
         * properly for whatever level you choose.
         * @param isr The interrupt handler, accepting a void * argument
         * @param arg The argument to pass the the interrupt handler
         * @throws std::runtime_error on failure
         */
        void installISR(LSM6DS3H_INTERRUPT_PINS_T intr, int gpio,
                        mraa::Edge level,
                        void (*isr)(void *), void *arg);

        /**
         * uninstall a previously installed interrupt handler
         *
         * @param intr One of the LSM6DS3H_INTERRUPT_PINS_T values
         * specifying which interrupt pin you are removing
         */
        void uninstallISR(LSM6DS3H_INTERRUPT_PINS_T intr);

        /**
         * Read a register
         *
         * @param reg The register to read
         * @return The value of the register
         */
        uint8_t readReg(uint8_t reg);

        /**
         * Read contiguous registers into a buffer
         *
         * @param buffer The buffer to store the results
         * @param len The number of registers to read
         * @return The number of bytes read
         * @throws std::runtime_error on failure
         */
        int readRegs(uint8_t reg, uint8_t *buffer, int len);

        /**
         * Write to a register
         *
         * @param reg The register to write to
         * @param val The value to write
         * @throws std::runtime_error on failure
         */
        void writeReg(uint8_t reg, uint8_t val);

    protected:
        lsm6ds3h_context m_lsm6ds3h;

    private:
        /* Disable implicit copy and assignment operators */
        LSM6DS3H(const LSM6DS3H&) = delete;
        LSM6DS3H &operator=(const LSM6DS3H&) = delete;

    };
}
