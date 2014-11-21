/*
 * Author: William Penner <william.penner@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Based on code adapted from the kernel HTU21 driver and
 * code by: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#pragma once

#include <string>
#include <mraa/i2c.h>
#include <math.h>

#define HTU21D_NAME     "htu21d"
#define HTU21D_I2C_ADDRESS  0x40

/* HTU21 Commands */
#define HTU21D_T_MEASUREMENT_HM   0xE3
#define HTU21D_RH_MEASUREMENT_HM  0xE5

namespace upm {

/**
 * @brief HTU21D humidity sensor library
 * @defgroup htu21d libupm-htu21
 */

/**
 * @brief C++ API for HTU21D chip (Atmospheric Pressure Sensor)
 *
 * Measurement Specialties [HTU21D]
 * (http://www.meas-spec.com/downloads/HTU21D.pdf)
 * is a digital humidity sensor with temperature output.
 * RH will report between 0 and 100% and temperature range is
 * -40 to +125 degC
 *
 * @ingroup htu21d i2c
 * @snippet htu21d.cxx Interesting
 * @image html htu21d.jpeg
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
        HTU21D (int bus=0, int devAddr=HTU21D_I2C_ADDRESS);

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
         * Get the current measured humidity [RH * 1000]
         * To convert to Relative Humidity, divide by 1000
         *
         */
        int32_t getHumidity(void);

        /**
         * Get the humidity cell temperature [degC * 1000]
         * To convert to Temperature, divide by 1000
         */
        int32_t getTemperature(void);
        
        /**
         * Reads both temperature and humidity together
         * To convert to Relative Humidity, divide by 1000
         * To convert to Temperature, divide by 1000
         *
         * @param pointer to int32_t buffer for temp
         */
        int32_t getRHumidity(int32_t* iTemperature);
        
        /**
         * Using the current humidity and temperature the function
         * will calculate the compensated RH using the equation from
         * the datasheet.
         */
        int32_t getCompRH(void);
        
        /**
         * Function intended to test the device and verify it
         * is correctly operating.
         *
         */
        int testSensor(void);
        
        /**
         * Read 1 to 4 bytes from i2c registers
         *
         * @param reg address of a register
         * @param puint32 pointer to buffer for register data
         * @param ibytes number of bytes to be returned
         */
        mraa_result_t i2cReadRegValue (int reg, uint32_t* puint32, int ibytes);

        /**
         * Read two bytes register
         *
         * @param reg address of a register
         */
        uint16_t i2cReadReg_16 (int reg);

        /**
         * Write to one byte register
         *
         * @param reg address of a register
         * @param value byte to be written
         */
        mraa_result_t i2cWriteReg (uint8_t reg, uint8_t value);

        /**
         * Read one byte register
         *
         * @param reg address of a register
         */
        uint8_t i2cReadReg_8 (int reg);

    private:

        /**
         * Convert temp register to value * 1000
         */
        int32_t htu21_temp_ticks_to_millicelsius(int ticks);

        /**
         * Convert temp register to value * 1000
         */
        int32_t htu21_rh_ticks_to_per_cent_mille(int ticks);

        std::string m_name;

        int m_controlAddr;
        int m_bus;
        mraa_i2c_context m_i2ControlCtx;
        
        int32_t m_temperature;
        int32_t m_humidity;
};

}
