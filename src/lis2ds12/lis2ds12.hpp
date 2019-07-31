/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
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
#include "lis2ds12.h"

#include <interfaces/iAcceleration.hpp>

namespace upm {

    /**
     * @brief LIS2DS12 3-axis Accelerometer
     * @defgroup lis2ds12 libupm-lis2ds12
     * @ingroup i2c spi gpio stmicro accelerometer
     */

    /**
     * @library lis2ds12
     * @sensor lis2ds12
     * @comname Digital 3-axis Accelerometer
     * @type accelerometer
     * @man stmicro
     * @con i2c spi gpio
     * @web http://www.st.com/en/mems-and-sensors/lis2ds12.html
     *
     * @brief API for the LIS2DS12 3-axis Accelerometer
     *
     * The LIS2DS12 is an ultra-low-power high performance three-axis
     * linear accelerometer belonging to the "pico" family which
     * leverages on the robust and mature manufacturing processes
     * already used for the production of micromachined
     * accelerometers.
     *
     * The LIS2DS12 has user-selectable full scales of
     * 2g/4g/8g/16g and is capable of measuring accelerations with
     * output data rates from 1 Hz to 6400 Hz.

     * Not all functionality of this chip has been implemented in this
     * driver, however all the pieces are present to add any desired
     * functionality.  This driver supports both I2C (default) and SPI
     * operation.
     *
     * This device requires 1.8v operation.
     *
     * @snippet lis2ds12.cxx Interesting
     */

    class LIS2DS12: virtual public iAcceleration {
    public:

        /**
         * LIS2DS12 constructor.
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
        LIS2DS12(int bus=LIS2DS12_DEFAULT_I2C_BUS,
                 int addr=LIS2DS12_DEFAULT_I2C_ADDR,
                 int cs=-1);

        /**
         * LIS2DS12 destructor
         */
        virtual ~LIS2DS12();

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
         * Initialize the device and start operation.  This function is
         * called from the constructor so will not typically need to be
         * called by a user unless the device is reset.
         *
         * @param odr One of the LIS2DS12_ODR_T values.  The default is
         * LIS2DS12_ODR_100HZ
         * @param fs One of the LIS2DS12_FS_T values.  The default is
         * LIS2DS12_FS_2G
         * @throws std::runtime_error on failure
         */
        void init(LIS2DS12_ODR_T odr=LIS2DS12_ODR_100HZ,
                  LIS2DS12_FS_T fs=LIS2DS12_FS_2G);

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
         * Set the output data rate (ODR) of the device
         *
         * @param odr One of the LIS2DS12_ODR_T values
         * @throws std::runtime_error on failure
         */
        void setODR(LIS2DS12_ODR_T odr);

        /**
         * Set the full scale (FS) of the device.  This device supports a
         * full scale of 2, 4, 8, and 16G.
         *
         * @param fs One of the LIS2DS12_FS_T values
         * @throws std::runtime_error on failure
         */
        void setFullScale(LIS2DS12_FS_T fs);

        /**
         * Enable high pass filtering of the accelerometer axis data.
         * init() disables this by default.  See the datasheet for
         * details.
         *
         * @param filter true to enable filtering, false to disable
         * @throws std::runtime_error on failure
         */
        void enableHPFiltering(bool filter);

        /**
         * Enable or disable interrupt latching.  If latching is disabled,
         * pulsed is enabled.  See the datasheet for details.
         *
         * @param latch true to enable latching, false to disable
         * @throws std::runtime_error on failure
         */
        void enableInterruptLatching(bool latch);

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
         * Set interrupt 1 configuration.  See the datasheet for details.
         *
         * @param cfg A bitmask of values from LIS2DS12_CTRL4_BITS_T
         * @throws std::runtime_error on failure
         */
        void setInt1Config(uint8_t cfg);

        /**
         * Set interrupt 2 configuration.  See the datasheet for details.
         *
         * @param cfg A bitmask of values from LIS2DS12_CTRL5_BITS_T
         * @throws std::runtime_error on failure
         */
        void setInt2Config(uint8_t cfg);

        /**
         * Return the contents of the status register
         *
         * @return A bitmask of values from LIS2DS12_STATUS_BITS_T
         */
        uint8_t getStatus();

        /**
         * install an interrupt handler
         *
         * @param intr One of the LIS2DS12_INTERRUPT_PINS_T values
         * specifying which interrupt pin you are installing
         * @param gpio GPIO pin to use as interrupt pin
         * @param level The interrupt trigger level (one of mraa::Edge
         * values).  Make sure that you have configured the interrupt pin
         * properly for whatever level you choose.
         * @param isr The interrupt handler, accepting a void * argument
         * @param arg The argument to pass the the interrupt handler
         * @throws std::runtime_error on failure
         */
        void installISR(LIS2DS12_INTERRUPT_PINS_T intr, int gpio,
                        mraa::Edge level,
                        void (*isr)(void *), void *arg);

        /**
         * uninstall a previously installed interrupt handler
         *
         * @param intr One of the LIS2DS12_INTERRUPT_PINS_T values
         * specifying which interrupt pin you are removing
         */
        void uninstallISR(LIS2DS12_INTERRUPT_PINS_T intr);

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
         * @param reg The register to start the read from
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
        lis2ds12_context m_lis2ds12;

    private:
        /* Disable implicit copy and assignment operators */
        LIS2DS12(const LIS2DS12&) = delete;
        LIS2DS12 &operator=(const LIS2DS12&) = delete;
        
    };
}
