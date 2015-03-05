/*
 * Author: Scott Ware <scott.r.ware@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <mraa/i2c.h>

#define ADS1015_DEFAULT_I2C_BUS 1
#define ADS1015_DEFAULT_I2C_ADDR 0x48

#define ADS1015_RESOLUTION  4096 // 12 bits

/**
 * ADS1015 registers
 */

#define ADS1015_REG_CONVERSION      0x00
#define ADS1015_REG_CONFIG          0x01
#define ADS1015_REG_LO_THRESH       0x02
#define ADS1015_REG_HI_THRESH       0x03

#define ADS1015_CONV_START          0x8000

#define ADS1015_MODE_CONT           0xFEFF
#define ADS1015_MODE_SINGLE         0xFFFF


#define ADS1015_CONFIG_DEFAULT      0x583

#define ADS1015_DEFAULT_VREF        3.0

namespace upm {

  /**
   * valid cycle times for automatic conversion mode
   */

  typedef enum { ADS1015_CYCLE_NONE = 0,  // disabled
                 ADS1015_CYCLE_32   = 1,  // 27 ksps
                 ADS1015_CYCLE_64   = 2,  // 13.5
                 ADS1015_CYCLE_128  = 3,  // 6.7
                 ADS1015_CYCLE_256  = 4,  // 3.4
                 ADS1015_CYCLE_512  = 5,  // 1.7
                 ADS1015_CYCLE_1024 = 6,  // 0.9
                 ADS1015_CYCLE_2048 = 7   // 0.4
  } ADS1015_CYCLE_TIME_T;

  /**
   * @brief C++ API for the ADS1015 I2C ADC
   *
   * UPM module for the Texas Instruments ADS1015 12 bit Analog to Digital (ADC) converter.
   *
   * @ingroup i2c ads1015
   * @snippet ads1015.cxx Interesting
   */
  class ADS1015 {
  public:
    /**
     * ads1015 ADC constructor
     *
     * @param bus i2c bus to use
     * @param address the address for this sensor; default is 0x48
     * @param vref reference voltage for this sensor; default is 3.0
     */
    ADS1015(int bus = ADS1015_DEFAULT_I2C_BUS, uint8_t address = ADS1015_DEFAULT_I2C_ADDR,
               float vref = ADS1015_DEFAULT_VREF);

    /**
     * ADS1015 Destructor
     */
    ~ADS1015();

    /**
     * Write byte value into register
     *
     * @param reg register location to write into
     * @param byte byte to write
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t writeByte(uint8_t reg, uint8_t byte);

    /**
     * Write word value into register
     *
     * @param reg register location to write into
     * @param word word to write
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t writeWord(uint8_t reg, uint16_t word);

    /**
     * Read byte value from register
     *
     * @param reg register location to read from
     * @return value at specified register
     */
    uint8_t readByte(uint8_t reg);

    /**
     * Read word value from register
     *
     * @param reg register location to read from
     * @return value at specified register
     */
    uint16_t readWord(uint8_t reg);

    /**
     * Read current value of conversion
     *
     * @return current conversion value
     */
    uint16_t value();

    /**
     * Convert a supplied value to voltage based on set VREF
     *
     * @param val value of conversion (from value())
     * @return conversion value in volts
     */
    float valueToVolts(uint16_t val);

  private:
    mraa_i2c_context m_i2c;
    uint8_t m_addr;
    float m_vref;
    bool m_alertLow;
    bool m_alertHigh;
  };
}


