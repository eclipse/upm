/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Based on InvenSense MPU-6050 register map document rev. 2.0, 5/19/2011 (RM-MPU-6000A-00)
 * 8/24/2011 by Jeff Rowberg <jeff@rowberg.net>
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

#include "mpu9150.h"

using namespace upm;

MPU9150::MPU9150 (int bus, int devAddr) {
    m_name = "MPU9150";

    m_i2cAddr = devAddr;
    m_bus = bus;

    m_i2Ctx = mraa_i2c_init(m_bus);

    mraa_result_t ret = mraa_i2c_address(m_i2Ctx, m_i2cAddr);
    if (ret != MRAA_SUCCESS) {
        fprintf(stderr, "Messed up i2c bus\n");
    }

    initSensor ();
}

MPU9150::~MPU9150() {
    mraa_i2c_stop(m_i2Ctx);
}

mraa_result_t
MPU9150::initSensor () {
    uint8_t regData = 0x0;

    // setClockSource
    updateRegBits ( MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT,
                    MPU6050_PWR1_CLKSEL_LENGTH, MPU6050_CLOCK_PLL_XGYRO);
    // setFullScaleGyroRange
    updateRegBits ( MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT,
                    MPU6050_GCONFIG_FS_SEL_LENGTH, MPU6050_GYRO_FS_250);
    // setFullScaleAccelRange
    updateRegBits ( MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT,
                    MPU6050_ACONFIG_AFS_SEL_LENGTH, MPU6050_ACCEL_FS_2);
    // setSleepEnabled
    i2cReadReg_N (MPU6050_RA_PWR_MGMT_1, 0x1, &regData);
    regData &= ~(1 << MPU6050_PWR1_SLEEP_BIT);
    i2cWriteReg (MPU6050_RA_PWR_MGMT_1, regData);

    return MRAA_SUCCESS;
}

uint8_t
MPU9150::getDeviceID () {
    uint8_t regData = 0x0;
    getRegBits (MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, &regData);
    return regData;
}

void
MPU9150::getData () {
    uint8_t buffer[14];

    for (int i = 0; i < SMOOTH_TIMES; i++) {
        i2cReadReg_N (MPU6050_RA_ACCEL_XOUT_H, 14, buffer);
        axisAcceleromter.rawData.axisX  = (((int16_t)buffer[0]) << 8)  | buffer[1];
        axisAcceleromter.rawData.axisY  = (((int16_t)buffer[2]) << 8)  | buffer[3];
        axisAcceleromter.rawData.axisZ  = (((int16_t)buffer[4]) << 8)  | buffer[5];
        axisAcceleromter.sumData.axisX  += (double) axisAcceleromter.rawData.axisX / 16384;
        axisAcceleromter.sumData.axisY  += (double) axisAcceleromter.rawData.axisY / 16384;
        axisAcceleromter.sumData.axisZ  += (double) axisAcceleromter.rawData.axisZ / 16384;

        axisGyroscope.rawData.axisX     = (((int16_t)buffer[8]) << 8)  | buffer[9];
        axisGyroscope.rawData.axisY     = (((int16_t)buffer[10]) << 8) | buffer[11];
        axisGyroscope.rawData.axisZ     = (((int16_t)buffer[12]) << 8) | buffer[13];
        axisGyroscope.sumData.axisX     += (double) axisAcceleromter.rawData.axisX * 250 / 32768;
        axisGyroscope.sumData.axisY     += (double) axisAcceleromter.rawData.axisY * 250 / 32768;
        axisGyroscope.sumData.axisZ     += (double) axisAcceleromter.rawData.axisZ * 250 / 32768;

        i2cWriteReg (MPU6050_RA_INT_PIN_CFG, 0x02);
        usleep (10000);
        m_i2cAddr = MPU9150_RA_MAG_ADDRESS;
        i2cWriteReg (0x0A, 0x01);
        usleep (10000);
        i2cReadReg_N (MPU9150_RA_MAG_XOUT_L, 6, buffer);
        m_i2cAddr = ADDR;

        axisMagnetomer.rawData.axisX  = (((int16_t)buffer[0]) << 8)  | buffer[1];
        axisMagnetomer.rawData.axisY  = (((int16_t)buffer[2]) << 8)  | buffer[3];
        axisMagnetomer.rawData.axisZ  = (((int16_t)buffer[4]) << 8)  | buffer[5];
        axisMagnetomer.sumData.axisX  += (double) axisMagnetomer.rawData.axisX * 1200 / 4096;
        axisMagnetomer.sumData.axisY  += (double) axisMagnetomer.rawData.axisY * 1200 / 4096;
        axisMagnetomer.sumData.axisZ  += (double) axisMagnetomer.rawData.axisZ * 1200 / 4096;
    }

    axisAcceleromter.data.axisX = axisAcceleromter.sumData.axisX / SMOOTH_TIMES;
    axisAcceleromter.data.axisY = axisAcceleromter.sumData.axisY / SMOOTH_TIMES;
    axisAcceleromter.data.axisZ = axisAcceleromter.sumData.axisZ / SMOOTH_TIMES;

    axisGyroscope.data.axisX = axisGyroscope.sumData.axisX / SMOOTH_TIMES;
    axisGyroscope.data.axisY = axisGyroscope.sumData.axisY / SMOOTH_TIMES;
    axisGyroscope.data.axisZ = axisGyroscope.sumData.axisZ / SMOOTH_TIMES;

    axisMagnetomer.data.axisX = axisMagnetomer.sumData.axisX / SMOOTH_TIMES;
    axisMagnetomer.data.axisY = axisMagnetomer.sumData.axisY / SMOOTH_TIMES;
    axisMagnetomer.data.axisZ = axisMagnetomer.sumData.axisZ / SMOOTH_TIMES;
}

mraa_result_t
MPU9150::getAcceleromter (Vector3D * data) {
    data->axisX = axisAcceleromter.data.axisX;
    data->axisY = axisAcceleromter.data.axisY;
    data->axisZ = axisAcceleromter.data.axisZ;

    return MRAA_SUCCESS;
}

mraa_result_t
MPU9150::getGyro (Vector3D * data) {
    data->axisX = axisGyroscope.data.axisX;
    data->axisY = axisGyroscope.data.axisY;
    data->axisZ = axisGyroscope.data.axisZ;

    return MRAA_SUCCESS;
}

mraa_result_t
MPU9150::getMagnometer (Vector3D * data) {
    data->axisX = axisMagnetomer.data.axisX;
    data->axisY = axisMagnetomer.data.axisY;
    data->axisZ = axisMagnetomer.data.axisZ;

    return MRAA_SUCCESS;
}

float
MPU9150::getTemperature () {
    uint8_t buffer[2];
    uint16_t tempRaw = 0;

    updateRegBits (MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_TEMP_DIS_BIT, 0x1, 0x0);
    i2cReadReg_N (MPU6050_RA_TEMP_OUT_H, 2, buffer);
    tempRaw = (((int16_t)buffer[0]) << 8) | buffer[1];

    return (float)tempRaw / 340.0 + 35.0;
}

/*
 * **************
 *  private area
 * **************
 */
uint16_t
MPU9150::i2cReadReg_N (int reg, unsigned int len, uint8_t * buffer) {
    int readByte = 0;
    mraa_i2c_address(m_i2Ctx, m_i2cAddr);
    mraa_i2c_write_byte(m_i2Ctx, reg);

    mraa_i2c_address(m_i2Ctx, m_i2cAddr);
    readByte = mraa_i2c_read(m_i2Ctx, buffer, len);
    return readByte;
}

mraa_result_t
MPU9150::i2cWriteReg (uint8_t reg, uint8_t value) {
    mraa_result_t error = MRAA_SUCCESS;

    uint8_t data[2] = { reg, value };
    error = mraa_i2c_address (m_i2Ctx, m_i2cAddr);
    error = mraa_i2c_write (m_i2Ctx, data, 2);

    return error;
}

int
MPU9150::updateRegBits (uint8_t reg, uint8_t bitStart, uint8_t length, uint16_t data) {
    uint8_t regData;

    if (i2cReadReg_N (reg, 0x1, &regData) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        regData &= ~(mask); // zero all important bits in existing byte
        regData |= data; // combine data with existing byte
        return i2cWriteReg (reg, regData);
    } else {
        return 0x0;
    }
}

uint8_t
MPU9150::getRegBits (uint8_t reg, uint8_t bitStart, uint8_t length, uint8_t * data) {
    uint8_t count = 0;
    uint8_t regData;
    if (i2cReadReg_N (reg, 0x1, &regData) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        regData &= mask;
        regData >>= (bitStart - length + 1);
        *data = regData;
    }
    return count;
}
