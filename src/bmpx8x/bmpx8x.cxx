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

#include <iostream>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>

#include "bmpx8x.h"

using namespace upm;

BMPX8X::BMPX8X (int bus, int devAddr, uint8_t mode) : m_controlAddr(devAddr), m_i2ControlCtx(bus) {
 
    m_name = "BMPX8X";
 
    mraa::Result ret = m_i2ControlCtx.address(m_controlAddr);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_address() failed");
        return;
    }

    if (i2cReadReg_8 (0xD0) != 0x55)  {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": Invalid chip ID");
        return;
    }

    if (mode > BMP085_ULTRAHIGHRES) {
        mode = BMP085_ULTRAHIGHRES;
    }
    oversampling = mode;

    /* read calibration data */
    ac1 = i2cReadReg_16 (BMP085_CAL_AC1);
    ac2 = i2cReadReg_16 (BMP085_CAL_AC2);
    ac3 = i2cReadReg_16 (BMP085_CAL_AC3);
    ac4 = i2cReadReg_16 (BMP085_CAL_AC4);
    ac5 = i2cReadReg_16 (BMP085_CAL_AC5);
    ac6 = i2cReadReg_16 (BMP085_CAL_AC6);

    b1 = i2cReadReg_16 (BMP085_CAL_B1);
    b2 = i2cReadReg_16 (BMP085_CAL_B2);

    mb = i2cReadReg_16 (BMP085_CAL_MB);
    mc = i2cReadReg_16 (BMP085_CAL_MC);
    md = i2cReadReg_16 (BMP085_CAL_MD);
}

int32_t
BMPX8X::getPressure () {
    int32_t UT, UP, B3, B5, B6, X1, X2, X3, p;
    uint32_t B4, B7;

    UT = getTemperatureRaw();
    UP = getPressureRaw();
    B5 = computeB5(UT);

    // do pressure calcs
    B6 = B5 - 4000;
    X1 = ((int32_t)b2 * ( (B6 * B6)>>12 )) >> 11;
    X2 = ((int32_t)ac2 * B6) >> 11;
    X3 = X1 + X2;
    B3 = ((((int32_t)ac1*4 + X3) << oversampling) + 2) / 4;

    X1 = ((int32_t)ac3 * B6) >> 13;
    X2 = ((int32_t)b1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = ((uint32_t)ac4 * (uint32_t)(X3 + 32768)) >> 15;
    B7 = ((uint32_t)UP - B3) * (uint32_t)( 50000UL >> oversampling );

    if (B7 < 0x80000000) {
        p = (B7 * 2) / B4;
    } else {
        p = (B7 / B4) * 2;
    }
    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;

    p = p + ((X1 + X2 + (int32_t)3791)>>4);

    return p;
}

int32_t
BMPX8X::getPressureRaw () {
    uint32_t raw;

    i2cWriteReg (BMP085_CONTROL, BMP085_READPRESSURECMD + (oversampling << 6));

    if (oversampling == BMP085_ULTRALOWPOWER) {
        usleep(5000);
    } else if (oversampling == BMP085_STANDARD) {
        usleep(8000);
    } else if (oversampling == BMP085_HIGHRES) {
        usleep(14000);
    } else {
        usleep(26000);
    }

    raw = i2cReadReg_16 (BMP085_PRESSUREDATA);

    raw <<= 8;
    raw |= i2cReadReg_8 (BMP085_PRESSUREDATA + 2);
    raw >>= (8 - oversampling);

    return raw;
}

int16_t
BMPX8X::getTemperatureRaw () {
    i2cWriteReg (BMP085_CONTROL, BMP085_READTEMPCMD);
    usleep(5000);
    return i2cReadReg_16 (BMP085_TEMPDATA);
}

float
BMPX8X::getTemperature () {
    int32_t UT, B5;     // following ds convention
    float temp;

    UT = getTemperatureRaw ();

    B5 = computeB5 (UT);
    temp = (B5 + 8) >> 4;
    temp /= 10;

    return temp;
}

int32_t
BMPX8X::getSealevelPressure(float altitudeMeters) {
    float pressure = getPressure ();
    return (int32_t)(pressure / pow(1.0-altitudeMeters/44330, 5.255));
}

float
BMPX8X::getAltitude (float sealevelPressure) {
    float altitude;

    float pressure = getPressure ();

    altitude = 44330 * (1.0 - pow(pressure /sealevelPressure,0.1903));

    return altitude;
}

int32_t
BMPX8X::computeB5(int32_t UT) {
    int32_t X1 = (UT - (int32_t)ac6) * ((int32_t)ac5) >> 15;
    int32_t X2 = ((int32_t)mc << 11) / (X1+(int32_t)md);

    return X1 + X2;
}

mraa::Result
BMPX8X::i2cWriteReg (uint8_t reg, uint8_t value) {
    mraa::Result error = mraa::SUCCESS;

    uint8_t data[2] = { reg, value };
    error = m_i2ControlCtx.address (m_controlAddr);
    error = m_i2ControlCtx.write (data, 2);

    return error;
}

uint16_t
BMPX8X::i2cReadReg_16 (int reg) {
    uint16_t data;

    m_i2ControlCtx.address(m_controlAddr);
    m_i2ControlCtx.writeByte(reg);

    m_i2ControlCtx.address(m_controlAddr);
    m_i2ControlCtx.read((uint8_t *)&data, 0x2);

    uint8_t high = (data & 0xFF00) >> 8;
    data = (data << 8) & 0xFF00;
    data |= high;

    return data;
}

uint8_t
BMPX8X::i2cReadReg_8 (int reg) {
    uint8_t data;

    m_i2ControlCtx.address(m_controlAddr);
    m_i2ControlCtx.writeByte(reg);

    m_i2ControlCtx.address(m_controlAddr);
    m_i2ControlCtx.read(&data, 0x1);

    return data;
}
