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

#include "bmp180.hpp"

/* CALIBRATION */
#define BMP180_PROM_START_ADDR          (0xAA)
#define BMP180_PROM_DATA_LEN            (22)

#define BMP180_CALIB_DATA_SIZE          (22)
#define BMP180_CALIB_PARAM_AC1_MSB      (0)
#define BMP180_CALIB_PARAM_AC1_LSB      (1)
#define BMP180_CALIB_PARAM_AC2_MSB      (2)
#define BMP180_CALIB_PARAM_AC2_LSB      (3)
#define BMP180_CALIB_PARAM_AC3_MSB      (4)
#define BMP180_CALIB_PARAM_AC3_LSB      (5)
#define BMP180_CALIB_PARAM_AC4_MSB      (6)
#define BMP180_CALIB_PARAM_AC4_LSB      (7)
#define BMP180_CALIB_PARAM_AC5_MSB      (8)
#define BMP180_CALIB_PARAM_AC5_LSB      (9)
#define BMP180_CALIB_PARAM_AC6_MSB      (10)
#define BMP180_CALIB_PARAM_AC6_LSB      (11)
#define BMP180_CALIB_PARAM_B1_MSB       (12)
#define BMP180_CALIB_PARAM_B1_LSB       (13)
#define BMP180_CALIB_PARAM_B2_MSB       (14)
#define BMP180_CALIB_PARAM_B2_LSB       (15)
#define BMP180_CALIB_PARAM_MB_MSB       (16)
#define BMP180_CALIB_PARAM_MB_LSB       (17)
#define BMP180_CALIB_PARAM_MC_MSB       (18)
#define BMP180_CALIB_PARAM_MC_LSB       (19)
#define BMP180_CALIB_PARAM_MD_MSB       (20)
#define BMP180_CALIB_PARAM_MD_LSB       (21)

/* REGISTERS */
#define BMP180_TEMPERATURE_DATA_LENGTH          ((uint8_t)2)
#define BMP180_PRESSURE_DATA_LENGTH             ((uint8_t)3)

#define BMP180_CHIP_ID_REG          (0xD0)
#define BMP180_VERSION_REG          (0xD1)

#define BMP180_CTRL_MEAS_REG        (0xF4)
#define BMP180_ADC_OUT_MSB_REG      (0xF6)
#define BMP180_ADC_OUT_LSB_REG      (0xF7)

#define BMP180_SOFT_RESET_REG       (0xE0)

/* temperature measurement */
#define BMP180_T_MEASURE            (0x2E)

/* pressure measurement*/
#define BMP180_P_MEASURE            (0x34)

#define BMP180_TEMPERATURE_DATA_BYTES   (2)
#define BMP180_PRESSURE_DATA_BYTES      (3)
#define BMP180_TEMPERATURE_LSB_DATA     (1)
#define BMP180_TEMPERATURE_MSB_DATA     (0)
#define BMP180_PRESSURE_MSB_DATA        (0)
#define BMP180_PRESSURE_LSB_DATA        (1)
#define BMP180_PRESSURE_XLSB_DATA       (2)

#define BMP180_PARAM_MG     (3038)
#define BMP180_PARAM_MH     (-7357)
#define BMP180_PARAM_MI     (3791)

/* ID */
#define BMP180_ID                (0x55)



using namespace upm;

BMP180::BMP180 (int bus, int devAddr, uint8_t mode) {
    m_name = "BMP180";
    m_controlAddr = devAddr;
    m_bus = bus;

    m_i2c = new mraa::I2c(m_bus);

    // Set mode
    if (mode > BMP180_ULTRAHIGHRES) {
        mode = BMP180_ULTRAHIGHRES;
    }
    oversampling = mode;

    // Check this is a BMP180 chip and get calibration data
    if (!isAvailable() || !getCalibrationData())
        UPM_THROW("Init failed");

    getTemperatureCelcius();
}

BMP180::~BMP180() {
    delete m_i2c;
}

int
BMP180::getPressurePa () {
    int pressure;
    int32_t x1, x2, x3, b3, b6, value;
    uint32_t b4, b7;
    uint32_t raw_pressure = getPressureRaw();

    /*****calculate B6************/
    b6 = b5 - 4000;

    /*****calculate B3************/
    x1 = (b6 * b6) >> 12;
    x1 *= b2;
    x1 >>= 11;

    x2 = (ac2 * b6);
    x2 >>= 11;

    x3 = x1 + x2;
    b3 = (((((int32_t) ac1) * 4 + x3) << oversampling) + 2) >> 2;

    /*****calculate B4************/
    x1 = (ac3 * b6) >> 13;
    x2 = (b1 * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    b4 = (ac4 * (uint32_t)(x3 + 32768)) >> 15;
    b7 = ((uint32_t)(raw_pressure - b3) * (50000 >> oversampling));

    if (b4 == 0)
        UPM_THROW("b4 == 0");

    if (b7 < 0x80000000)
        pressure = (b7 << 1) / b4;
    else
        pressure = (b7 / b4) << 1;

    x1 = pressure >> 8;
    x1 *= x1;
    x1 = (x1 * BMP180_PARAM_MG) >> 16;
    x2 = (pressure * BMP180_PARAM_MH) >> 16;

    /*pressure in Pa*/
    pressure += (x1 + x2 + BMP180_PARAM_MI) >> 4;
    return pressure;
}

int
BMP180::getTemperatureCelcius () {
    uint32_t temp;
    int32_t x1, x2;

    temp = (uint32_t) getTemperatureRaw();

    /* calculate temperature*/
    x1 = (((int32_t) temp - (int32_t) ac6) * (int32_t) ac5) >> 15;

    // Check divisor
    if (x1 == 0 && md == 0)
        UPM_THROW("x1 == 0 && md == 0");

    x2 = ((int32_t) mc << 11) / (x1 + md);
    b5 = x1 + x2;

    int value = ((b5 + 8) >> 4);
    return (value + 5) / 10;
}

uint32_t
BMP180::getPressureRaw () {
    uint8_t data[BMP180_PRESSURE_DATA_BYTES];

    m_i2c->address(m_controlAddr);
    status = m_i2c->writeReg(BMP180_CTRL_MEAS_REG, BMP180_P_MEASURE + (oversampling << 6));
    if (status != mraa::SUCCESS)
        UPM_THROW("BMP180_CTRL_MEAS_REG write failed");

    if (oversampling == BMP180_ULTRALOWPOWER) {
        usleep(5000);
    } else if (oversampling == BMP180_STANDARD) {
        usleep(8000);
    } else if (oversampling == BMP180_HIGHRES) {
        usleep(14000);
    } else {
        usleep(26000);
    }

    int length = m_i2c->readBytesReg(BMP180_ADC_OUT_MSB_REG, data, BMP180_PRESSURE_DATA_LENGTH);
    if (length != BMP180_PRESSURE_DATA_LENGTH)
        UPM_THROW("BMP180_ADC_OUT_MSB_REG read failed");

    return (uint32_t)((((uint32_t)data[BMP180_PRESSURE_MSB_DATA] << 16)
                       | ((uint32_t) data[BMP180_PRESSURE_LSB_DATA] << 8)
                       | (uint32_t) data[BMP180_PRESSURE_XLSB_DATA]) >> (8 - oversampling));
}


uint16_t
BMP180::getTemperatureRaw () {
    uint8_t data[BMP180_TEMPERATURE_DATA_BYTES];

    m_i2c->address(m_controlAddr);
    status = m_i2c->writeReg(BMP180_CTRL_MEAS_REG, BMP180_T_MEASURE);
    if (status != mraa::SUCCESS)
        UPM_THROW("BMP180_CTRL_MEAS_REG write failed");

    usleep(5000);

    int length = m_i2c->readBytesReg(BMP180_ADC_OUT_MSB_REG, data, BMP180_TEMPERATURE_DATA_LENGTH);
    if (length != BMP180_TEMPERATURE_DATA_LENGTH)
        UPM_THROW("BMP180_ADC_OUT_MSB_REG read failed");

    return (uint16_t)((((int32_t)((int8_t)data[BMP180_TEMPERATURE_MSB_DATA])) << 8)
                      | (data[BMP180_TEMPERATURE_LSB_DATA]));
}

bool
BMP180::getCalibrationData() {

    // Holds calibration data
    uint8_t calibration_data[BMP180_CALIB_DATA_SIZE];

    /* Read calibration data */
    m_i2c->address(m_controlAddr);
    int length = m_i2c->readBytesReg(BMP180_PROM_START_ADDR, calibration_data, BMP180_PROM_DATA_LEN);

    // Check we read all calibration data
    if(length != BMP180_PROM_DATA_LEN) { return false; }

    ac1 = (int16_t)(((int32_t)((int8_t)calibration_data[BMP180_CALIB_PARAM_AC1_MSB] << 8))
                    | calibration_data[BMP180_CALIB_PARAM_AC1_LSB]);
    ac2 = (int16_t)(((int32_t)((int8_t)calibration_data[BMP180_CALIB_PARAM_AC2_MSB] << 8))
                    | calibration_data[BMP180_CALIB_PARAM_AC2_LSB]);
    ac3 = (int16_t)(((int32_t)((int8_t)calibration_data[BMP180_CALIB_PARAM_AC3_MSB] << 8))
                    | calibration_data[BMP180_CALIB_PARAM_AC3_LSB]);
    ac4 = (uint16_t)(((uint32_t)((uint8_t)calibration_data[BMP180_CALIB_PARAM_AC4_MSB] << 8))
                     | calibration_data[BMP180_CALIB_PARAM_AC4_LSB]);
    ac5 = (uint16_t)(((uint32_t)((uint8_t)calibration_data[BMP180_CALIB_PARAM_AC5_MSB] << 8))
                     | calibration_data[BMP180_CALIB_PARAM_AC5_LSB]);
    ac6 = (uint16_t)(((uint32_t)((uint8_t)calibration_data[BMP180_CALIB_PARAM_AC6_MSB] << 8))
                     | calibration_data[BMP180_CALIB_PARAM_AC6_LSB]);

    b1 = (int16_t)(((int32_t)((int8_t)calibration_data[BMP180_CALIB_PARAM_B1_MSB] << 8))
                   | calibration_data[BMP180_CALIB_PARAM_B1_LSB]);
    b2 = (int16_t)(((int32_t)((int8_t)calibration_data[BMP180_CALIB_PARAM_B2_MSB] << 8))
                   | calibration_data[BMP180_CALIB_PARAM_B2_LSB]);

    mb = (int16_t)(((int32_t)((int8_t)calibration_data[BMP180_CALIB_PARAM_MB_MSB] << 8))
                   | calibration_data[BMP180_CALIB_PARAM_MB_LSB]);
    mc = (int16_t)(((int32_t)((int8_t)calibration_data[BMP180_CALIB_PARAM_MC_MSB] << 8))
                   | calibration_data[BMP180_CALIB_PARAM_MC_LSB]);
    md = (int16_t)(((int32_t)((int8_t)calibration_data[BMP180_CALIB_PARAM_MD_MSB] << 8))
                   | calibration_data[BMP180_CALIB_PARAM_MD_LSB]);

    return true;
}

bool
BMP180::isAvailable() {
    m_i2c->address(m_controlAddr);
    if (m_i2c->readReg(BMP180_CHIP_ID_REG) != BMP180_ID)  {
        return false;
    }

    return true;
}

