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
#include <mraa/i2c.hpp>

#define HP20X_I2C_BUS 0
#define HP20X_DEFAULT_I2C_ADDR 0x76

namespace upm {
  
  /**
   * @brief HP20X I2C Barometer (high accuracy) library
   * @defgroup hp20x libupm-hp20x
   * @ingroup seeed i2c pressure
   */

  /**
   * @library hp20x
   * @sensor hp20x
   * @comname Grove Barometer (high accuracy)
   * @altname HP20X Barometer (high accuracy)
   * @type pressure
   * @man seeed
   * @web http://www.seeedstudio.com/depot/Grove-Barometer-HighAccuracy-p-1865.html
   * @con i2c
   *
   * @brief API for the HP20X based Grove Barometer (high accuracy)
   *
   * This is a high accuracy barometer providing pressure, altitude
   * and temperature data.  It can be calabrated for a given altitude
   * offset, and a wide range of interrupt generating capabilities are
   * supported.  As usual, see the HP20x datasheet for more detail.
   *
   * This module was developed using a Grove Barometer (High Accuracy)
   * based on an HP206C chip.
   *
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

      INT_SRC_TH_ERR             = 0x80 // threshhold error
    } INT_SRC_BITS_T;

    /**
     * PARA bits
     */
    typedef enum {
      // 0x01-0x40 reserved
      PARA_CMPS_EN               = 0x80 // compensation enable
    } PARA_BITS_T;

    /**
     * hp20x constructor
     *
     * @param bus i2c bus to use
     * @param address the address for this device
     */
    HP20X(int bus=HP20X_I2C_BUS, uint8_t address=HP20X_DEFAULT_I2C_ADDR);

    /**
     * HP20X Destructor
     */
    ~HP20X();
    
    /**
     * set up initial values and start operation
     *
     * @param dsr the data sampling rate: one of the DSR_BITS_T values
     * @return true if successful
     */
    bool init(DSR_BITS_T dsr=DSR_4096);

    /**
     * send a command to the device
     *
     * @param cmd command to send, one of the HP20X_CMD_T values, usually
     * @return true if successful
     */
    bool writeCmd(uint8_t cmd);

    /**
     * write a value to a register
     *
     * @param reg register to write, one of the HP20X_REG_T values
     * @param data value to write
     * @return true if successful
     */
    bool writeReg(HP20X_REG_T reg, uint8_t data);

    /**
     * read a register and return it's value
     *
     * @param reg register to read, one of the HP20X_REG_T values
     * @return value of specified register
     */
    uint8_t readReg(HP20X_REG_T reg);

    /**
     * read 3 bytes of data in response to a conversion request, and
     * convert to an integer
     *
     * @return value read back (temperature, pressure, etc)
     */
    int readData();

    /**
     * check to see if the DR_RDY bit is set, indicating the device
     * can accept commands
     *
     * @return true if device is ready, false otherwise
     */
    bool isReady();

    /**
     * check to see if device is ready and sleep/retry if not.
     * Returns once device indicates it's ready.
     *
     * @return true if device is ready, false if retries exhausted
     */
    bool waitforDeviceReady();

    /**
     * return the temperature in celcius
     *
     * @return the temperature
     */
    float getTemperature();

    /**
     * return the pressure in millibars
     *
     * @return the pressure
     */
    float getPressure();

    /**
     * return the computed altitude in meters
     *
     * @return the altitude
     */
    float getAltitude();

    /**
     * enable or disable the on-chip compensator.  This allows the
     * chip to filter and clean up the output data.
     *
     * @param enable true to enable, false otherwise
     */
    void compensationEnable(bool enable);

    /**
     * setup the interrupt enable register.  This register defines
     * which events can cause a hardware interrupt pin to be pulled high
     * (active).
     *
     * @param bits one or more of the INT_EN_BITS_T bits
     * @return true if successful, false otherwise
     */
    bool setInterruptEnable(uint8_t bits);

    /**
     * setup the interrupt configuration register.  This register
     * defines which events can cause an interrupt to be indicated.
     *
     * @param bits one or more of the INT_EN_BITS_T bits
     * @return true if successful, false otherwise
     */
    bool setInterruptConfig(uint8_t bits);

    /**
     * get the interrupt source register.  This register indicates
     * which interrupts have been triggered.  In addition, it
     * indicates when certain operations have been completed.
     *
     * @return one of more of the INT_SRC_BITS_T values
     */
    uint8_t getInterruptSource();

    /**
     * set the data sampling rate.  Higher rates are more precise, but
     * take more time per measurement.
     *
     * @param dsr one of the DSR_BITS_T values
     */
    void setDSR(DSR_BITS_T dsr);


    /**
     * start an internal recalibration of the analog blocks.  This is
     * faster than a soft reset.
     */
    void recalibrateInternal();

    /**
     * execute a soft reset.  All register values are reset to power
     * on defaults.  This function will return when the reset is
     * complete and the device reports that it is ready.
     */
    void softReset();

    /**
     * Set the altitude offset for your region.  See the datasheet for
     * more details.  Setting this correctly for your region is
     * required for accurate altitude data.
     *
     * @param off the offset
     */
    void setAltitudeOffset(int16_t off);

    /**
     * set the pressure/altitude thresholds for interrupt generation.
     *
     * @param low the low threshold for generating an interrupt
     * @param med the medium threshold for generating an interrupt
     * @param high the high threshold for generating an interrupt
     */
    void setPAThreshholds(int16_t low, int16_t med, int16_t high);

    /**
     * set the temperature thresholds for interrupt generation.
     *
     * @param low the low threshold for generating an interrupt
     * @param med the medium threshold for generating an interrupt
     * @param high the high threshold for generating an interrupt
     */
    void setTemperatureThreshholds(int8_t low, int8_t med, int8_t high);


  protected:
    mraa::I2c m_i2c;

  private:
    uint8_t m_addr;
    uint8_t m_dsr;

  };
}


