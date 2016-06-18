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

#include <stdint.h>
#include <sys/time.h>

#include <string>
#include <mraa/i2c.h>

#define AT42QT1070_I2C_BUS 0
#define AT42QT1070_DEFAULT_I2C_ADDR 0x1b

namespace upm
{
/**
 * @brief Atmel* AT42QT1070 QTouch* Sensor library
 * @defgroup at42qt1070 libupm-at42qt1070
 * @ingroup seeed i2c touch
 */

/**
 * @library at42qt1070
 * @sensor at42qt1070
 * @comname AT42QT1070 QTouch Sensor
 * @altname Grove QTouch Sensor
 * @type touch
 * @man seeed
 * @con i2c
 *
 * @brief API for the Atmel AT42QT1070 QTouch Sensor
 *
 * This class implements support for the Atmel AT42QT1070 QTouch
 * sensor, which supports 7 capacitive buttons.
 *
 * It was developed using a Grove-Q Touch Sensor board.
 *
 * @image html at42qt1070.jpg
 * @snippet at42qt1070.cxx Interesting
 */
class AT42QT1070
{
  public:
    // registers
    typedef enum {
        REG_CHIPID = 0,
        REG_FWVERS = 1,

        REG_DETSTATUS = 2, // detection status
        REG_KEYSTATUS = 3, // key status

        REG_KEYSIG0_H = 4, // key signal
        REG_KEYSIG0_L = 5,
        REG_KEYSIG1_H = 6,
        REG_KEYSIG1_L = 7,
        REG_KEYSIG2_H = 8,
        REG_KEYSIG2_L = 9,
        REG_KEYSIG3_H = 10,
        REG_KEYSIG3_L = 11,
        REG_KEYSIG4_H = 12,
        REG_KEYSIG4_L = 13,
        REG_KEYSIG5_H = 14,
        REG_KEYSIG5_L = 15,
        REG_KEYSIG6_H = 16,
        REG_KEYSIG6_L = 17,

        REG_REFDATA0_H = 18, // key reference data
        REG_REFDATA0_L = 19,
        REG_REFDATA1_H = 20,
        REG_REFDATA1_L = 21,
        REG_REFDATA2_H = 22,
        REG_REFDATA2_L = 23,
        REG_REFDATA3_H = 24,
        REG_REFDATA3_L = 25,
        REG_REFDATA4_H = 26,
        REG_REFDATA4_L = 27,
        REG_REFDATA5_H = 28,
        REG_REFDATA5_L = 29,
        REG_REFDATA6_H = 30,
        REG_REFDATA6_L = 31,

        REG_NTHR0 = 32, // negative threshold level
        REG_NTHR1 = 33,
        REG_NTHR2 = 34,
        REG_NTHR3 = 35,
        REG_NTHR4 = 36,
        REG_NTHR5 = 37,
        REG_NTHR6 = 38,

        REG_AVE0 = 39, // key suppression
        REG_AVE1 = 40,
        REG_AVE2 = 41,
        REG_AVE3 = 42,
        REG_AVE4 = 43,
        REG_AVE5 = 44,
        REG_AVE6 = 45,

        REG_DI0 = 46, // detection integrator
        REG_DI1 = 47,
        REG_DI2 = 48,
        REG_DI3 = 49,
        REG_DI4 = 50,
        REG_DI5 = 51,
        REG_DI6 = 52,

        REG_GUARD = 53, // FastOutDI/Max Cal/Guard channel
        REG_LP = 54,    // low power
        REG_MAXON = 55, // max on duration
        REG_CALIBRATE = 56,
        REG_RESET = 57
    } AT42QT1070_REG_T;

    // detection register bits
    typedef enum {
        DET_TOUCH = 0x01,
        // 0x02-0x20 reserved
        DET_OVERFLOW = 0x40,
        DET_CALIBRATE = 0x80
    } AT42QT1070_DET_T;


    /**
     * AT42QT1070 constructor
     *
     * @param bus I2C bus to use
     * @param address Address for this sensor
     */
    AT42QT1070(int bus, uint8_t address = AT42QT1070_DEFAULT_I2C_ADDR);

    /**
     * AT42QT1070 destructor
     */
    ~AT42QT1070();

    /**
     * Writes a byte value into the register
     *
     * @param reg Register location to write into
     * @param byte Byte to write
     * @return True if successful
     */
    bool writeByte(uint8_t reg, uint8_t byte);

    /**
     * Writes a word value into the register.  Note: the device must have the
     * auto-increment bit set in the MODE1 register to work.
     *
     * @param reg Register location to write into
     * @param word Word to write
     * @return True if successful
     */
    bool writeWord(uint8_t reg, uint16_t word);

    /**
     * Read a byte value from the register
     *
     * @param reg Register location to read from
     * @return Value in the specified register
     */
    uint8_t readByte(uint8_t reg);

    /**
     * Read a word value from the register.  Note: the device must have the
     * auto-increment bit set in the MODE1 register to work.
     *
     * @param reg Register location to read from
     * @return Value in the specified register
     */
    uint16_t readWord(uint8_t reg);


    /**
     * Reads the Chip ID register on the sensor
     *
     * @return Value of the Chip ID register
     */
    uint8_t readChipID(void);

    /**
     * Reads the current touch status and detection state
     *
     * @return Key status bits for all keys (0-6)
     */
    void updateState();


    /**
     * Reads the current low-power mode setting
     *
     * @return Low-power mode setting from the sensor
     */
    uint8_t getLPMode(void);

    /**
     * Changes the low-pomer mode setting on the sensor
     *
     * @param mode dDsired new mode
     * @return New setting on the sensor
     */
    uint8_t setLPMode(uint8_t mode);


    /**
     * Reads the current averaging factor setting for a key
     *
     * @param key Key being read
     * @return Averaging factor
     */
    uint8_t getAVE(uint8_t key);

    /**
     * Changes the averaging factor setting for a key
     *
     * @param key Key being changed
     * @param ave New averaging factor
     * @return New averaging factor as read from the device
     */
    uint8_t setAVE(uint8_t key, uint8_t ave);

    /**
     * Reads the AKS group of which a key is part
     *
     * @param key Key (0-6) being queried
     * @return AKS group of which the key is part
     */
    uint8_t getAKSGroup(uint8_t key);

    /**
     * Changes the AKS group of which a key is part
     *
     * @param key Key (0-6) being changed
     * @param group New group for the key
     * @return New value on the sensor
     */
    uint8_t setAKSGroup(uint8_t key, uint8_t group);

    /**
     * Returns the overflow indicator
     *
     * @return True if overflow is indicated
     */
    bool
    isOverflowed()
    {
        return m_overflow;
    };

    /**
     * Returns the calibrating indicator
     *
     * @return True if calibration is in progress
     */
    bool
    isCalibrating()
    {
        return m_calibrating;
    };

    /**
     * Issues a reset command
     *
     * @return True if successful
     */
    bool reset();

    /**
     * Issues a calibrate command
     *
     * @return True if successful
     */
    bool calibrate();

    /**
     * Gets the current button states
     *
     * @returns Button states
     */
    uint8_t
    getButtons()
    {
        return m_buttonStates;
    };

  private:
    uint8_t m_buttonStates;
    bool m_calibrating;
    bool m_overflow;

    mraa_i2c_context m_i2c;
    uint8_t m_addr;
};
}
