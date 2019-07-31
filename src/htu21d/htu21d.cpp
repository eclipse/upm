/*
 * Author: William Penner <william.penner@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>
#include <stdexcept>

#include <unistd.h>
#include <stdlib.h>
#include <endian.h>

#include "htu21d.hpp"

using namespace upm;

HTU21D::HTU21D(int bus, int devAddr) : m_i2ControlCtx(bus) {
    m_temperature = 0;
    m_humidity    = 0;

    m_name = HTU21D_NAME;

    m_controlAddr = devAddr;
    m_bus = bus;

    mraa::Result ret = m_i2ControlCtx.address(m_controlAddr);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_address() failed");
    }
    resetSensor();
}

void
HTU21D::resetSensor(void)
{
    uint8_t data = HTU21D_SOFT_RESET;
    m_i2ControlCtx.write (&data, 1);
    usleep(20000);
}

/*
 * Convert register value to degC * 1000
 */
int32_t
HTU21D::convertTemp(int32_t regval)
{
    return ((21965 * (regval & 0xFFFC)) >> 13) - 46850;
}

/*
 * Convert register value to %RH * 1000
 */
int32_t
HTU21D::convertRH(int32_t regval)
{
    return ((15625 * (regval & 0xFFFC)) >> 13) - 6000;
}

int
HTU21D::sampleData(void)
{
    uint32_t itemp;

    itemp = be16toh(i2cReadReg_16(HTU21D_READ_TEMP_HOLD));
    m_temperature = convertTemp(itemp);

    itemp = be16toh(i2cReadReg_16(HTU21D_READ_HUMIDITY_HOLD));
    m_humidity = convertRH(itemp);

    return itemp == 0xFFFF;
}

float
HTU21D::getTemperature(int bSampleData)
{
    if (bSampleData) {
        sampleData();
    }
    return (float)m_temperature / 1000;
}

float
HTU21D::getTemperature()
{
    return getTemperature(0);
}

float
HTU21D::getHumidity(int bSampleData)
{
    if (bSampleData) {
        sampleData();
    }
    return (float)m_humidity / 1000;
}

float
HTU21D::getHumidity()
{
    return getHumidity(0);
}

/*
 * Use the compensation equation from the datasheet to correct the
 * current reading
 * RHcomp = RHactualT + (25 - Tactual) * CoeffTemp
 * RHcomp is in units of %RH * 1000
 */
float
HTU21D::getCompRH(int bSampleData)
{
	if (bSampleData) {
		sampleData();
	}
	return (float)(m_humidity + (25000 - m_temperature) * 3 / 20) / 1000;
}

int
HTU21D::setHeater(int bEnable)
{
	uint8_t userreg;

	userreg = i2cReadReg_8(HTU21D_READ_USER_REG);
	if (bEnable)
		userreg |= HTU21D_HEATER_ENABLE;
	else
		userreg &= ~HTU21D_HEATER_ENABLE;
	if (i2cWriteReg(HTU21D_WRITE_USER_REG, userreg) < 0)
		return -1;

	return 0;
}

/*
 * Use the equation from the datasheet to calculate the partial pressure
 * and then calculate the dew point temperature in degree C.  Typical
 * use would be after reading the temp and humidity the partial
 * pressure can be caculated using the previously read values.
 */

float
HTU21D::getDewPoint(int bSampleData)
{
    if (bSampleData) {
        sampleData();
    }

    float fA  = 8.1332;
    float fB  = 1762.39;
    float fC  = 235.66;
    float fT  = getTemperature(false);
    float fRH = getHumidity(false);
    float fPP = powf(10, fA - (fB / (fT + fC)));
    float fDP = -(fB / (log10(fRH * fPP / 100) - fA) + fC);

    return (fDP);
}

/*
 * Function to optimize reading of values from the device.
 * This function will always initiate a read from the sensor
 * and return the values and dew point calculated value
 */

int
HTU21D::getHumidityData(float* pfHum, float* pfHumTemp, float* pfDewPt)
{
    getHumidity(true);
    float fTemp  = getTemperature(false);
    float fDewPt = getDewPoint(false);
    float fCHum  = getCompRH(false);

    if (pfHum)
        *pfHum     = fCHum;
    if (pfHumTemp)
        *pfHumTemp = fTemp;
    if (pfDewPt)
        *pfDewPt   = fDewPt;

    return 0;
}

/*
 * Test function: when reading the HTU21D many times rapidly should
 * result in a temperature increase.  This test will verify that the
 * value is changing from read to read
 */

int
HTU21D::testSensor(void)
{
    int i;
    int iError = 0;
    float fTemp, fHum;
    float fTempMax, fTempMin;
    float fHumMax = 0.0, fHumMin = 0.0;
    float fTempFirst;

    fprintf(stdout, "Executing Sensor Test\n" );

    fHum  = getHumidity(true);
    fTemp = getTemperature(false);
    fTempFirst = fTempMax = fTempMin = fTemp;

    // Turn on the heater to make a sensor change
    setHeater(true);

    // Then sample the sensor a few times
    for (i=0; i < 10; i++) {
        fHum  = getHumidity(true);
        fTemp = getTemperature(false);
        if (fHum  < fHumMin)  fHumMin  = fHum;
        if (fHum  > fHumMax)  fHumMax  = fHum;
        if (fTemp < fTempMin) fTempMin = fTemp;
        if (fTemp > fTempMax) fTempMax = fTemp;
        usleep(50000);
    }

    // Turn off the heater
    setHeater(false);

    // Now check the results
    if ((fTemp - fTempFirst) <= 0) {
        fprintf(stdout, "  Temperature should have increased, but didn't\n" );
        iError++;
    }
    if (fHumMin == fHumMax) {
        fprintf(stdout, "  Humidity reading was unchanged - warning\n" );
        iError++;
    }
    if (fTempMin == fTempMax) {
        fprintf(stdout, "  Temperature reading was unchanged - warning\n" );
        iError++;
    }
    if (iError == 0) {
        fprintf(stdout, "  Device appears functional\n" );
    }

    fprintf(stdout, "  Test complete\n" );

    return iError;
}

/*
 * Functions to read and write data to the i2c device
 */

mraa::Result
HTU21D::i2cWriteReg (uint8_t reg, uint8_t value) {
    mraa::Result error = mraa::SUCCESS;

    uint8_t data[2] = { reg, value };
    error = m_i2ControlCtx.write (data, 2);
    if ( error != mraa::SUCCESS)
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_i2c_write() failed");

    return error;
}

uint16_t
HTU21D::i2cReadReg_16 (int reg) {
    return m_i2ControlCtx.readWordReg(reg);
}

uint8_t
HTU21D::i2cReadReg_8 (int reg) {
    return m_i2ControlCtx.readReg(reg);
}

