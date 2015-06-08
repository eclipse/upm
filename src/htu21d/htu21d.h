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

#define HTU21D_NAME "htu21d"
#define HTU21D_I2C_ADDRESS 0x40

/* HTU21D Commands */
#define HTU21D_READ_TEMP_HOLD     0xE3
#define HTU21D_READ_HUMIDITY_HOLD 0xE5
#define HTU21D_WRITE_USER_REG     0xE6
#define HTU21D_READ_USER_REG      0xE7
#define HTU21D_SOFT_RESET         0xFE

/* User Register Bit Definition */
#define HTU21D_DISABLE_OTP        0x02
#define HTU21D_HEATER_ENABLE      0x04
#define HTU21D_END_OF_BATTERY     0x40
#define HTU21D_RESO_RH12_T14      0x00
#define HTU21D_RESO_RH8_T12       0x01
#define HTU21D_RESO_RH10_T13      0x80
#define HTU21D_RESO_RH11_T11      0x81

namespace upm {

/**
 * @brief HTU21D humidity sensor library
 * @defgroup htu21d libupm-htu21d
 * @ingroup seeed adafruit sparkfun i2c temp
 */

/**
 * @library htu21d
 * @sensor htu21d
 * @comname HTU21D Temperature & Humidity Sensor
 * @type temp
 * @man seeed adafruit sparkfun
 * @web http://www.meas-spec.com/downloads/HTU21D.pdf
 * @con i2c
 *
 * @brief API for HTU21D chip (Atmospheric Pressure Sensor)
 *
 * Measurement Specialties [HTU21D] is a digital humidity sensor with
 * temperature output.
 * RH will report between 0 and 100% and temperature range is
 * -40 to +125 degC.  Note that the getCompRH is the preferred
 * function below (passing true to cause a measurement cycle).  If
 * the actual values used for the compensated ready are necessary, use
 * the getHumidity(false) and getTemperature(false) functions following
 * the getCompRH call.
 * Also note that the sensor should not perform more than a couple of
 * measurements per second to limit the heating of the sensor.
 *
 * @image html htu21d.jpeg
 * @snippet htu21d.cxx Interesting
 */
class HTU21D {
    public:
        /**
         * Instanciates a HTU21D object
         *
         * @param bus number of used bus
         * @param devAddr address of used i2c device
         * @param mode HTU21D oversampling
         */
        HTU21D (int bus, int devAddr=HTU21D_I2C_ADDRESS);

        /**
         * HTU21D object destructor, basicaly it close i2c connection.
         */
        ~HTU21D ();

        /**
         * Initiate a temp/pressure mesasurement and wait for function
         * to complete.  The humidity and temp registers can be read
         * after this call.
         */
        int sampleData(void);

        /**
         * Get the current measured humidity [RH]
         */
        float getHumidity(int bSampleData = false);

        /**
         * Get the humidity cell temperature [degC]
         */
        float getTemperature(int bSampleData = false);

        /**
         * Using the current humidity and temperature the function
         * will calculate the compensated RH using the equation from
         * the datasheet.
         */
        float getCompRH(int bSampleData = true);

        /**
         * Set the heater state.  The heater is used to either test
         * the sensor functionality since the temp should increase
         * 0.5 to 1.5 degC and the humidity should decrease.  The
         * testSensor() function below will use the heater.
         *
         * @param bEnable Set to non-zero to turn on heater
         */
         int setHeater(int bEnable = false);

        /**
         * Perform a soft RESET of the MPL3115A2 device to ensure
         * it is in a known state.  This function can be used to reset
         * the min/max temperature and pressure values.
         */
        int resetSensor(void);

        /**
         * Function intended to test the device and verify it
         * is correctly operating.
         *
         */
        int testSensor(void);

        /**
         * Write to one byte register
         *
         * @param reg address of a register
         * @param value byte to be written
         */
        mraa_result_t i2cWriteReg (uint8_t reg, uint8_t value);

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

        /**
         * Convert temp register to degC * 1000
         */
        int32_t convertTemp(int32_t regval);

        /**
         * Convert RH register to %RH * 1000
         */
        int32_t convertRH(int32_t regval);

        std::string m_name;

        int m_controlAddr;
        int m_bus;
        mraa_i2c_context m_i2ControlCtx;

        int32_t m_temperature;
        int32_t m_humidity;
};

}
