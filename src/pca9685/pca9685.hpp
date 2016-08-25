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
#include <mraa/i2c.h>
#include <mraa/gpio.h>

#define PCA9685_I2C_BUS 0
#define PCA9685_DEFAULT_I2C_ADDR 0x60
// internal oscillator frequency
#define PCA9685_INTERNAL_OSC  25000000.0

// This is a 'special' LED number, used to refer to the ALL_LED registers
// that affect all LED outputs at once.
#define PCA9685_ALL_LED 0xff

namespace upm {
  
  /**
   * @brief PCA9685 PWM Controller library
   * @defgroup pca9685 libupm-pca9685
   * @ingroup adafruit i2c led motor
   */
  /**
   * @library pca9685
   * @sensor pca9685
   * @comname PCA9685 PWM Controller
   * @type led motor
   * @man adafruit
   * @web http://www.adafruit.com/products/815
   * @con i2c
   *
   * @brief API for the PCA9685 16-channel, 12-bit PWM LED Controller
   *
   * This controller is also used on the Adafruit* Motor Shield v2.3
   * board to control up to 4 DC motors, 2 stepper motors, and 2 servo
   * motors.
   *
   * This module was tested with the Adafruit Motor Shield v2.3
   *
   * @image html pca9685.jpg
   * @snippet pca9685.cxx Interesting
   */
  class PCA9685 {
  public:

    /**
     * PCA9685 registers
     */
    typedef enum { REG_MODE1       = 0x00,
                   REG_MODE2       = 0x01,
                   REG_I2C_SA1     = 0x02, // I2C subaddress 1
                   REG_I2C_SA2     = 0x03,
                   REG_I2C_SA3     = 0x04,
                   REG_ALLCALL     = 0x05, // I2C all call address

                   // LED output PWM control
                   REG_LED0_ON_L   = 0x06, // LED0 ON low byte
                   REG_LED0_ON_H   = 0x07, // LED0 ON high byte
                   REG_LED0_OFF_L  = 0x08, // LED0 OFF low byte
                   REG_LED0_OFF_H  = 0x09, // LED0 OFF high byte
                   REG_LED1_ON_L   = 0x0a,
                   REG_LED1_ON_H   = 0x0b,
                   REG_LED1_OFF_L  = 0x0c,
                   REG_LED1_OFF_H  = 0x0d,
                   REG_LED2_ON_L   = 0x0e,
                   REG_LED2_ON_H   = 0x0f,
                   REG_LED2_OFF_L  = 0x10,
                   REG_LED2_OFF_H  = 0x11,
                   REG_LED3_ON_L   = 0x12,
                   REG_LED3_ON_H   = 0x13,
                   REG_LED3_OFF_L  = 0x14,
                   REG_LED3_OFF_H  = 0x15,
                   REG_LED4_ON_L   = 0x16,
                   REG_LED4_ON_H   = 0x17,
                   REG_LED4_OFF_L  = 0x18,
                   REG_LED4_OFF_H  = 0x19,
                   REG_LED5_ON_L   = 0x1a,
                   REG_LED5_ON_H   = 0x1b,
                   REG_LED5_OFF_L  = 0x1c,
                   REG_LED5_OFF_H  = 0x1d,
                   REG_LED6_ON_L   = 0x1e,
                   REG_LED6_ON_H   = 0x1f,
                   REG_LED6_OFF_L  = 0x20,
                   REG_LED6_OFF_H  = 0x21,
                   REG_LED7_ON_L   = 0x22,
                   REG_LED7_ON_H   = 0x23,
                   REG_LED7_OFF_L  = 0x24,
                   REG_LED7_OFF_H  = 0x25,
                   REG_LED8_ON_L   = 0x26,
                   REG_LED8_ON_H   = 0x27,
                   REG_LED8_OFF_L  = 0x28,
                   REG_LED8_OFF_H  = 0x29,
                   REG_LED9_ON_L   = 0x2a,
                   REG_LED9_ON_H   = 0x2b,
                   REG_LED9_OFF_L  = 0x2c,
                   REG_LED9_OFF_H  = 0x2d,
                   REG_LED10_ON_L  = 0x2e,
                   REG_LED10_ON_H  = 0x2f,
                   REG_LED10_OFF_L = 0x30,
                   REG_LED10_OFF_H = 0x31,
                   REG_LED11_ON_L  = 0x32,
                   REG_LED11_ON_H  = 0x33,
                   REG_LED11_OFF_L = 0x34,
                   REG_LED11_OFF_H = 0x35,
                   REG_LED12_ON_L  = 0x36,
                   REG_LED12_ON_H  = 0x37,
                   REG_LED12_OFF_L = 0x38,
                   REG_LED12_OFF_H = 0x39,
                   REG_LED13_ON_L  = 0x3a,
                   REG_LED13_ON_H  = 0x3b,
                   REG_LED13_OFF_L = 0x3c,
                   REG_LED13_OFF_H = 0x3d,
                   REG_LED14_ON_L  = 0x3e,
                   REG_LED14_ON_H  = 0x3f,
                   REG_LED14_OFF_L = 0x40,
                   REG_LED14_OFF_H = 0x41,
                   REG_LED15_ON_L  = 0x42,
                   REG_LED15_ON_H  = 0x43,
                   REG_LED15_OFF_L = 0x44,
                   REG_LED15_OFF_H = 0x45,
                   // 0x46-0xf9 reserved

                   REG_ALL_LED_ON_L  = 0xfa, // write all LED ON L
                   REG_ALL_LED_ON_H  = 0xfb, // write all LED ON H
                   REG_ALL_LED_OFF_L = 0xfc, // write all LED OFF L
                   REG_ALL_LED_OFF_H = 0xfd, // write all LED OFF H
                   REG_PRESCALE      = 0xfe,
                   REG_TESTMODE      = 0xff  // don't use
    } PCA9685_REG_T;
    
    /**
     * MODE1 bits
     */
    typedef enum { MODE1_ALL_CALL    = 0x01, // all call status
                   MODE1_SUB3        = 0x02, // subcall 3 status
                   MODE1_SUB2        = 0x04, // subcall 2 status
                   MODE1_SUB1        = 0x08, // subcall 1 status
                   MODE1_SLEEP       = 0x10, // sleep/normal mode
                   MODE1_AI          = 0x20, // auto-increment enable
                   MODE1_EXTCLK      = 0x40, // external clock enable
                   MODE1_RESTART     = 0x80  // restart status
    } PCA9685_MODE1_T;
    
    /**
     * MODE2 bits
     */
    typedef enum { MODE2_OUTNE0      = 0x01, // output driver enable bit 0
                   MODE2_OUTNE       = 0x02, // output driver enable bit 1
                   MODE2_OUTDRV      = 0x04, // output open-drain/totem pole
                   MODE2_OCH         = 0x08, // output change on STOP or ACK
                   MODE2_INVRT       = 0x10, // output logic state invert
                   MODE2_RESERVE0    = 0x20, // reserved
                   MODE2_RESERVE1    = 0x40, // reserved
                   MODE2_RESERVE2    = 0x80  // reserved
    } PCA9685_MODE2_T;
    
    /**
     * PCA9685 constructor
     *
     * @param bus I2C bus to use
     * @param address Address for this device
     */
    PCA9685(int bus, uint8_t address = PCA9685_DEFAULT_I2C_ADDR, bool raw = false);

    /**
     * PCA9685 destructor
     */
    ~PCA9685();

    /**
     * Writes a byte value into a register
     *
     * @param reg Register location to write into
     * @param byte Byte to write
     * @return True if successful
     */
    bool writeByte(uint8_t reg, uint8_t byte);

    /**
     * Writes a word value into a register. Note: the device must have the
     * auto-increment bit set in the MODE1 register to work.
     *
     * @param reg Register location to write into
     * @param word Word to write
     * @return True if successful
     */
    bool writeWord(uint8_t reg, uint16_t word);

    /**
     * Reads a byte value from a register
     *
     * @param reg Register location to read from
     * @return Value in a specified register
     */
    uint8_t readByte(uint8_t reg);

    /**
     * Reads a word value from a register. Note: the device must have the
     * auto-increment bit set in the MODE1 register to work.
     *
     * @param reg Register location to read from
     * @return Value in a specified register
     */
    uint16_t readWord(uint8_t reg);

    /**
     * Puts the device in or out of the sleep mode. The device is always
     * in the sleep mode upon power-up.
     *
     * @param sleep True to put the device in the sleep mode, false to put out  
     * @return True if successful
     */
    bool setModeSleep(bool sleep);

    /**
     * Sets or clears the FULL ON bit for a given LED
     *
     * @param led LED number; valid values are 0-15, PCA9685_ALL_LED
     * @param val True to set the bit, false to clear it
     * @return True if successful
     */
    bool ledFullOn(uint8_t led, bool val);

    /**
     * Sets or clears the FULL OFF bit for a given LED. If the FULL ON
     * bit is also set, FULL OFF has precedence.
     *
     * @param led LED number; valid values are 0-15, PCA9685_ALL_LED
     * @param val True to set the bit, false to clear it
     * @return True if successful
     */
    bool ledFullOff(uint8_t led, bool val);

    /**
     * Sets the 'LED on' time (0-4,095). See the PCA9685 datasheet for details. 
     *
     * @param led LED number; valid values are 0-15, PCA9685_ALL_LED
     * @param time 12-bit value at which point the LED turns on
     * @return True if successful
     */
    bool ledOnTime(uint8_t led, uint16_t time);

    /**
     * Sets the 'LED off' time (0-4,095). See the PCA9685 datasheet for details. 
     *
     * @param led LED number; valid values are 0-15, PCA9685_ALL_LED
     * @param time 12-bit value at which point the LED turns off
     * @return True if successful
     */
    bool ledOffTime(uint8_t led, uint16_t time);

    /**
     * Sets the prescale value. See the PCA9685 datasheet for
     * details. The prescale can only be set when the device is in
     * the sleep mode.
     *
     * @param prescale Prescale value
     * @return True if successful
     */
    bool setPrescale(uint8_t prescale);

    /**
     * Sets the prescale value based on a desired frequency in Hz. The
     * prescale can only be set when the device is in the sleep mode.
     *
     * @param hz Desired frequency in Hz
     * @param oscFreq Oscillator frequency; default is 25 MHz 
     * @return True if successful
     */
    bool setPrescaleFromHz(float hz, 
                           float oscFreq=PCA9685_INTERNAL_OSC);

    /**
     * Enables or disables the restart capability of the controller
     *
     * @param enabled True to enable, false to disable
     * It is enabled by default.
     */
    void enableRestart(bool enabled) { m_restartEnabled = enabled; };

  private:
    /**
     * Enables the I2C register auto-increment. This needs to be enabled
     * for write/readWord() to work. The contructor enables this by
     * default.
     *
     * @param ai True to enable, false otherwise
     */
    bool enableAutoIncrement(bool ai);

    bool m_restartEnabled;
    mraa_i2c_context m_i2c;
    uint8_t m_addr;
  };
}


