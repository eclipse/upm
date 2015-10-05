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

#include <iostream>
#include <string>
#include <stdexcept>

#include <unistd.h>
#include <stdlib.h>

#include "htu21d.h"

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
    uint8_t data;
    m_i2ControlCtx.address (m_controlAddr);
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
HTU21D::convertRH(int regval)
{
    return ((15625 * (regval & 0xFFFC)) >> 13) - 6000;
}

int
HTU21D::sampleData(void)
{
    uint32_t itemp;

    itemp = i2cReadReg_16(HTU21D_READ_TEMP_HOLD);
    m_temperature = convertTemp(itemp);

    itemp = i2cReadReg_16(HTU21D_READ_HUMIDITY_HOLD);
    m_humidity = convertRH(itemp);

    return 0;
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
HTU21D::getHumidity(int bSampleData)
{
    if (bSampleData) {
        sampleData();
    }
    return (float)m_humidity / 1000;
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
    float fHumMax, fHumMin;
    float fHumFirst, fTempFirst;

    fprintf(stdout, "Executing Sensor Test\n" );

    fHum  = getHumidity(true);
    fTemp = getTemperature(false);
    fTempFirst = fTempMax = fTempMin = fTemp;
    fHumFirst  = fHumMax  = fHumMin  = fHum;

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
    m_i2ControlCtx.address (m_controlAddr);
    error = m_i2ControlCtx.write (data, 2);
    if ( error != mraa::SUCCESS)
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_i2c_write() failed");

    return error;
}

uint16_t
HTU21D::i2cReadReg_16 (int reg) {
    uint16_t data;
    m_i2ControlCtx.address(m_controlAddr);
    data  = (uint16_t)m_i2ControlCtx.readReg(reg) << 8;
    data |= (uint16_t)m_i2ControlCtx.readReg(reg+1);
    return data;
}

uint8_t
HTU21D::i2cReadReg_8 (int reg) {
    m_i2ControlCtx.address(m_controlAddr);
    return m_i2ControlCtx.readReg(reg);
}

