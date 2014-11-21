/*
 * Author: William Penner <william.penner@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This driver supports the HTU21D digital humidity and temperature
 * sensor. The datasheet is available from:
 * http://www.meas-spec.com/downloads/HTU21D.pdf
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

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "htu21d.h"

using namespace upm;

HTU21D::HTU21D(int bus, int devAddr) {
    m_temperature = 0;
    m_humidity    = 0;

    m_name = HTU21D_NAME;

    m_controlAddr = devAddr;
    m_bus = bus;

    m_i2ControlCtx = mraa_i2c_init(m_bus);

    mraa_result_t ret = mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    if (ret != MRAA_SUCCESS) {
        fprintf(stderr, "Error accessing i2c bus\n");
    }
}

HTU21D::~HTU21D() {
    mraa_i2c_stop(m_i2ControlCtx);
}

int32_t
HTU21D::htu21_temp_ticks_to_millicelsius(int ticks)
{
    ticks &= ~0x0003; /* clear status bits */
    /*
     * Formula T = -46.85 + 175.72 * ST / 2^16 from datasheet p14,
     * optimized for integer fixed point (3 digits) arithmetic
     */
    return ((21965 * (int32_t)ticks) >> 13) - 46850;
}

int32_t
HTU21D::htu21_rh_ticks_to_per_cent_mille(int ticks)
{
    ticks &= ~0x0003; /* clear status bits */
    /*
     * Formula RH = -6 + 125 * SRH / 2^16 from datasheet p14,
     * optimized for integer fixed point (3 digits) arithmetic
     */
    return ((15625 * (int32_t)ticks) >> 13) - 6000;
}

int
HTU21D::sampleData(void)
{
    uint32_t itemp;

    itemp = i2cReadReg_16(HTU21D_T_MEASUREMENT_HM);
    m_temperature = htu21_temp_ticks_to_millicelsius(itemp);

    itemp = i2cReadReg_16(HTU21D_RH_MEASUREMENT_HM);
    m_humidity = htu21_rh_ticks_to_per_cent_mille(itemp);

    return 0;
}

int32_t
HTU21D::getTemperature(void)
{
    return m_temperature;
}

int32_t
HTU21D::getHumidity(void)
{
    return m_humidity;
}

/*
 * This is the primary function to read the data.  It will initiate
 * a measurement cycle and will then return both the humidity and
 * temperature values.  piTemperature can be NULL.
 */

int32_t
HTU21D::getRHumidity(int32_t* piTemperature)
{
    sampleData();
    if (NULL != piTemperature)
        *piTemperature = m_temperature;
    return m_humidity;
}

/*
 * Use the compensation equation from the datasheet to correct the
 * current reading
 * RHcomp = RHactualT + (25 - Tactual) * CoeffTemp
 * RHcomp is in units of %RH * 1000
 */
int32_t
HTU21D::getCompRH(void)
{
    return m_humidity + (25000 - m_temperature) * 3 / 20;
}


/*
 * Test function: when reading the HTU21D many times rapidly should
 * result in a temperature increase.  This test will verify that the
 * value is changing from read to read
 */

int
HTU21D::testSensor(void)
{
    int iError = 0;
    int i, j;
    int32_t iTemp, iHum;
    int32_t iTempMax, iTempMin;
    int32_t iHumMax, iHumMin;
    int32_t iHumFirst, iTempFirst;

    fprintf(stdout, "Executing Sensor Test.\n  Reading registers 100 times to look for operation\n" );

    iHum = getRHumidity(&iTemp);
    iTempFirst = iTempMax = iTempMin = iTemp;
    iHumFirst  = iHumMax  = iHumMin  = iHum;

    for (i=0; i < 100; i++) {
        iHum = getRHumidity(&iTemp);
        if (iHum  < iHumMin)  iHumMin  = iHum;
        if (iHum  > iHumMax)  iHumMax  = iHum;
        if (iTemp < iTempMin) iTempMin = iTemp;
        if (iTemp > iTempMax) iTempMax = iTemp;
//        fprintf(stdout, "Temp: %d  Humidity: %d\n", iTemp, iHum);
    }

    if ((iTemp - iTempFirst) <= 0) {
        fprintf(stdout, "! Temperature should have increased, but didn't\n" );
        iError++;
    }

    if (iHumMin == iHumMax) {
        fprintf(stdout, "! Humidity was unchanged - not working?\n" );
        iError++;
    }

    if (iTempMin == iTempMax) {
        fprintf(stdout, "! Temperature was unchanged - not working?\n" );
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

mraa_result_t
HTU21D::i2cReadRegValue(int reg, uint32_t* puint32, int ibytes) {
    uint32_t data = 0;

    if (ibytes > 4)
        ibytes = 4;

    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    mraa_i2c_write_byte(m_i2ControlCtx, reg);

    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    mraa_i2c_read(m_i2ControlCtx, (uint8_t *)&data, ibytes);

    fprintf(stdout, "reg data = %08x\n", data);
    *puint32 = be32toh(data) >> ((4-ibytes) * 8);
    fprintf(stdout, "reg return = %08x\n", *puint32);

    return MRAA_SUCCESS;
}

mraa_result_t
HTU21D::i2cWriteReg (uint8_t reg, uint8_t value) {
    mraa_result_t error = MRAA_SUCCESS;

    uint8_t data[2] = { reg, value };
    error = mraa_i2c_address (m_i2ControlCtx, m_controlAddr);
    error = mraa_i2c_write (m_i2ControlCtx, data, 2);

    return error;
}

/*
 * Function to read 16 bits starting at reg.  This function
 * was replaced due to functionality of using read() to
 * access i2c data.
 */
uint16_t
HTU21D::i2cReadReg_16 (int reg) {
    uint16_t data;
    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    data  = (uint16_t)mraa_i2c_read_byte_data(m_i2ControlCtx, reg) << 8;
    data |= (uint16_t)mraa_i2c_read_byte_data(m_i2ControlCtx, reg+1);
    return data;
}

/*
 * Function to read 8 bits starting at reg.  This function
 * was replaced due to functionality of using read() to
 * access i2c data.
 */
uint8_t
HTU21D::i2cReadReg_8 (int reg) {
    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    return mraa_i2c_read_byte_data(m_i2ControlCtx, reg);
}

