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

#include <mraa/initio.hpp>
#include <mraa/gpio.hpp>
#include "bmm150.h"

#include <interfaces/iMagnetometer.hpp>

namespace upm {

    /**
     * @brief BMM150 3-axis Magnetometer
     * @defgroup bmm150 libupm-bmm150
     * @ingroup i2c spi gpio bosch compass
     */

    /**
     * @library bmm150
     * @sensor bmm150
     * @comname 3-axis Geomagnetic Sensor
     * @altname bmm050
     * @type compass
     * @man bosch
     * @con i2c spi gpio
     * @web https://www.bosch-sensortec.com/bst/products/all_products/bmm150
     *
     * @brief API for the BMM150 3-Axis Geomagnetic Sensor
     *
     * The BMM150 is a standalone geomagnetic sensor for consumer market
     * applications. It allows measurements of the magnetic field in
     * three perpendicular axes. Based on Bosch's proprietary FlipCore
     * technology, performance and features of BMM150 are carefully
     * tuned and perfectly match the demanding requirements of all
     * 3-axis mobile applications such as electronic compass, navigation
     * or augmented reality.
     *
     * An evaluation circuitry (ASIC) converts the output of the
     * geomagnetic sensor to digital results which can be read out over
     * the industry standard digital interfaces (SPI and I2C).
     *
     * Not all functionality of this chip has been implemented in this
     * driver, however all the pieces are present to add any desired
     * functionality.  This driver supports both I2C (default) and SPI
     * operation.
     *
     * This device requires 3.3v operation.
     *
     * @snippet bmm150.cxx Interesting
     */

    class BMM150: virtual public iMagnetometer {
    public:
        /**
         * BMM150 constructor.
         *
         * This device can support both I2C and SPI. For SPI, set the addr
         * to -1, and specify a positive integer representing the Chip
         * Select (CS) pin for the cs argument.  If you are using a
         * hardware CS pin (like edison with arduino breakout), then you
         * can connect the proper pin to the hardware CS pin on your MCU
         * and supply -1 for cs.  The default operating mode is I2C.
         *
         * @param bus I2C or SPI bus to use.
         * @param addr The address for this device.  -1 for SPI.
         * @param cs The gpio pin to use for the SPI Chip Select.  -1 for
         * I2C or for SPI with a hardware controlled pin.
         *
         * @throws std::runtime_error on failure.
         */
        BMM150(int bus=BMM150_DEFAULT_I2C_BUS,
               int addr=BMM150_DEFAULT_ADDR,
               int cs=-1);

        /**
         * Instantiates BMM150 3-axis Magnetometer based on a given string.
         *
         * @param initStr string containing specific information for BMM150 initialization.
         */
        BMM150(std::string initStr);

        /**
         * BMM150 Destructor.
         */
        ~BMM150();

        /**
         * Update the internal stored values from sensor data.
         *
         * @throws std::runtime_error on failure.
         */
        void update();

        /**
         * Return the chip ID.
         *
         * @return The chip ID.
         */
        uint8_t getChipID();

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
         * Return magnetometer data in micro-Teslas (uT) in the form
         * of a floating point vector.  update() must have been called
         * prior to calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order.
         */
        std::vector<float> getMagnetometer();

        /**
         * Initialize the device and start operation.  This function is
         * called from the constructor so will not typically need to be
         * called by a user unless the device is reset.  This method will
         * call setPresetMode() with the passed parameter.
         *
         * @param usage One of the BMM150_USAGE_PRESETS_T values.  The
         * default is BMM150_USAGE_HIGH_ACCURACY.
         * @throws std::runtime_error on failure.
         */
        void init(BMM150_USAGE_PRESETS_T usage=BMM150_USAGE_HIGH_ACCURACY);

        /**
         * Set one of the Bosch recommended preset modes.  These modes
         * configure the sensor for varying use cases.
         *
         * @param usage One of the BMM150_USAGE_PRESETS_T values.  The
         * default is BMM150_USAGE_HIGH_ACCURACY.
         *
         * @throws std::runtime_error on failure.
         */
        void setPresetMode(BMM150_USAGE_PRESETS_T usage);

        /**
         * Perform a device soft-reset.  The device will be placed in
         * SUSPEND mode afterward with all configured setting lost, so
         * some re-initialization will be required to get data from the
         * sensor.  Calling init() will get everything running again.
         *
         * @throws std::runtime_error on failure.
         */
        void reset();

        /**
         * Set the magnetometer Output Data Rate. See the datasheet for
         * details.
         *
         * @param odr One of the BMM150_DATA_RATE_T values.
         * @throws std::runtime_error on failure.
         */
        void setOutputDataRate(BMM150_DATA_RATE_T odr);

        /**
         * Set or clear the Power bit.  When the power bit is cleared, the
         * device enters a deep suspend mode where only the REG_POWER_CTRL
         * register can be accessed.  This bit needs to be enabled for the
         * device to operate.  See the datasheet for details.  The
         * constructor enables this by default.  After a deep suspend mode
         * has been entered, all configured data is lost and the device
         * must be reconfigured (as via init()).
         *
         * @param power true to enable the bit, false otherwise.
         * @throws std::runtime_error on failure.
         */
        void setPowerBit(bool power);

        /**
         * Set the operating mode of the device.  See the datasheet for
         * details.
         *
         * @param opmode One of the BMM150_OPERATION_MODE_T values.
         * @throws std::runtime_error on failure.
         */
        void setOpmode(BMM150_OPERATION_MODE_T opmode);

        /**
         * Get the current operating mode of the device.  See the datasheet for
         * details.  The power bit must be one for this method to succeed.
         *
         * @return One of the BMM150_OPERATION_MODE_T values.
         */
        BMM150_OPERATION_MODE_T getOpmode();

        /**
         * Return the Interrupt Enables register.  This register
         * allows you to enable various interrupt conditions.  See the
         * datasheet for details.
         *
         * @return A bitmask of BMM150_INT_EN_BITS_T bits.
         */
        uint8_t getInterruptEnable();

        /**
         * Set the Interrupt Enables register.  See the datasheet for
         * details.
         *
         * @param bits A bitmask of BMM150_INT_EN_BITS_T bits.
         * @throws std::runtime_error on failure.
         */
        void setInterruptEnable(uint8_t bits);

        /**
         * Return the Interrupt Config register.  This register allows
         * determining the electrical characteristics of the 2 interrupt
         * pins (open-drain/push-pull and level/edge triggering) as well
         * as other options.  See the datasheet for details.
         *
         * @return A bitmask of BMM150_INT_CONFIG_BITS_T bits.
         */
        uint8_t getInterruptConfig();

        /**
         * Set the Interrupt Config register.  This register
         * allows determining the electrical characteristics of the 2
         * interrupt pins (open-drain/push-pull and level/edge
         * triggering).  See the datasheet for details.
         *
         * @param bits A bitmask of BMM150_INT_CONFIG_BITS_T bits.
         * @throws std::runtime_error on failure.
         */
        void setInterruptConfig(uint8_t bits);

        /**
         * Return the interrupt status register.  This register
         * indicates which interrupts have been triggered.  See the
         * datasheet for details.
         *
         * @return a bitmask of BMM150_INT_STATUS_BITS_T bits.
         */
        uint8_t getInterruptStatus();

        /**
         * Set the repetition counter for the X and Y axes.  This allows the
         * device to average a number of measurements for a more stable
         * output.  See the datasheet for details.
         *
         * @param reps A coefficient for specifying the number of
         * repititions to perform. (1 + 2(reps))
         * @throws std::runtime_error on failure.
         */
        void setRepetitionsXY(uint8_t reps);

        /**
         * Set the repetition counter for the Z axis.  This allows the
         * device to average a number of measurements for a more stable
         * output.  See the datasheet for details.
         *
         * @param reps A coefficient for specifying the number of
         * repititions to perform. (1 + (reps))
         * @throws std::runtime_error on failure.
         */
        void setRepetitionsZ(uint8_t reps);

        /**
         * Install an interrupt handler.
         *
         * @param intr One of the BMM150_INTERRUPT_PINS_T values
         * specifying which interrupt pin you are installing.
         * @param gpio GPIO pin to use as interrupt pin.
         * @param level The interrupt trigger level (one of mraa::Edge
         * values).  Make sure that you have configured the interrupt pin
         * properly for whatever level you choose.
         * @param isr The interrupt handler, accepting a void * argument.
         * @param arg The argument to pass the the interrupt handler.
         * @throws std::runtime_error on failure.
         */
        void installISR(BMM150_INTERRUPT_PINS_T intr, int gpio,
                        mraa::Edge level,
                        void (*isr)(void *), void *arg);

        /**
         * Uninstall a previously installed interrupt handler.
         *
         * @param intr One of the BMM150_INTERRUPT_PINS_T values
         * specifying which interrupt pin you are removing.
         */
        void uninstallISR(BMM150_INTERRUPT_PINS_T intr);

        /**
         * Read a register.
         *
         * @param reg The register to read.
         * @return The value of the register.
         */
        uint8_t readReg(uint8_t reg);

        /**
         * Read contiguous registers into a buffer.
         *
         * @param buffer The buffer to store the results.
         * @param len The number of registers to read.
         * @return The number of bytes read.
         * @throws std::runtime_error on failure.
         */
        int readRegs(uint8_t reg, uint8_t *buffer, int len);

        /**
         * Write to a register.
         *
         * @param reg The register to write to.
         * @param val The value to write.
         * @throws std::runtime_error on failure.
         */
        void writeReg(uint8_t reg, uint8_t val);

    protected:
        bmm150_context m_bmm150;
        mraa::MraaIo mraaIo;

    private:
        /* Disable implicit copy and assignment operators */
        BMM150(const BMM150&) = delete;
        BMM150 &operator=(const BMM150&) = delete;

    };
}
