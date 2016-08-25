/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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
#include <mraa/common.hpp>
#include <mraa/i2c.hpp>

#define HP20X_I2C_BUS 0
#define HP20X_DEFAULT_I2C_ADDR 0x76

namespace upm {
  
  /**
   * @brief HP20X I2C Barometer (High-Accuracy) library
   * @defgroup hp20x libupm-hp20x
   * @ingroup seeed i2c pressure
   */

  /**
   * @library hp20x
   * @sensor hp20x
   * @comname Grove Barometer (High-Accuracy)
   * @altname HP20X Barometer (High-Accuracy)
   * @type pressure
   * @man seeed
   * @web http://www.seeedstudio.com/depot/Grove-Barometer-HighAccuracy-p-1865.html
   * @con i2c
   *
   * @brief API for the HP20X-based Grove Barometer (High-Accuracy)
   *
   * This is a high-accuracy barometer providing pressure, altitude,
   * and temperature data. It can be calibrated for a given altitude
   * offset, and a wide range of interrupt generating capabilities are
   * supported. As usual, see the HP20X datasheet for more details.
   *
   * This module was developed using a Grove Barometer (High-Accuracy)
   * based on an HP206C chip.
   *
   * @image html hp20x.jpg
   * @snippet hp20x.cxx Interesting
   */
  class HP20X {
  public:

    /**
     * HP20X commands
     */
    typedef enum {
      CMD_SOFT_RST              = 0x06,

      CMD_ADC_CVT               = 0x40, // mask - ANDed with DSR and CHNL bits

      CMD_READ_PT               = 0x10, // read pressure/temp
      CMD_READ_AT               = 0x11, // read alt/temp

      CMD_READ_P                = 0x30, // read pressure only
      CMD_READ_A                = 0x31, // read alt only
      CMD_READ_T                = 0x32, // read temp only

      CMD_ANA_CAL               = 0x28, // recalibrate internal analog blocks

      CMD_READ_REG              = 0x80, // mask - ANDed with reg addr
      CMD_WRITE_REG             = 0xc0  // mask - ANDed with reg addr
    } HP20X_CMD_T;

    /**
     * CHNL bits
     */
    typedef enum {
      CHNL_PT                   = 0x00, // pressure and temperature
      CHNL_T                    = 0x02, // temperature

      CHNL_SHIFT                = 0 // don't use, indicates position in REG
    } CHNL_BITS_T;
    
    /**
     * DSR bits
     */
    typedef enum {
      DSR_4096                  = 0x00, // decimation rate of digital filter
      DSR_2048                  = 0x01,
      DSR_1024                  = 0x02,
      DSR_512                   = 0x03,
      DSR_256                   = 0x04,
      DSR_128                   = 0x05,

      DSR_SHIFT                 = 2 // don't use, indicates position in REG
    } DSR_BITS_T;
    

    /**
     * HP20X registers
     */
    typedef enum {
      REG_ALT_OFF_LSB           = 0x00,
      REG_ALT_OFF_MSB           = 0x01,

      REG_PA_H_TH_LSB           = 0x02, // Pres/Alt high threshold
      REG_PA_H_TH_MSB           = 0x03,

      REG_PA_M_TH_LSB           = 0x04, // Pres/Alt medium threshold
      REG_PA_M_TH_MSB           = 0x05,

      REG_PA_L_TH_LSB           = 0x06, // Pres/Alt low threshold
      REG_PA_L_TH_MSB           = 0x07,

      REG_T_H_TH                = 0x08, // temperature high threshold
      REG_T_M_TH                = 0x09,
      REG_T_L_TH                = 0x0a,

      REG_INT_EN                = 0x0b, // interrupt enables
      REG_INT_CFG               = 0x0c, // interrupt configuration
      REG_INT_SRC               = 0x0d, // interrupt sources

      REG_PARA                  = 0x0e  // parameters config
    } HP20X_REG_T;
    
    /**
     * INT_EN bits
     */
    typedef enum {
      INT_EN_T_WIN_EN           = 0x01,
      INT_EN_PA_WIN_EN          = 0x02,

      INT_EN_T_TRAV_EN          = 0x04,
      INT_EN_PA_TRAV_EN         = 0x08,

      INT_EN_T_RDY_EN           = 0x10,
      INT_EN_PA_RDY_EN          = 0x20
      // 0x40, 0x80 reserved
    } INT_EN_BITS_T;

    /**
     * INT_CFG bits
     */
    typedef enum {
      INT_CFG_T_WIN_CFG          = 0x01,
      INT_CFG_PA_WIN_CFG         = 0x02,

      INT_CFG_T_TRAV_CFG         = 0x04,
      INT_CFG_PA_TRAV_CFG        = 0x08,

      INT_CFG_T_RDY_CFG          = 0x10,
      INT_CFG_PA_RDY_CFG         = 0x20,

      INT_CFG_PA_MODE            = 0x40
      // 0x80 reserved
    } INT_CFG_BITS_T;

    /**
     * INT_SRC bits
     */
    typedef enum {
      INT_SRC_T_WIN              = 0x01,
      INT_SRC_PA_WIN             = 0x02,

      INT_SRC_T_TRAV             = 0x04,
      INT_SRC_PA_TRAV            = 0x08,

      INT_SRC_T_RDY              = 0x10,
      INT_SRC_PA_RDY             = 0x20,

      INT_SRC_DEV_RDY            = 0x40, // device is ready

      INT_SRC_TH_ERR             = 0x80 // threshold error
    } INT_SRC_BITS_T;

    /**
     * PARA bits
     */
    typedef enum {
      // 0x01-0x40 reserved
      PARA_CMPS_EN               = 0x80 // compensation enable
    } PARA_BITS_T;

    /**
     * HP20X constructor
     *
     * @param bus I2C bus to use
     * @param address Address for this device
     */
    HP20X(int bus=HP20X_I2C_BUS, uint8_t address=HP20X_DEFAULT_I2C_ADDR);

    /**
     * HP20X destructor
     */
    ~HP20X();
    
    /**
     * Sets up initial values and starts operation
     *
     * @param dsr Data sampling rate; one of the DSR_BITS_T values
     * @return True if successful
     */
    bool init(DSR_BITS_T dsr=DSR_4096);

    /**
     * Sends a command to the device
     *
     * @param cmd Command to send; usually, one of the HP20X_CMD_T values
     * @return True if successful
     */
    bool writeCmd(uint8_t cmd);

    /**
     * Writes a value to a register
     *
     * @param reg Register to write to; one of the HP20X_REG_T values
     * @param data Value to write
     * @return True if successful
     */
    bool writeReg(HP20X_REG_T reg, uint8_t data);

    /**
     * Reads a register and returns its value
     *
     * @param reg Register to read; one of the HP20X_REG_T values
     * @return Value of a specified register
     */
    uint8_t readReg(HP20X_REG_T reg);

    /**
     * Reads 3 bytes of data in response to a conversion request, and
     * converts it to an integer
     *
     * @return Value read back (temperature, pressure, etc.)
     */
    int readData();

    /**
     * Checks to see if the DR_RDY bit is set, indicating the device
     * can accept commands
     *
     * @return True if the device is ready, false otherwise
     */
    bool isReady();

    /**
     * Checks to see if the device is ready, and sleeps/retries if not.
     * Returns once the device indicates it's ready.
     *
     * @return True if the device is ready; false if retries are exhausted
     */
    bool waitforDeviceReady();

    /**
     * Returns the temperature in Celsius
     *
     * @return Temperature
     */
    float getTemperature();

    /**
     * Returns the pressure in millibars
     *
     * @return Pressure
     */
    float getPressure();

    /**
     * Returns the computed altitude in meters
     *
     * @return Altitude
     */
    float getAltitude();

    /**
     * Enables or disables the on-chip compensator. This allows the
     * chip to filter and clean up the output data.
     *
     * @param enable True to enable, false otherwise
     */
    void compensationEnable(bool enable);

    /**
     * Sets up the interrupt enable register. This register defines
     * which events can cause a hardware interrupt pin to be pulled high
     * (active).
     *
     * @param bits One or more of the INT_EN_BITS_T bits
     * @return True if successful, false otherwise
     */
    bool setInterruptEnable(uint8_t bits);

    /**
     * Sets up the interrupt configuration register. This register
     * defines which events can cause an interrupt to be indicated.
     *
     * @param bits One or more of the INT_EN_BITS_T bits
     * @return True if successful, false otherwise
     */
    bool setInterruptConfig(uint8_t bits);

    /**
     * Gets the interrupt source register. This register indicates
     * which interrupts have been triggered. In addition, it
     * indicates when certain operations have been completed.
     *
     * @return One of more of the INT_SRC_BITS_T values
     */
    uint8_t getInterruptSource();

    /**
     * Sets the data sampling rate. Higher rates are more precise, but
     * take more time per measurement.
     *
     * @param dsr One of the DSR_BITS_T values
     */
    void setDSR(DSR_BITS_T dsr);


    /**
     * Starts an internal recalibration of analog blocks. This is
     * faster than a soft reset.
     */
    void recalibrateInternal();

    /**
     * Executes a soft reset. All register values are reset to power-on
     * defaults. This function returns when the reset is
     * complete and the device reports it is ready.
     */
    void softReset();

    /**
     * Sets the altitude offset for your region. See the datasheet for
     * more details. Setting this correctly for your region is
     * required for accurate altitude data.
     *
     * @param off Offset
     */
    void setAltitudeOffset(int16_t off);

    /**
     * Sets pressure/altitude thresholds for interrupt generation
     *
     * @param low Low threshold to generate an interrupt
     * @param med Medium threshold to generate an interrupt
     * @param high High threshold to generate an interrupt
     */
    void setPAThreshholds(int16_t low, int16_t med, int16_t high);

    /**
     * Sets temperature thresholds for interrupt generation
     *
     * @param low Low threshold to generate an interrupt
     * @param med Medium threshold to generate an interrupt
     * @param high High threshold to generate an interrupt
     */
    void setTemperatureThreshholds(int8_t low, int8_t med, int8_t high);


  protected:
    mraa::I2c m_i2c;

  private:
    uint8_t m_addr;
    uint8_t m_dsr;

  };
}


