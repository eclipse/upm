/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Credits to Seeed Studeo.
 * Based on Seeed Studeo code example,
 * http://www.seeedstudio.com/wiki/index.php?title=Twig_-_I2C_Color_Sensor_v0.9b.
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

#define ADDR                        0x39 // device address

#define REG_CTL                     0x80
#define REG_TIMING                  0x81
#define REG_INT                     0x82
#define REG_INT_SOURCE              0x83
#define REG_ID                      0x84
#define REG_GAIN                    0x87
#define REG_LOW_THRESH_LOW_BYTE     0x88
#define REG_LOW_THRESH_HIGH_BYTE    0x89
#define REG_HIGH_THRESH_LOW_BYTE    0x8A
#define REG_HIGH_THRESH_HIGH_BYTE   0x8B
#define REG_BLOCK_READ              0xCF
#define REG_GREEN_LOW               0xD0
#define REG_GREEN_HIGH              0xD1
#define REG_RED_LOW                 0xD2
#define REG_RED_HIGH                0xD3
#define REG_BLUE_LOW                0xD4
#define REG_BLUE_HIGH               0xD5
#define REG_CLEAR_LOW               0xD6
#define REG_CLEAR_HIGH              0xD7
#define CTL_DAT_INIITIATE           0x03
#define CLR_INT                     0xE0

/* Timing Register */
#define SYNC_EDGE                   0x40
#define INTEG_MODE_FREE             0x00
#define INTEG_MODE_MANUAL           0x10
#define INTEG_MODE_SYN_SINGLE       0x20
#define INTEG_MODE_SYN_MULTI        0x30

#define INTEG_PARAM_PULSE_COUNT1    0x00
#define INTEG_PARAM_PULSE_COUNT2    0x01
#define INTEG_PARAM_PULSE_COUNT4    0x02
#define INTEG_PARAM_PULSE_COUNT8    0x03

/* Interrupt Control Register */
#define INTR_STOP                   40
#define INTR_DISABLE                0x00
#define INTR_LEVEL                  0x10
#define INTR_PERSIST_EVERY          0x00
#define INTR_PERSIST_SINGLE         0x01

/* Interrupt Souce Register */
#define INT_SOURCE_GREEN            0x00
#define INT_SOURCE_RED              0x01
#define INT_SOURCE_BLUE             0x10
#define INT_SOURCE_CLEAR            0x03

/* Gain Register */
#define GAIN_1                      0x00
#define GAIN_4                      0x10
#define GAIN_16                     0x20
#define GANI_64                     0x30
#define PRESCALER_1                 0x00
#define PRESCALER_2                 0x01
#define PRESCALER_4                 0x02
#define PRESCALER_8                 0x03
#define PRESCALER_16                0x04
#define PRESCALER_32                0x05
#define PRESCALER_64                0x06

#define HIGH                        1
#define LOW                         0

namespace upm {

typedef struct {
    uint16_t r;
    uint16_t g;
    uint16_t b;
    uint16_t clr;
} tcs3414sc_rgb_t;

/**
 * @brief TCS3414CS Color Sensor library
 * @defgroup tcs3414cs libupm-tcs3414cs
 * @ingroup seeed i2c color
 */
/**
 * @library tcs3414cs
 * @sensor tcs3414cs
 * @comname TCS3414CS Color Sensor
 * @altname Grove Color Sensor
 * @type color
 * @man seeed
 * @web http://www.seeedstudio.com/wiki/Grove_-_I2C_Color_Sensor
 * @con i2c
 *
 * @brief API for the TCS3414CS Color Sensor
 * 
 * This module defines the TCS3414CS interface for the color sensor
 *
 * @image html tcs3414cs.jpg
 * @snippet tcs3414cs.cxx Interesting
 */
class TCS3414CS {
    public:
        /**
         * Instantiates a TCS3414CS object
         *
         * @param bus Number of the used bus
         */
        TCS3414CS ();

        /**
         * Gets the RGB value from the sensor.
         *
         * @param rgb Color values
         */
        void readRGB (tcs3414sc_rgb_t * rgb);

        /**
         * Clears interrupts.
         */
        void clearInterrupt ();

        /**
         * Returns the name of the component
         */
        std::string name()
        {
            return m_name;
        }
    private:
        std::string m_name;
        mraa::I2c m_i2Ctx;

        uint16_t i2cReadReg_N (int reg, unsigned int len, uint8_t * buffer);
        mraa::Result i2cWriteReg_N (uint8_t reg, unsigned int len, uint8_t * buffer);
        mraa::Result i2cWriteReg (uint8_t reg, uint8_t data);
};

}
