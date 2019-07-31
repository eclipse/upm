/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <vector>

#include <mraa/gpio.hpp>
#include "lsm303agr.h"

#include <interfaces/iAcceleration.hpp>
#include <interfaces/iMagnetometer.hpp>

namespace upm {

    /**
     * @brief LSM303AGR Ultra-compact eCompass Module
     * @defgroup lsm303agr libupm-lsm303agr
     * @ingroup i2c gpio stmicro compass accelerometer
     */

    /**
     * @library lsm303agr
     * @sensor lsm303agr
     * @comname Ultra-compact high-performance eCompass module
     * @type compass
     * @man stmicro
     * @con i2c gpio
     * @web http://www.st.com/en/mems-and-sensors/lsm303agr.html
     *
     * @brief API for the LSM303AGR 3-Axis Geomagnetic Sensor
     *
     * The LSM303AGR is an ultra-low-power high-performance
     * system-in-package featuring a 3D digital linear acceleration
     * sensor and a 3D digital magnetic sensor.  The LSM303AGR has
     * linear acceleration full scales of 2g/4g/8g/16g and a
     * magnetic field dynamic range of 50 Gauss.
     *
     * Not all functionality of this chip has been implemented in this
     * driver, however all the pieces are present to add any desired
     * functionality.  This driver supports only I2C operation.
     *
     * This device requires 3.3v operation.
     *
     * @snippet lsm303agr.cxx Interesting
     */

    class LSM303AGR: virtual public iAcceleration, virtual public iMagnetometer {
    public:
        /**
         * LSM303AGR constructor
         *
         * This driver can only support I2C. SPI requires support for
         * 3-wire SPI which we cannot currently handle.  Only the
         * basic capabilities of the device are supported, however
         * there is a full register map defined (lsm303agr_defs.h),
         * and with access to the bus read/write functions, any
         * desired additional functionality can be implemented.
         *
         * @param bus I2C bus to use
         * @param acc_addr The I2C address of the accelerometer.  Use -1
         * if you do not wish to use the accelerometer.
         * @param mag_addr The I2C address of the magnetometer.  Use -1
         * if you do not wish to use the magnetometer.
         * @return The device context, or NULL if an error occurred
         * @throws std::runtime_error on failure.
         */
        LSM303AGR(int bus=LSM303AGR_DEFAULT_I2C_BUS,
                  int acc_addr=LSM303AGR_DEFAULT_ACC_ADDR,
                  int mag_addr=LSM303AGR_DEFAULT_MAG_ADDR);

        /**
         * LSM303AGR Destructor
         */
        ~LSM303AGR();

        /**
         * Update the internal stored values from sensor data.  This
         * method must be called before querying the acceleration,
         * magnetometer, or temperature.
         *
         * @throws std::runtime_error on failure
         */
        void update();

        /**
         * Return magnetometer data in micro-Teslas (uT).  update() must
         * have been called prior to calling this method.
         *
         * @param x Pointer to a floating point value that will have the
         * current x component placed into it
         * @param y Pointer to a floating point value that will have the
         * current y component placed into it
         * @param z Pointer to a floating point value that will have the
         * current z component placed into it
         */
        void getMagnetometer(float *x, float *y, float *z);

        /**
         * Return magnetometer data in micro-Teslas (uT) in the form
         * of a floating point vector.  update() must have been called
         * prior to calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order
         */
        std::vector<float> getMagnetometer();

        /**
         * Return acceleration data in gravities.  update() must have
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
         * Return acceleration data in gravities in the form of a
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
         * Return temperature data in degrees Celsius.  NOTE: This is
         * not the ambient room temperature.  update() must have been
         * called prior to calling this method.
         *
         * @return Temperature in degrees Celsius
         */
        float getTemperature();

        /**
         * Initialize the device and start operation.  This function
         * is called from the constructor so it will not typically
         * need to be called by a user unless the device is reset.  It
         * will initialize the accelerometer and magnetometer (if
         * enabled) to certain default running modes.
         *
         * For the accelerometer, the full scale will be set to 2G,
         * mode continuous, all axes enabled, BDU enabled, temperature
         * measurement enabled, and an output data rate (ODR) of 100Hz
         * with the power mode set at high resolution.
         *
         * For the magnetometer, temperature compensation will be
         * enabled, mode continuous, and an output data rate of 10Hz
         * with the power mode set at high resolution.
         *
         * @param usage One of the LSM303AGR_POWER_MODE_T values.  The
         * default is LSM303AGR_POWER_HIGH_RESOLUTION.
         * @throws std::runtime_error on failure
         */
        void init(LSM303AGR_POWER_MODE_T usage=LSM303AGR_POWER_HIGH_RESOLUTION);

        /**
         * Set the full scale (sensitivity) value for the accelerometer.
         * This device supports 2G, 4G, 8G, and 16G full scale modes.
         *
         * @param fs One of the LSM303AGR_A_FS_T values
         * @throws std::runtime_error on failure
         */
        void setFullScale(LSM303AGR_A_FS_T fs);

        /**
         * Set an operating power mode.  There are 3 modes available: low
         * power, normal, and high resolution.
         *
         * @param mode One of the LSM303AGR_POWER_MODE_T values.  The
         * default set at initialization time is
         * LSM303AGR_POWER_HIGH_RESOLUTION.
         * @throws std::runtime_error on failure
         */
        void setPowerMode(LSM303AGR_POWER_MODE_T mode);

        /**
         * Set the accelerometer output data rate (ODR)
         *
         * @param odr One of the LSM303AGR_A_ODR_T values.  The default
         * set at initialization time is LSM303AGR_A_ODR_100HZ.
         * @throws std::runtime_error on failure
         */
        void setAccelerometerODR(LSM303AGR_A_ODR_T odr);

        /**
         * Set the magnetometer output data rate (ODR)
         *
         * @param odr One of the LSM303AGR_CFG_A_M_ODR_T values.
         * The default set at initialization time is
         * LSM303AGR_CFG_A_M_ODR_10HZ.
         * @throws std::runtime_error on failure
         */
        void setMagnetometerODR(LSM303AGR_CFG_A_M_ODR_T odr);

        /**
         * Return the accelerometer interrupt 1 config register.
         * This register allows you to enable various interrupt
         * conditions.  See the datasheet for details.
         *
         * @return A bitmask of LSM303AGR_INT_CFG_A_BITS_T bits
         */
        uint8_t getAccelerometerInt1Config();

        /**
         * Return the accelerometer interrupt 2 config register.
         * This register allows you to enable various interrupt
         * conditions.  See the datasheet for details.
         *
         * @return A bitmask of LSM303AGR_INT_CFG_A_BITS_T bits
         */
        uint8_t getAccelerometerInt2Config();

        /**
         * Set the accelerometer interrupt 1 config register.  See the
         * datasheet for details.
         *
         * @param dev The device context
         * @param bits A bitmask of LSM303AGR_INT_CFG_A_BITS_T bits
         * @throws std::runtime_error on failure
         */
        void setAccelerometerInt1Config(uint8_t bits);

        /**
         * Set the accelerometer interrupt 2 config register.  See the
         * datasheet for details.
         *
         * @param dev The device context
         * @param bits A bitmask of LSM303AGR_INT_CFG_A_BITS_T bits
         * @throws std::runtime_error on failure
         */
        void setAccelerometerInt2Config(uint8_t bits);

        /**
         * Return the magnetometer interrupt config register.  See the
         * datasheet for details.
         *
         * @return A bitmask of LSM303AGR_INT_CTRL_REG_M_BITS_T bits
         */
        uint8_t getMagnetometerIntConfig();

        /**
         * Set the magnetometer (mag) interrupt config register.  See
         * the datasheet for details.
         *
         * @param bits A bitmask of LSM303AGR_INT_CTRL_REG_M_BITS_T bits
         * @throws std::runtime_error on failure
         */
        void setMagnetometerIntConfig(uint8_t bits);

        /**
         * Return the accelerometer interrupt 1 source register.  This
         * register indicates which interrupts have been triggered.
         * See the datasheet for details.
         *
         * @return a bitmask of LSM303AGR_INT_SRC_A_BITS_T bits
         */
        uint8_t getAccelerometerInt1Src();

        /**
         * Return the accelerometer interrupt 2 source register.  This
         * register indicates which interrupts have been triggered.
         * See the datasheet for details.
         *
         * @return a bitmask of LSM303AGR_INT_SRC_A_BITS_T bits
         */
        uint8_t getAccelerometerInt2Src();

        /**
         * Return the magnetometer (mag) interrupt source register.
         * This register indicates which interrupts have been triggered.
         * See the datasheet for details.
         *
         * @param dev The device context
         * @return a bitmask of LSM303AGR_INT_SRC_REG_M_BITS_T bits
         */
        uint8_t getMagnetometerIntSrc();

        /**
         * Install an interrupt handler
         *
         * @param intr One of the LSM303AGR_INTERRUPT_PINS_T values
         * specifying which interrupt pin you are installing
         * @param gpio GPIO pin to use as interrupt pin
         * @param level The interrupt trigger level (one of mraa::Edge
         * values).  Make sure that you have configured the interrupt pin
         * properly for whatever level you choose.
         * @param isr The interrupt handler, accepting a void * argument
         * @param arg The argument to pass the the interrupt handler
         * @throws std::runtime_error on failure
         */
        void installISR(LSM303AGR_INTERRUPT_PINS_T intr, int gpio,
                        mraa::Edge level,
                        void (*isr)(void *), void *arg);

        /**
         * Uninstall a previously installed interrupt handler
         *
         * @param intr One of the LSM303AGR_INTERRUPT_PINS_T values
         * specifying which interrupt pin you are removing
         */
        void uninstallISR(LSM303AGR_INTERRUPT_PINS_T intr);

    protected:
        // our underlying device context
        lsm303agr_context m_lsm303agr;

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

    private:
        /* Disable implicit copy and assignment operators */
        LSM303AGR(const LSM303AGR&) = delete;
        LSM303AGR &operator=(const LSM303AGR&) = delete;

    };
}
