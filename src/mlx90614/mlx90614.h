/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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

#define DEVICE_ADDR         0x5A // device address

// RAM
#define MLX90614_RAWIR1     0x04
#define MLX90614_RAWIR2     0x05
#define MLX90614_TA         0x06
#define MLX90614_TOBJ1      0x07
#define MLX90614_TOBJ2      0x08
// EEPROM
#define MLX90614_TOMAX      0x20
#define MLX90614_TOMIN      0x21
#define MLX90614_PWMCTRL    0x22
#define MLX90614_TARANGE    0x23
#define MLX90614_EMISS      0x24
#define MLX90614_CONFIG     0x25
#define MLX90614_ADDR       0x0E
#define MLX90614_ID1        0x3C
#define MLX90614_ID2        0x3D
#define MLX90614_ID3        0x3E
#define MLX90614_ID4        0x3F

#define HIGH                    1
#define LOW                     0

namespace upm {

/**
 * @brief mlx90614 temperature sensor library
 * @defgroup mlx90614 libupm-mlx90614
 */

/**
 * @brief C++ API for MLX90614
 *
 * This file defines the MLX90614 C++ interface for libmlx90614
 *
 * @ingroup mlx90614 i2c
 * @snippet mlx90614.cxx Interesting
 */
class MLX90614 {
    public:

        /**
         * Instanciates a MLX90614 object
         *
         * @param bus number of used bus
         * @param devAddr addres of used i2c device
         */
        MLX90614 (int bus=0, int devAddr=0x5A);

        /**
         * MLX90614 object destructor, basicaly it close i2c connection.
         */
        ~MLX90614 ();

        /**
         * Read object temperature in Fahrenheit
         */
        float readObjectTempF(void);

        /**
         * Read ambient temperature in Fahrenheit
         */
        float readAmbientTempF(void);
        /**
         * Read object temperature in Celsius
         */
        float readObjectTempC(void);

        /**
         * Read ambient temperature in Celsius
         */
        float readAmbientTempC(void);

        /**
         * Return name of the component
         */
        std::string name()
        {
            return m_name;
        }
    private:
        std::string m_name;

        int m_i2cAddr;
        int m_bus;
        mraa_i2c_context m_i2Ctx;

        uint16_t i2cReadReg_N (int reg, unsigned int len, uint8_t * buffer);
        mraa_result_t i2cWriteReg_N (uint8_t reg, unsigned int len, uint8_t * buffer);
        float readTemperature (uint8_t address);
};

}
