/*
 * Author: William Penner <william.penner@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This application code supports the mpl3115a2 digital barometric pressure
 * and temperature sensor from Freescale.  The datasheet is available
 * from their website:
 * http://cache.freescale.com/files/sensors/doc/data_sheet/MPL3115A2.pdf
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

#include <iostream>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>

#include "mpl3115a2.h"

using namespace upm;

MPL3115A2::MPL3115A2 (int bus, int devAddr, uint8_t mode) : m_i2ControlCtx(bus)
{
    int id;

    m_name = MPL3115A2_NAME;

    m_controlAddr = devAddr;
    m_bus = bus;

    mraa::Result ret = m_i2ControlCtx.address(m_controlAddr);
    if (ret != mraa::SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mraa_i2c_address() failed");
        return;
    }

    setOversampling(mode);

    id = i2cReadReg_8(MPL3115A2_WHO_AM_I);
    if (id != MPL3115A2_DEVICE_ID)  {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": incorrect device id");
        return;
    }
}

/*
 * Function to test the device and verify that is appears operational
 * Typically functioning sensors will return "noisy" values and would
 * be expected to change a bit.  This fuction will check for this
 * variation.
 */

int
MPL3115A2::testSensor(void)
{
    int i, iTries;
    int iError = 0;
    float pressure, temperature;
    float fPMin, fPMax, fTMin, fTMax;

    fprintf(stdout, "Executing Sensor Test.\n" );

    pressure    = getPressure(true);
    temperature = getTemperature(false);
    fPMin = fPMax = pressure;
    fTMin = fTMax = temperature;

    iTries = 20;
    do {
        sampleData();
        pressure = getPressure(true);
        temperature = getTemperature(false);
        if (pressure < fPMin)    fPMin = pressure;
        if (pressure > fPMax)    fPMax = pressure;
        if (temperature < fTMin) fTMin = temperature;
        if (temperature > fTMax) fTMax = temperature;
    }
    while(fPMin == fPMax && fTMin == fTMax && --iTries);

    if (fPMin == fPMax && fTMin == fTMax) {
        fprintf(stdout, "  Warning - sensor values not changing.\n" );
        return -1;
    }

    fprintf(stdout, "  Test complete.\n");

    return 0;
}

/*
 * Function to dump out the i2c register block to the screen
 */

void
MPL3115A2::dumpSensor(void)
{
    int i, j, ival;

    fprintf(stdout, "Dumping i2c block from %s\n", MPL3115A2_NAME);
    for (i=0; i < 256; i+=16) {
        fprintf(stdout, "  %02x: ", i);
        for (j=i; j < i+16; j++) {
            fprintf(stdout, "%02x ", i2cReadReg_8(j));
        }
        fprintf(stdout, "\n");
    }
}

/*
 * Function used to soft RESET the MPL3115A2 device to ensure
 * it is in a known state.  This function can be used to reset
 * the min/max temperature and pressure values.
 */

int
MPL3115A2::resetSensor(void)
{
    fprintf(stdout, "Resetting MPL3115A2 device\n" );
    i2cWriteReg(MPL3115A2_CTRL_REG1, MPL3115A2_CTRL_RESET);
    usleep(50000);
    i2cWriteReg(MPL3115A2_CTRL_REG1, MPL3115A2_CTRL_RESET |
            MPL3115A2_SETOVERSAMPLE(m_oversampling));

    return 0;
}

int
MPL3115A2::sampleData(void)
{
    int val;
    mraa::Result ret;
    int tries = 15;
    uint32_t us_delay;

    // trigger measurement
    ret = i2cWriteReg(MPL3115A2_CTRL_REG1,
            MPL3115A2_CTRL_OST | MPL3115A2_SETOVERSAMPLE(m_oversampling));
    if (mraa::SUCCESS != ret) {
        fprintf(stdout, "Write to trigger measurement failed\n");
        return -1;
    }

    // Calculate and delay the appopriate time for the measurement
    us_delay = ((1 << m_oversampling) * 4 + 2) * 1000;
    usleep(us_delay);

    // Loop waiting for the ready bit to become active
    while (tries-- > 0) {
        val = i2cReadReg_8(MPL3115A2_CTRL_REG1);

        /* wait for data ready, i.e. OST cleared */
        if (!(val & MPL3115A2_CTRL_OST))
            break;
        usleep(20000);
    }
    if (tries < 0) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": timeout during measurement");
        return -1;
    }

    return 0;
}

int32_t
MPL3115A2::getPressureReg(int reg) {
    return ((i2cReadReg_16(reg) << 8)|(uint32_t)i2cReadReg_8(reg+2))*100/64;
}

int32_t
MPL3115A2::getTempReg(int reg) {
    return (int32_t)((int16_t)i2cReadReg_16(reg)) * 1000 / 256;
}

float
MPL3115A2::getPressure(int bSampleData) {
    int ret;

    // Trigger request to make a measurement
    if (bSampleData) {
        ret = sampleData();
        if (ret < 0) {
            fprintf(stdout, "Error sampling pressure\n");
            return -1;
        }
    }
    m_iPressure = getPressureReg(MPL3115A2_OUT_PRESS);

    return (float)m_iPressure / 100;
}

float
MPL3115A2::getTemperature(int bSampleData) {
    int ret;

    // Trigger request to make a measurement
    if (bSampleData) {
        ret = sampleData();
        if (ret < 0) {
            fprintf(stdout, "Error sampling temperature\n");
            return -1;
        }
	}
    m_iTemperature = getTempReg(MPL3115A2_OUT_TEMP);

    return (float)m_iTemperature / 1000;
}

float
MPL3115A2::getSealevelPressure(float altitudeMeters) {
    float fPressure = (float)m_iPressure / 100.0;
    return fPressure / pow(1.0-altitudeMeters/44330, 5.255);
}

float
MPL3115A2::getAltitude (float sealevelPressure) {
    float fPressure = (float)m_iPressure / 100.0;
    return 44330 * (1.0 - pow(fPressure /sealevelPressure,0.1903));
}

void
MPL3115A2::setOversampling(uint8_t oversampling)
{
    if (oversampling > MPL3115A2_MAXOVERSAMPLE)
        oversampling = MPL3115A2_MAXOVERSAMPLE;
    m_oversampling = oversampling;
}

uint8_t
MPL3115A2::getOversampling(void)
{
    return m_oversampling;
}

float
MPL3115A2::getTemperatureMax(void)
{
    return (float)getTempReg(MPL3115A2_T_MAX) / 1000;
}

float
MPL3115A2::getTemperatureMin(void)
{
    return (float)getTempReg(MPL3115A2_T_MIN) / 1000;
}

float
MPL3115A2::getPressureMax(void)
{
    return (float)getPressureReg(MPL3115A2_P_MAX) / 1000;
}

float
MPL3115A2::getPressureMin(void)
{
    return (float)getPressureReg(MPL3115A2_P_MIN) / 1000;
}

float
MPL3115A2::convertTempCtoF(float fTemp)
{
    return(fTemp * 9 / 5 + 32);
}

/*
 * This is set for 15degC (Pa = 0.0002961 in Hg)
 */
float
MPL3115A2::convertPaToinHg(float fPressure)
{
    return(fPressure * 0.0002961);
}

/*
 * Functions to read and write data to the i2c device
 */

mraa::Result
MPL3115A2::i2cWriteReg (uint8_t reg, uint8_t value) {
    mraa::Result error = mraa::SUCCESS;

    uint8_t data[2] = { reg, value };
    m_i2ControlCtx.address (m_controlAddr);
    error = m_i2ControlCtx.write (data, 2);

    if (error != mraa::SUCCESS)
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ":mraa_i2c_write() failed");
    return error;
}

uint16_t
MPL3115A2::i2cReadReg_16 (int reg) {
    uint16_t data;

    m_i2ControlCtx.address(m_controlAddr);
    data  = (uint16_t)m_i2ControlCtx.readReg(reg) << 8;
    data |= (uint16_t)m_i2ControlCtx.readReg(reg+1);

    return data;
}

uint8_t
MPL3115A2::i2cReadReg_8 (int reg) {
    m_i2ControlCtx.address(m_controlAddr);
    return m_i2ControlCtx.readReg(reg);
}

