/*
 * Author: Alex Tereschenko <alext.mkrs@gmail.com>
 * Copyright (c) 2018 Alex Tereschenko.
 *
 * Based on LIS2DS12 module by
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

#include "lis3dh.h"
#include <mraa/gpio.hpp>

#include <interfaces/iAcceleration.hpp>

namespace upm
{
/**
 * @brief LIS3DH 3-axis Accelerometer
 * @defgroup lis3dh libupm-lis3dh
 * @ingroup i2c spi gpio stmicro accelerometer
 */

/**
 * @library lis3dh
 * @sensor lis3dh
 * @comname Digital 3-axis Accelerometer
 * @type accelerometer
 * @man stmicro
 * @con i2c spi gpio
 * @web http://www.st.com/en/mems-and-sensors/lis3dh.html
 *
 * @brief API for the LIS3DH 3-axis Accelerometer
 *
 * The LIS3DH is an ultra-low-power high performance three-axis
 * linear accelerometer belonging to the "nano" family which
 * leverages on the robust and mature manufacturing processes
 * already used for the production of micromachined
 * accelerometers.
 *
 * The LIS3DH has user-selectable full scales of
 * 2g/4g/8g/16g and is capable of measuring accelerations with
 * output data rates from 1 Hz to 5300 Hz.

 * Not all functionality of this chip has been implemented in this
 * driver, however all the pieces are present to add any desired
 * functionality. This driver supports both I2C (default) and SPI
 * operation.
 *
 * @snippet lis3dh.cxx Interesting
 */

class LIS3DH: virtual public iAcceleration
{
  public:
    /**
     * LIS3DH constructor.
     *
     * This device can support both I2C and SPI. For SPI, set the addr
     * to -1, and specify a positive integer representing the Chip
     * Select (CS) pin for the cs argument. If you are using a
     * hardware CS pin (like edison with arduino breakout), then you
     * can connect the proper pin to the hardware CS pin on your MCU
     * and supply -1 for cs. The default operating mode is I2C.
     *
     * @param bus I2C or SPI bus to use
     * @param addr The address for this device. -1 for SPI
     * @param cs The gpio pin to use for the SPI Chip Select. -1 for
     * I2C or for SPI with a hardware controlled pin.
     * @throws std::runtime_error on initialization failure
     */
    LIS3DH(int bus = LIS3DH_DEFAULT_I2C_BUS, int addr = LIS3DH_DEFAULT_I2C_ADDR, int cs = -1);

    /**
     * LIS3DH destructor
     */
    virtual ~LIS3DH();

    /**
     * Initialize the device and start operation. This function is
     * called from the constructor so will not typically need to be
     * called by a user unless the device is reset.
     *
     * @param odr One of the LIS3DH_ODR_T values. The default is
     * LIS3DH_ODR_100HZ
     * @param fs One of the LIS3DH_FS_T values. The default is
     * LIS3DH_FS_2G
     * @param high_res true to enable high resolution mode,
     * false to disable. The default is true.
     * @throws std::runtime_error on failure
     */
    void
    init(LIS3DH_ODR_T odr = LIS3DH_ODR_100HZ, LIS3DH_FS_T fs = LIS3DH_FS_2G, bool high_res = true);

    /**
     * Enable or disable specific axes.
     * init() enables all three by default.
     *
     * @param x_axis_enable true to enable X axis, false to disable
     * @param y_axis_enable true to enable Y axis, false to disable
     * @param z_axis_enable true to enable Z axis, false to disable
     * @throws std::runtime_error on failure
     */
    void enableAxes(bool x_axis_enable, bool y_axis_enable, bool z_axis_enable);

    /**
     * Enable or disable Block Data Update (BDU) mode.
     *
     * @param bdu_enable true to enable BDU mode, false to disable
     * @throws std::runtime_error on failure
     */
    void enableBDUMode(bool bdu_enable);

    /**
     * Enable or disable Low Power (LP) mode. Checks if mutually exclusive
     * High Resolution (HR) mode is enabled and bails out if yes.
     *
     * @param lp_enable true to enable LP mode, false to disable
     * @throws std::runtime_error on failure
     */
    void enableLPMode(bool lp_enable);

    /**
     * Enable or disable High Resolution (HR) mode. Checks if mutually exclusive
     * Low Power (LP) mode is enabled and bails out if yes.
     *
     * @param hr_enable true to enable HR mode, false to disable
     * @throws std::runtime_error on failure
     */
    void enableHRMode(bool hr_enable);

    /**
     * Enable Normal mode by explicitly disabling LP and HR ones.
     * Note that there's no "disable" part as it's generally unknown, which mode
     * we were in previously. To get out of Normal mode, just enable HR or LP one.
     *
     * @throws std::runtime_error on failure
     */
    void enableNormalMode();

    /**
     * Enable high pass filtering of the accelerometer axis data.
     * init() disables this by default. See the datasheet for
     * details.
     *
     * @param filter true to enable filtering, false to disable
     * @throws std::runtime_error on failure
     */
    void enableHPFiltering(bool filter);

    /**
     * Enable or disable interrupt latching for INT1 and INT2.
     * See the datasheet for details.
     *
     * @param int1_latch true to enable latching for INT1, false to disable
     * @param int2_latch true to enable latching for INT2, false to disable
     * @throws std::runtime_error on failure
     */
    void enableInterruptLatching(bool int1_latch, bool int2_latch);

    /**
     * Enable or disable built-in Analog-to-Digital Converter (ADC).
     *
     * @param adc_enable true to enable ADC, false to disable
     * @throws std::runtime_error on failure
     */
    void enableADC(bool adc_enable);

    /**
     * Enable or disable built-in temperature sensor. It depends on ADC being
     * enabled, so we enable it unconditionally. See datasheet for details.
     *
     * @param temperature_enable true to enable temp sensor, false to disable
     * @throws std::runtime_error on failure
     */
    void enableTemperature(bool temperature_enable);

    /**
     * Set the output data rate (ODR) of the device
     *
     * @param odr One of the LIS3DH_ODR_T values
     * @throws std::runtime_error on failure
     */
    void setODR(LIS3DH_ODR_T odr);

    /**
     * Set the full scale (FS) of the device. This device supports a
     * full scale of 2, 4, 8 and 16G.
     *
     * @param fs One of the LIS3DH_FS_T values
     * @throws std::runtime_error on failure
     */
    void setFullScale(LIS3DH_FS_T fs);

    /**
     * Indicate whether INT1 and INT2 interrupts should be active high (default)
     * or active low. See the datasheet for details.
     *
     * @param high true for active high, false for active low
     * @throws std::runtime_error on failure
     */
    void setInterruptActiveHigh(bool high);

    /**
     * Set interrupt 1 configuration. See the datasheet for details.
     *
     * @param cfg A bitmask of values from LIS3DH_CTRL_REG3_BITS_T
     * @throws std::runtime_error on failure
     */
    void setInt1Config(uint8_t cfg);

    /**
     * Set interrupt 2 configuration. See the datasheet for details.
     *
     * @param cfg A bitmask of values from LIS3DH_CTRL_REG6_BITS_T
     * @throws std::runtime_error on failure
     */
    void setInt2Config(uint8_t cfg);

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
    int readRegs(uint8_t reg, uint8_t* buffer, int len);

    /**
     * Write to a register
     *
     * @param reg The register to write to
     * @param val The value to write
     * @throws std::runtime_error on failure
     */
    void writeReg(uint8_t reg, uint8_t val);

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
     * Return accelerometer data in gravities. update() must have
     * been called prior to calling this method.
     *
     * @param x Pointer to a floating point value that will have the
     * current x component placed into it
     * @param y Pointer to a floating point value that will have the
     * current y component placed into it
     * @param z Pointer to a floating point value that will have the
     * current z component placed into it
     */
    void getAccelerometer(float* x, float* y, float* z);

    /**
     * Return accelerometer data in gravities in the form of a
     * floating point vector. update() must have been called
     * prior to calling this method.
     *
     * @return A floating point vector containing x, y and z in
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
     * Return the current measured temperature. Note, this is not
     * ambient temperature. update() must have been called prior to
     * calling this method.
     *
     * @param fahrenheit true to return data in Fahrenheit, false for
     * Celicus. Celsius is the default.
     * @return The temperature in degrees Celsius or Fahrenheit
     */
    float getTemperature(bool fahrenheit = false);

    /**
     * Return the contents of the status register
     *
     * @return A bitmask of values from LIS3DH_STATUS_REG_BITS_T
     */
    uint8_t getStatus();

    /**
     * Return the contents of the status aux register
     *
     * @return A bitmask of values from LIS3DH_STATUS_REG_AUX_BITS_T
     */
    uint8_t getStatusAux();

    /**
     * Install an interrupt handler
     *
     * @param intr One of the LIS3DH_INTERRUPT_PINS_T values
     * specifying which interrupt pin you are installing
     * @param gpio GPIO pin to use as interrupt pin
     * @param level The interrupt trigger level (one of mraa::Edge
     * values). Make sure that you have configured the interrupt pin
     * properly for whatever level you choose.
     * @param isr The interrupt handler, accepting a void * argument
     * @param arg The argument to pass the the interrupt handler
     * @throws std::runtime_error on failure
     */
    void installISR(LIS3DH_INTERRUPT_PINS_T intr,
                    int gpio,
                    mraa::Edge level,
                    void (*isr)(void*),
                    void* arg);

    /**
     * Uninstall a previously installed interrupt handler
     *
     * @param intr One of the LIS3DH_INTERRUPT_PINS_T values,
     * specifying which interrupt pin you are removing
     */
    void uninstallISR(LIS3DH_INTERRUPT_PINS_T intr);

  protected:
    lis3dh_context m_lis3dh;

  private:
    // Disable implicit copy and assignment operators
    LIS3DH(const LIS3DH&) = delete;
    LIS3DH& operator=(const LIS3DH&) = delete;

};
}
