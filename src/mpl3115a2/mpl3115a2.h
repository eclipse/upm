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

#define MPL3115A2_NAME        "mpl3115a2"

#define MPL3115A2_I2C_ADDRESS   0x60
#define MPL3115A2_DEVICE_ID     0xc4

#define MPL3115A2_STATUS        0x00
#define MPL3115A2_OUT_PRESS     0x01  /* MSB first, 20 bit */
#define MPL3115A2_OUT_TEMP      0x04  /* MSB first, 12 bit */
#define MPL3115A2_WHO_AM_I      0x0c
#define MPL3115A2_PT_DATA_CFG   0x13
#define MPL3115A2_P_MIN         0x1C
#define MPL3115A2_T_MIN         0x1F
#define MPL3115A2_P_MAX         0x21
#define MPL3115A2_T_MAX         0x24
#define MPL3115A2_CTRL_REG1     0x26

// CTRL_REG1
#define MPL3115A2_CTRL_SBYB     0x01  /* Standby (not) */
#define MPL3115A2_CTRL_OST      0x02  /* One-shot trigger */
#define MPL3115A2_CTRL_RESET    0x04  /* RESET device */
#define MPL3115A2_CTRL_ALT_MODE 0x80  /* Altitude mode */

#define MPL3115A2_SETOVERSAMPLE(a) ((a & 7) << 3)
#define MPL3115A2_GETOVERSAMPLE(a) ((a >> 3) & 7)
#define MPL3115A2_MAXOVERSAMPLE   7

namespace upm {

/**
 * @brief mpl3115a2 Atmospheric Pressure library
 * @defgroup mpl3115a2 libupm-mpl3115a2
 * @ingroup freescale i2c pressure
 */
/**
 * @library libupm-mpl3115a2
 * @sensor mpl3115a2
 * @comname mpl3115a2 Atmospheric Pressure
 * @type pressure
 * @man freescale
 * @web http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=MPL3115A2
 * @con i2c
 *
 * @brief C++ API for MPL3115A2 chip (Atmospheric Pressure Sensor)
 *
 * Freescale [MPL3115A2]
 * (http://cache.freescale.com/files/sensors/doc/data_sheet/MPL3115A2.pdf)
 * is a high precision, ultra-low power consumption pressure sensor. It has a
 * range of between 50 and 110 kPa.
 *
 * @snippet mpl3115a2.cxx Interesting
 */
class MPL3115A2 {
    public:
        /**
         * Instantiates an MPL3115A2 object
         *
         * @param bus number of used bus
         * @param devAddr address of used i2c device
         * @param mode MPL3115A2 oversampling (6 = 64x)
         */
        MPL3115A2(int bus, int devAddr=MPL3115A2_I2C_ADDRESS, uint8_t mode=6);

        /**
         * MPL3115A2 object destructor, basicaly it close i2c connection.
         */
        ~MPL3115A2();

        /**
         * Test the sensor and try to determine if operating by looking
         * for small variations in the value
         */
        int testSensor(void);

        /**
         * Perform a soft RESET of the MPL3115A2 device to ensure
         * it is in a known state.  This function can be used to reset
         * the min/max temperature and pressure values.
         */
        int resetSensor(void);

        /**
         * Dump out the i2c register block to stdout
         */
        void dumpSensor(void);

        /**
         * Initiate a temp/pressure mesasurement and wait for function
         * to complete.  The temp and pressure registers can be read
         * after this call.
         */
        int sampleData(void);

        /**
         * Read a pressure value from the mpl3115a2 [Pa * 100]
         *
         * @param reg base address of pressure register
         */
        int32_t getPressureReg(int reg);

        /**
         * Read a temperature value from the mpl3115a2 [degC * 1000]
         *
         * @param reg base address of temperature register
         */
        int32_t getTempReg(int reg);

        /**
         * Read the current pressure value from the mpl3115a2 [Pa]
         *
         * @param bSampleData Set non-zero to sample reading
         */
        float getPressure(int bSampleData = true);

        /**
         * Read the current temperature value from the mpl3115a2 [degC]
         *
         * @param bSampleData Set non-zero to sample reading
         */
        float getTemperature(int bSampleData = true);

        /**
         * Read the current pressure and using a known altitude calculate
         * the sea level pressure value [Pa]
         * This function should be preceeded by the sampleData() call
         *
         * @param altitudeMeters Altitude in meters
         */
        float getSealevelPressure(float altitudeMeters = 0.0);

        /**
         * Read the current pressure and using a known sea level pressure
         * calculate the altitude value [m]
         * This function should be preceeded by the sampleData() call
         *
         * @param sealevelPressure Current sea level pressure
         */
        float getAltitude (float sealevelPressure = 101325.0);

        /**
         * Set the oversampling setting (ranges from 0 to 7).  The
         * value represents 2^n samples (ranging from 1 to 128).  The
         * time to calculate the sample is approximately (2^n * 4 + 2) mS
         *
         * @param oversampling New oversampling value
         */
        void setOversampling(uint8_t oversampling);

        /**
         * Returns the current oversampling value
         */
        uint8_t getOversampling(void);

        /**
         * Read the maximum measured temperature [degC]
         */
        float getTemperatureMax(void);

        /**
         * Read the minimum measured temperature [degC]
         */
        float getTemperatureMin(void);

        /**
         * Read the maximum measured pressure [Pa]
         */
        float getPressureMax (void);

        /**
         * Read the minimum measured pressure [Pa]
         */
        float getPressureMin (void);

        /**
         * Convert temperature from degC*1000 to degF*1000
         *
         * @param iTemp Temperature in degC
         */
        float convertTempCtoF(float fTemp);

        /**
         * Convert pressure from Pa*100 to inHg*10000
         * This is set for 15degC (Pa = 0.0002961 in Hg)
         * TODO: Change function to add temp calibration
         *
         * @param iPressure Pressure in Pa
         */
        float convertPaToinHg(float fPressure);

        /**
         * Write to one byte to i2c register
         *
         * @param reg address of a register
         * @param value byte to be written
         */
        mraa_result_t i2cWriteReg (uint8_t reg, uint8_t value);

        /**
         * Read two bytes from i2c registers
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
        std::string m_name;

        int m_controlAddr;
        int m_bus;
        mraa_i2c_context m_i2ControlCtx;

        uint8_t m_oversampling;
        int32_t m_iPressure;
        int32_t m_iTemperature;
};

}

