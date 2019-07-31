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
#include <mraa/initio.hpp>
#include "bmg160.h"

#include <interfaces/iGyroscope.hpp>

namespace upm {

    /**
     * @brief BMG160 3-axis Gyroscope
     * @defgroup bmg160 libupm-bmg160
     * @ingroup i2c spi gpio bosch gyro
     */

    /**
     * @library bmg160
     * @sensor bmg160
     * @comname 3-axis Gyroscope Sensor
     * @type gyro
     * @man bosch
     * @con i2c spi gpio
     * @web https://www.bosch-sensortec.com/bst/products/all_products/bmg160
     *
     * @brief API for the BMG160 16 bit Triaxial Gyroscope
     *
     * The BMG160 is a 3-axis angular rate sensor that is made of a
     * surface micro machined sensing element and an evaluation ASIC.
     * Both parts are packed into one single LGA 3.0mm x 3.0mm x 0.95mm
     * housing.  The BMG160 is designed to meet requirements for
     * consumer applications such as image stabilization (DSC and
     * camera-phone), gaming and pointing devices. It is capable to
     * measure angular rates in three perpendicular room dimensions, the
     * x-, y- and z-axis, and to provide the corresponding output
     * signals. The BMG160 is fitted with digital bi-directional SPI and
     * I2C interfaces for optimum system integration.
     *
     * Not all functionality of this chip has been implemented in this
     * driver, however all the pieces are present to add any desired
     * functionality.  This driver supports both I2C (default) and SPI
     * operation.
     *
     * This device requires 3.3v operation.
     *
     * @snippet bmg160.cxx Interesting
     */

    class BMG160: virtual public iGyroscope {
    public:
        /**
         * BMG160 constructor.
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
         * @throws std::runtime_error on initialization failure.
         */
        BMG160(int bus=BMG160_DEFAULT_I2C_BUS, int addr=BMG160_DEFAULT_ADDR,
               int cs=-1);

        /**
         * Instantiates BMG160 3-axis Gyroscope based on a given string.
         *
         * @param initStr string containing specific information for BMG160 initialization.
         */
        BMG160(std::string initStr);

        /**
         * BMG160 Destructor.
         */
        ~BMG160();

        /**
         * Update the internal stored values from sensor data.
         *
         * @throws std::runtime_error on failure.
         */
        void update();

        /**
         * Return the chip ID.
         *
         * @return The chip ID (BMG160_CHIPID).
         */
        uint8_t getChipID();

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
         * Return gyroscope data in degrees per second in the form of
         * a floating point vector.  update() must have been called
         * prior to calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order.
         */
        std::vector<float> getGyroscope();

        /**
         * Return the current measured temperature.  Note, this is not
         * ambient temperature.  update() must have been called prior to
         * calling this method.
         *
         * @param fahrenheit true to return data in Fahrenheit, false for
         * Celicus.  Celsius is the default.
         * @return The temperature in degrees Celsius or Fahrenheit.
         */
        float getTemperature(bool fahrenheit=false);

        /**
         * Initialize the device and start operation.  This function is
         * called from the constructor so will not typically need to be
         * called by a user unless the device is reset.
         *
         * @param pwr One of the BMG160_POWER_MODE_T values.  The default is
         * BMG160_POWER_MODE_NORMAL.
         * @param range One of the BMG160_RANGE_T values.  The default is
         * BMG160_RANGE_250.
         * @param bw One of the filtering BMG160_BW_T values.  The default is
         * BMG160_BW_400_47.
         * @throws std::runtime_error on failure.
         */
        void init(BMG160_POWER_MODE_T pwr=BMG160_POWER_MODE_NORMAL,
                  BMG160_RANGE_T range=BMG160_RANGE_250,
                  BMG160_BW_T bw=BMG160_BW_400_47);

        /**
         * Reset the device as if during a power on reset.  All configured
         * values are lost when this happens.  You should call init()
         * afterwards, or at least perform the same initialization init()
         * does before continuing.
         *
         * @throws std::runtime_error on failure.
         */
        void reset();

        /**
         * Set the gyroscope detection scaling range.  This device
         * supports 125, 250, 500, 1000, and 2000 degree/s ranges.
         *
         * @param range One of the BMG160_RANGE_T values.
         * @throws std::runtime_error on failure.
         */
        void setRange(BMG160_RANGE_T range);

        /**
         * Set the output filtering bandwidth of the device.
         *
         * @param bw One of the BMG160_BW_T values.
         * @throws std::runtime_error on failure.
         */
        void setBandwidth(BMG160_BW_T bw);

        /**
         * Set the power mode of the device.  Care must be taken when
         * setting a low power or suspend mode.  See the datasheet for
         * details.  I ncertain power modes, register write must be
         * drastically slowed down. which we cannot support.
         *
         * @param power One of the POWER_MODE_T values.
         * @throws std::runtime_error on failure.
         */
        void setPowerMode(BMG160_POWER_MODE_T power);

        /**
         * Enable update() to read from the FIFO rather than the gyroscope
         * axis registers directly.  init() enables this mode by default.
         * An advantage to this mode that all axis data is sampled from
         * the same timeslice.  When reading directly from the gyroscope
         * output registers, it's possible for one axis to be updated
         * while another is being read, causing a temporal
         * inconsistancy..
         *
         * Using the FIFO removes this problem.
         *
         * @param useFIFO true to enable update() to read from the FIFO.
         * When false, update will read from the gyroscope output
         * registers directly.
         */
        void enableFIFO(bool useFIFO);

        /**
         * Set the FIFO watermark.  When the watermark is reached an
         * interrupt (if enabled) will be generated.
         *
         * @param wm The FIFO watermark to use.  The maximum value is 63.
         * @throws std::runtime_error on failure.
         */
        void fifoSetWatermark(int wm);

        /**
         * Set the FIFO configuration.  init() uses the
         * BMG160_FIFO_MODE_BYPASS mode with axes set to
         * BMG160_FIFO_DATA_SEL_XYZ by default.
         *
         * @param mode One of the BMG160_FIFO_MODE_T values.
         * @param axes One of the BMG160_FIFO_DATA_SEL_T values.
         * @throws std::runtime_error on failure.
         */
        void fifoConfig(BMG160_FIFO_MODE_T mode, BMG160_FIFO_DATA_SEL_T axes);

        /**
         * Return the Interrupt Enables 0 register.  These registers
         * allow you to enable various interrupt conditions.  See the
         * datasheet for details.
         *
         * @return A bitmask of INT_EN_0_BITS_T bits.
         */
        uint8_t getInterruptEnable0();

        /**
         * Set the Interrupt Enables 0 register.  See the datasheet for
         * details.
         *
         * @param bits A bitmask of BMG160_INT_EN_0_BITS_T bits.
         * @throws std::runtime_error on failure.
         */
        void setInterruptEnable0(uint8_t bits);

        /**
         * Return the Interrupt Map 0 register.  These registers allow you
         * to map specific interrupts to the interrupt 1 or interrupt 2
         * pin.  See the datasheet for details.
         *
         * @return A bitmask of INT_MAP_0_BITS_T bits.
         */
        uint8_t getInterruptMap0();

        /**
         * Set the Interrupt Map 0 register.  These registers allow you
         * to map specific interrupts to the interrupt 1 or interrupt 2
         * pin.  See the datasheet for details.
         *
         * @param bits A bitmask of BMG160_INT_MAP_0_BITS_T bits.
         * @throws std::runtime_error on failure.
         */
        void setInterruptMap0(uint8_t bits);

        /**
         * Return the Interrupt Map 1 register.  See the datasheet for
         * details.
         *
         * @return A bitmask of BMG160_INT_MAP_1_BITS_T bits.
         */
        uint8_t getInterruptMap1();

        /**
         * Set the Interrupt Map 1 register.  See the datasheet for
         * details.
         *
         * @param bits A bitmask of BMG160_INT_MAP_1_BITS_T bits.
         * @throws std::runtime_error on failure.
         */
        void setInterruptMap1(uint8_t bits);

        /**
         * Return the Interrupt source register.  This register allows
         * determining where data comes from (filtered/unfiltered) for
         * those interrupt sources where this is selectable.  See the
         * datasheet for details.
         *
         * @return A bitmask of INT_1A_BITS_T bits.
         */
        uint8_t getInterruptSrc();

        /**
         * Set the Interrupt source register.  This register allows
         * determining where data comes from (filtered/unfiltered) for
         * those interrupt sources where this is selectable.  See the
         * datasheet for details.
         *
         * @param bits A bitmask of INT_1A_BITS_T bits.
         * @throws std::runtime_error on failure.
         */
        void setInterruptSrc(uint8_t bits);

        /**
         * Return the Interrupt output control register.  This register
         * allows determining the electrical characteristics of the 2
         * interrupt pins (open-drain/push-pull and level/edge
         * triggering).  See the datasheet for details.
         *
         * @return A bitmask of INT_EN_1_BITS_T bits.
         */
        uint8_t getInterruptOutputControl();

        /**
         * Set the Interrupt output control register.  This register
         * allows determining the electrical characteristics of the 2
         * interrupt pins (open-drain/push-pull and level/edge
         * triggering).  See the datasheet for details.
         *
         * @param bits A bitmask of INT_EN_1_BITS_T bits.
         * @throws std::runtime_error on failure.
         */
        void setInterruptOutputControl(uint8_t bits);

        /**
         * Clear all latched interrupts.  See the datasheet for details.
         *
         * @throws std::runtime_error on failure.
         */
        void clearInterruptLatches();

        /**
         * Return the current interrupt latching behavior.  See the
         * datasheet for details.
         *
         * @return One of the RST_LATCH_T values.
         */
        BMG160_RST_LATCH_T getInterruptLatchBehavior();

        /**
         * Set the current interrupt latching behavior.  See the datasheet
         * for details.
         *
         * @param latch One of the RST_LATCH_T values.
         * @throws std::runtime_error on failure.
         */
        void setInterruptLatchBehavior(BMG160_RST_LATCH_T latch);

        /**
         * Return the interrupt status 0 register.  These registers
         * indicate which interrupts have been triggered.  See the
         * datasheet for details.
         *
         * @return a bitmask of INT_STATUS_0_BITS_T bits.
         */
        uint8_t getInterruptStatus0();

        /**
         * Return the interrupt status 1 register.  See the datasheet for
         * details.
         *
         * @return a bitmask of INT_STATUS_1_BITS_T bits.
         */
        uint8_t getInterruptStatus1();

        /**
         * Return the interrupt status 2 register.  See the datasheet for
         * details.
         *
         * @return a bitmask of INT_STATUS_2_BITS_T bits.
         */
        uint8_t getInterruptStatus2();

        /**
         * Return the interrupt status 3 register.  See the datasheet for
         * details.
         *
         * @return a bitmask of INT_STATUS_3_BITS_T bits.
         */
        uint8_t getInterruptStatus3();

        /**
         * Enable shadowing of the gyroscope output registers.  When
         * enabled, a read of an axis LSB register automatically locks the
         * MSB register of that axis until it has been read.  This is
         * usually a good thing to have enabled.  init() enables this by
         * default.  If disabled, then it becomes possible for part of an
         * axis value to change while another part is being read, causing
         * inconsistent data.
         *
         * @param shadow true to enable axis register shadowing, false
         * otherwise.
         * @throws std::runtime_error on failure.
         */
        void enableRegisterShadowing(bool shadow);

        /**
         * Enable filtering of the gyroscope axis data.  init()
         * enables this by default.  If disabled, then gyroscope data
         * that is read will be raw and unfiltered (rated R).  See the
         * datasheet for details.
         *
         * @param filter true to enable filtering, false to disable.
         */
        void enableOutputFiltering(bool filter);

        /**
         * install an interrupt handler.
         *
         * @param intr One of the BMG160_INTERRUPT_PINS_T values
         * specifying which interrupt pin you are installing.
         * @param gpio GPIO pin to use as interrupt pin.
         * @param level The interrupt trigger level (one of the
         * mraa::Edge values).  Make sure that you have
         * configured the interrupt pin properly for whatever level
         * you choose.
         * @param isr The interrupt handler, accepting a void * argument.
         * @param arg The argument to pass the the interrupt handler.
         * @throws std::runtime_error on failure.
         */
        void installISR(BMG160_INTERRUPT_PINS_T intr, int gpio,
                        mraa::Edge level,
                        void (*isr)(void *), void *arg);

        /**
         * uninstall a previously installed interrupt handler
         *
         * @param intr one of the INTERRUPT_PINS_T values specifying which
         * interrupt pin you are removing.
         */
        void uninstallISR(BMG160_INTERRUPT_PINS_T intr);

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
         * Write to a register
         *
         * @param reg The register to write to.
         * @param val The value to write.
         * @throws std::runtime_error on failure.
         */
        void writeReg(uint8_t reg, uint8_t val);

    protected:
        bmg160_context m_bmg160;
        mraa::MraaIo mraaIo;

    private:
        /* Disable implicit copy and assignment operators */
        BMG160(const BMG160&) = delete;
        BMG160 &operator=(const BMG160&) = delete;

    };
}
