/*
 * Author: William Penner <william.penner@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <string>
#include <mraa/i2c.h>
#include <math.h>

#define AM2315_NAME "am2315"
#define AM2315_I2C_ADDRESS 0x5c

#define AM2315_READ     0x03
#define AM2315_WRITE    0x10

/* AM2315 Commands */
#define AM2315_HUMIDITY 0x00
#define AM2315_TEMP     0x02
#define AM2315_MODEL    0x08
#define AM2315_VERSION  0x0A
#define AM2315_ID       0x0B
#define AM2315_STATUS   0x0F
#define AM2315_USER_A   0x10
#define AM2315_USER_B   0x12

#define AM2315_SAMPLE   2

#define HIGH_PRIORITY   99

namespace upm {

/**
 * @brief AM2315 temperature & humidity sensor library
 * @defgroup am2315 libupm-am2315
 * @ingroup adafruit i2c temp
 */

/**
 * @library am2315
 * @sensor am2315
 * @comname AM2315 Temperature & Humidity Sensor
 * @type temp
 * @man adafruit
 * @web http://www.adafruit.com/products/1293
 * @con i2c
 *
 * @brief C++ API for AM2315 chip (Temperature & Humidity Sensor)
 *
 * Measurement Specialties [AM2315]
 * (http://www.aosong.com/asp_bin/Products/en/AM2315.pdf)
 * is a digital humidity sensor with temperature output.
 * RH will report between 0 and 100% and temperature range is
 * -40 to +125 degC.
 * The sampling period of this sensor is 2 seconds.  Reads occurring
 * more often than that will return cached data.
 *
 * @image html am2315.jpeg
 * @snippet am2315.cxx Interesting
 */
class AM2315 {
    public:
        /**
         * Instanciates a AM2315 object
         *
         * @param bus number of used bus
         * @param devAddr address of used i2c device
         * @param mode AM2315 oversampling
         */
        AM2315 (int bus, int devAddr=AM2315_I2C_ADDRESS);

        /**
         * AM2315 object destructor, basicaly it close i2c connection.
         */
        ~AM2315 ();

        /**
         * Get the current measured humidity [RH]
         * Data is updated every 2 seconds - accesses more often than
         * that will return cached data
         */
        float getHumidity(void);

        /**
         * Get the humidity cell temperature [degC]
         * Data is updated every 2 seconds - accesses more often than
         * that will return cached data
         */
        float getTemperature(void);

        /**
         * Get the humidity cell temperature [degF]
         * Data is updated every 2 seconds - accesses more often than
         * that will return cached data
         */
        float getTemperatureF(void);

        /**
         * Function intended to test the device and verify it
         * is correctly operating.
         *
         */
        int testSensor(void);

        /**
         * Write four byte (32b) register
         *
         * Note: These access routines are not the normal accesses to an i2c
         * device.  The AM2315 contains a microcontroller that manages the
         * actual readings.  These handlers then make requests over i2c using
         * a protocol defined by the AM2315.
         *
         * @param reg address of a register
         * @param ival 32b value
         */
        int i2cWriteReg_32(int reg, uint32_t ival);

        /**
         * Write two byte (16b) register
         *
         * @param reg address of a register
         * @param ival 16b value
         */
        int i2cWriteReg_16(int reg, uint16_t ival);

        /**
         * Write one byte (8b) register
         *
         * @param reg address of a register
         * @param ival 8b value
         */
        int i2cWriteReg_8(int reg, uint8_t ival);

        /**
         * Read four bytes register
         *
         * @param reg address of a register
         */
        uint32_t i2cReadReg_32 (int reg);

        /**
         * Read two bytes register
         *
         * @param reg address of a register
         */
        uint16_t i2cReadReg_16 (int reg);

        /**
         * Read one byte register
         *
         * @param reg address of a register
         */
        uint8_t i2cReadReg_8 (int reg);

    private:

        char* m_name;

        int m_controlAddr;
        int m_bus;
        mraa_i2c_context m_i2ControlCtx;

        void update_values(void);
        uint8_t i2cReadReg(int reg, uint8_t* data, int ilen);
        int i2cWriteReg(uint8_t reg, uint8_t* data, uint8_t ilen);
        uint16_t crc16(uint8_t* ptr, uint8_t len);

        int32_t   m_temperature;
        int32_t   m_humidity;

        uint16_t  m_model;
        uint16_t  m_version;
        uint32_t  m_id;

        time_t    m_last_time;

        int       m_base_priority;
        pthread_t this_thread;
};

}
