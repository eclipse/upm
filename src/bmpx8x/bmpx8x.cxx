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
#include <unistd.h>
#include <stdlib.h>

#include "bmpx8x.h"

using namespace upm;

BMPX8X::BMPX8X (int bus, int devAddr, uint8_t mode) {
    m_name = "BMPX8X";

    m_controlAddr = devAddr;
    m_bus = bus;
    configured = false;

    m_i2ControlCtx = mraa_i2c_init(m_bus);

    mraa_result_t status = mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    if (status != MRAA_SUCCESS) {
        configured = false;
        fprintf(stderr, "BMPX8X: I2C bus failed to initialise.\n");
        return;
    }

    // Set mode
    if (mode > BMPX8X_ULTRAHIGHRES) {
        mode = BMPX8X_ULTRAHIGHRES;
    }
    oversampling = mode;

    // Check this is a BMPX8X chip
    if(!isAvailable()) {
        configured = false;
        return;
    }

    // Get calibration data
    if(!getCalibrationData()) {
        configured = false;
        return;
    }

    configured = true;
}

BMPX8X::~BMPX8X() {
    mraa_i2c_stop(m_i2ControlCtx);
}

mraa_result_t
BMPX8X::getPressure (int32_t *value) {
    int32_t UT, UP, B3, B5, B6, X1, X2, X3, p;
    uint32_t B4, B7;

    UT = getTemperatureRaw();
    UP = getPressureRaw();

    if(UT == -1 || UP == -1) { return MRAA_ERROR_INVALID_RESOURCE; }

    // Temperature
    X1 = (UT - ac6) * ac5 / 2^15;
    X2 = mc * 2^11 / (X1 + md);
    B5 = X1 + X2;

    // Pressure
    B6 = B5 - 4000;
    X1 = (b2 * ( (B6 * B6) / 2^12 )) / 2^11;
    X2 = (ac2 * B6) / 2^11;
    X3 = X1 + X2;
    B3 = (((ac1 * 4 + X3) << oversampling) + 2) / 4;

    X1 = (ac3 * B6) / 2^13;
    X2 = (b1 * ((B6 * B6) / 2^12)) / 2^16;
    X3 = ((X1 + X2) + 2) / 2^2;
    B4 = (ac4 * (uint32_t)(X3 + 32768)) / 2^15;
    B7 = ((uint32_t)UP - B3) * ( 50000 >> oversampling );

    if (B7 < 0x80000000) {
        p = (B7 * 2) / B4;
    } else {
        p = (B7 / B4) * 2;
    }

    X1 = (p / 2^8) * (p / 2^8);
    X1 = (X1 * 3038) / 2^16;
    X2 = (-7357 * p) / 2^16;

    *value = (int32_t)(p + (X1 + X2 + 3791) / 2^4);

    return MRAA_SUCCESS;
}

int32_t
BMPX8X::getPressureRaw () {
    int8_t msb, lsb, xlsb;

    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    if(mraa_i2c_write_byte_data(m_i2ControlCtx, BMPX8X_READPRESSURECMD + (oversampling << 6), BMPX8X_CONTROL) != MRAA_SUCCESS) { return -1; }

    if (oversampling == BMPX8X_ULTRALOWPOWER) {
        usleep(5000);
    } else if (oversampling == BMPX8X_STANDARD) {
        usleep(8000);
    } else if (oversampling == BMPX8X_HIGHRES) {
        usleep(14000);
    } else {
        usleep(26000);
    }

    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    msb = mraa_i2c_read_byte_data(m_i2ControlCtx, BMPX8X_DATA_MSB);

    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    lsb = mraa_i2c_read_byte_data(m_i2ControlCtx, BMPX8X_DATA_LSB);

    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    xlsb = mraa_i2c_read_byte_data(m_i2ControlCtx, BMPX8X_DATA_XLSB);

    if(msb == -1 || lsb == -1) { return -1; }

    return ((msb << 16) | (lsb << 8) | xlsb) >> (8 - oversampling);
}

int32_t
BMPX8X::getTemperatureRaw () {
    int8_t msb, lsb;
    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    mraa_i2c_write_byte_data(m_i2ControlCtx, BMPX8X_READTEMPCMD, BMPX8X_CONTROL);
    usleep(5000);
    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    msb = mraa_i2c_read_byte_data(m_i2ControlCtx, BMPX8X_DATA_MSB);
    lsb = mraa_i2c_read_byte_data(m_i2ControlCtx, BMPX8X_DATA_LSB);

    if(msb == -1 || lsb == -1) { return -1; }

    return (msb << 8) | lsb;
}

mraa_result_t
BMPX8X::getTemperature (int32_t *value) {
    int32_t UT, X1, X2, B5;

    UT = getTemperatureRaw ();

    if(UT == -1) { return MRAA_ERROR_INVALID_RESOURCE; }

    X1 = (UT - ac6) * ac5 / 2^15;
    X2 = mc * 2^11 / (X1 + md);
    B5 = X1 + X2;
    *value = (int32_t)((B5 + 8) / 2^4);

    return MRAA_SUCCESS;
}

int32_t
BMPX8X::getSealevelPressure(int32_t pressure, int32_t altitudeMeters) {
    return (int32_t)(pressure / pow((1.0 - altitudeMeters) / 44330, 5.255));
}

int32_t
BMPX8X::getAltitude (int32_t pressure, int32_t sealevelPressure) {
    return (int32_t)(44330 * (1.0 - pow(pressure / sealevelPressure, 0.1903)));
}

bool
BMPX8X::getCalibrationData() {
    /* Read calibration data */
    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    ac1 = mraa_i2c_read_word_data (m_i2ControlCtx, BMPX8X_CAL_AC1);
    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    ac2 = mraa_i2c_read_word_data (m_i2ControlCtx, BMPX8X_CAL_AC2);
    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    ac3 = mraa_i2c_read_word_data (m_i2ControlCtx, BMPX8X_CAL_AC3);
    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    ac4 = mraa_i2c_read_word_data (m_i2ControlCtx, BMPX8X_CAL_AC4);
    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    ac5 = mraa_i2c_read_word_data (m_i2ControlCtx, BMPX8X_CAL_AC5);
    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    ac6 = mraa_i2c_read_word_data (m_i2ControlCtx, BMPX8X_CAL_AC6);

    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    b1 = mraa_i2c_read_word_data (m_i2ControlCtx, BMPX8X_CAL_B1);
    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    b2 = mraa_i2c_read_word_data (m_i2ControlCtx, BMPX8X_CAL_B2);

    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    mb = mraa_i2c_read_word_data (m_i2ControlCtx, BMPX8X_CAL_MB);
    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    mc = mraa_i2c_read_word_data (m_i2ControlCtx, BMPX8X_CAL_MC);
    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    md = mraa_i2c_read_word_data (m_i2ControlCtx, BMPX8X_CAL_MD);

    if(ac1 == -1 || ac2 == -1 || ac3 == -1 || ac4 == -1 || ac5 == -1 || ac6 == -1 || b1 == -1 || b2 == -1 || mb == -1 || mc == -1 || md == -1) { return false; }

    return true;
}

bool
BMPX8X::isAvailable() {
    mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    if (mraa_i2c_read_byte_data(m_i2ControlCtx, BMPX8X_CHIP_ID) != BMPX8X_ID)  {
        return false;
    }

    return true;
}

bool
BMPX8X::isConfigured() {
    return configured;
}
