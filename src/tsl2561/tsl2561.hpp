/*
 * Author: Nandkishor Sonar <Nandkishor.Sonar@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * LIGHT-TO-DIGITAL CONVERTER [TAOS-TSL2561]
 *   Inspiration and lux calculation formulas from data sheet
 *   URL: http://www.adafruit.com/datasheets/TSL2561.pdf
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
#include <math.h>

namespace upm {

#define TSL2561_Address          (0x29)  //Device address

// Integration time
#define  INTEGRATION_TIME0_13MS  (0x00)  // 13.7ms
#define  INTEGRATION_TIME1_101MS (0x01)  // 101ms
#define  INTEGRATION_TIME2_402MS (0x02)  // 402ms

// Integration time
#define  GAIN_0X  (0x00)                // No gain - Low
#define  GAIN_16X (0x10)                // 16x gain - High

// Power control bits
#define CONTROL_POWERON   (0x03)        // ON
#define CONTROL_POWEROFF  (0x00)        // OFF

// TSL2561 registers
#define  REGISTER_Control   (0x80)
#define  REGISTER_Timing    (0x81)
#define  REGISTER_Interrupt (0x86)
#define  REGISTER_Channal0L (0x8C)
#define  REGISTER_Channal0H (0x8D)
#define  REGISTER_Channal1L (0x8E)
#define  REGISTER_Channal1H (0x8F)

// Lux calculations differ slightly for CS package
#define LUX_SCALE         (14)      // Scale by 2^14
#define LUX_RATIOSCALE    (9)       // Scale ratio by 2^9
#define LUX_CHSCALE       (10)      // Scale channel values by 2^10
#define LUX_CHSCALE_TINT0 (0x7517)  // 322/11 * 2^TSL2561_LUX_CHSCALE
#define LUX_CHSCALE_TINT1 (0x0FE7)  // 322/81 * 2^TSL2561_LUX_CHSCALE

// CS package Coefficients
#define LUX_K1C           (0x0043)  // 0.130 * 2^RATIO_SCALE
#define LUX_B1C           (0x0204)  // 0.0315 * 2^LUX_SCALE
#define LUX_M1C           (0x01ad)  // 0.0262 * 2^LUX_SCALE
#define LUX_K2C           (0x0085)  // 0.260 * 2^RATIO_SCALE
#define LUX_B2C           (0x0228)  // 0.0337 * 2^LUX_SCALE
#define LUX_M2C           (0x02c1)  // 0.0430 * 2^LUX_SCALE
#define LUX_K3C           (0x00c8)  // 0.390 * 2^RATIO_SCALE
#define LUX_B3C           (0x0253)  // 0.0363 * 2^LUX_SCALE
#define LUX_M3C           (0x0363)  // 0.0529 * 2^LUX_SCALE
#define LUX_K4C           (0x010a)  // 0.520 * 2^RATIO_SCALE
#define LUX_B4C           (0x0282)  // 0.0392 * 2^LUX_SCALE
#define LUX_M4C           (0x03df)  // 0.0605 * 2^LUX_SCALE
#define LUX_K5C           (0x014d)  // 0.65 * 2^RATIO_SCALE
#define LUX_B5C           (0x0177)  // 0.0229 * 2^LUX_SCALE
#define LUX_M5C           (0x01dd)  // 0.0291 * 2^LUX_SCALE
#define LUX_K6C           (0x019a)  // 0.80 * 2^RATIO_SCALE
#define LUX_B6C           (0x0101)  // 0.0157 * 2^LUX_SCALE
#define LUX_M6C           (0x0127)  // 0.0180 * 2^LUX_SCALE
#define LUX_K7C           (0x029a)  // 1.3 * 2^RATIO_SCALE
#define LUX_B7C           (0x0037)  // 0.00338 * 2^LUX_SCALE
#define LUX_M7C           (0x002b)  // 0.00260 * 2^LUX_SCALE
#define LUX_K8C           (0x029a)  // 1.3 * 2^RATIO_SCALE
#define LUX_B8C           (0x0000)  // 0.000 * 2^LUX_SCALE
#define LUX_M8C           (0x0000)  // 0.000 * 2^LUX_SCALE

 /**
 * @brief TSL2561 Digital Light Sensor library
 * @defgroup tsl2561 libupm-tsl2561
 * @ingroup seeed i2c light eak
 */
/**
 * @library tsl2561
 * @sensor tsl2561
 * @comname TSL2561 Light Sensor
 * @altname Grove Digital Light Sensor
 * @type light
 * @man seeed
 * @web http://www.seeedstudio.com/wiki/Grove_-_Digital_Light_Sensor
 * @con i2c
 * @kit eak
 *
 * @brief API for the TSL2561 Digital Light Sensor
 * 
 *   TSL2560 and TSL2561 are light-to-digital converters that transform
 *   light intensity to a digital signal output capable of a direct I2C (TSL2561) interface
 *
 * @image html tsl2561.jpg
 * @snippet tsl2561.cxx Interesting
 */
class TSL2561{
    public:
       /**
        * Instantiates a TSL2561 object
        *
        * @param bus Number of the used bus
        * @param devAddr Address of the used I2C device
        * @param gain Correct gain to use
        * @param integration Time to use
        */
        TSL2561(int bus=0, uint8_t devAddr=TSL2561_Address, uint8_t gain=GAIN_0X, uint8_t integrationTime=INTEGRATION_TIME1_101MS);

       /**
        * GY65 object destructor; powers down TSL2561 and closes the I2C connection.
        */
        ~TSL2561();

       /**
        * Gets the calculated lux reading from TSL2561
        *
        * @return Calculated lux value from the sensor
        */
        int getLux();

    private:
       /**
        * Writes to a TSL2561 register
        *
        * @param reg Addess to write
        * @param Value to write
        * @return mraa::Result
        */
        mraa::Result i2cWriteReg(uint8_t reg, uint8_t value);

       /**
        * Reads from a TSL2561 register
        *
        * @param reg Addess to read
        * @param data Byte read from the register
        * @return mraa::Result
        */
        mraa::Result i2cReadReg(uint8_t reg, uint8_t &data);

        int m_bus;
        std::string m_name;
        int m_controlAddr;
        mraa::I2c m_i2ControlCtx;

        uint8_t m_gain;
        uint8_t m_integrationTime;
};

}

