/*
 * Author: Scott Ware <scott.r.ware@intel.com>
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
#include <math.h>

#include "mraa-utils.h"
#include "si7005.h"

using namespace upm;

SI7005::SI7005 (int bus, int devAddr, int pin) {

    m_controlAddr = devAddr;
    m_bus = bus;
    m_pin = pin;

    last_temperature = 25.0;
    config_reg = SI7005_CONFIG_RESET;

    configured = false;

    // Disable chip until we need to do something with it
    MraaUtils::setGpio(m_pin, 1);

    m_i2cControlCtx = mraa_i2c_init(m_bus);

    mraa_result_t status = mraa_i2c_address(m_i2cControlCtx, m_controlAddr);
    if (status != MRAA_SUCCESS) {
        fprintf(stderr, "SI7005: I2C bus failed to initialise.\n");
        configured = false;
        return;
    }

    if (!isAvailable()) {
        fprintf(stderr, "SI7005: Device at this address is not an Si7005.\n");
        configured = false;
        return;
    }

    configured = true;
}

SI7005::~SI7005() {
    mraa_i2c_stop(m_i2cControlCtx);
}

mraa_result_t
SI7005::getTemperature (float* value) {
    float rawTemperature;

    rawTemperature = getMeasurement( SI7005_CONFIG_TEMPERATURE ) >> 2;

    if(rawTemperature == -1) { return MRAA_ERROR_INVALID_RESOURCE; }

    last_temperature = ( rawTemperature / SI7005_TEMPERATURE_SLOPE ) - SI7005_TEMPERATURE_OFFSET;
    *value = last_temperature;

    return MRAA_SUCCESS;
}

mraa_result_t
SI7005::getHumidity (float* value) {
    float rawHumidity, curve, linearHumidity;

    rawHumidity = getMeasurement( SI7005_CONFIG_HUMIDITY ) >> 4;

    if(rawHumidity == -1) { return MRAA_ERROR_INVALID_RESOURCE; }

    curve = ( rawHumidity / SI7005_HUMIDITY_SLOPE ) - SI7005_HUMIDITY_OFFSET;
    linearHumidity = curve - ( (curve * curve) * A2 + curve * A1 + A0);
    linearHumidity = linearHumidity + ( last_temperature - 30 ) * ( linearHumidity * Q1 + Q0 );
    *value = linearHumidity;

    return MRAA_SUCCESS;
}

int SI7005::getMeasurement(uint8_t configValue) {

    uint16_t rawData;
    uint8_t data[2];
    uint8_t measurementStatus;

    // Enable the sensor
    if(MraaUtils::setGpio(m_pin, 0) != MRAA_SUCCESS) { return -1; }

    // Wait for sensor to wake up
    usleep(SI7005_WAKE_UP_TIME);

    // Setup config register
    mraa_i2c_address(m_i2cControlCtx, m_controlAddr);
    mraa_i2c_write_byte_data(m_i2cControlCtx, (SI7005_CONFIG_START | configValue | config_reg), SI7005_REG_CONFIG);

    // Wait for the measurement to finish
    measurementStatus = SI7005_STATUS_NOT_READY;
    while ( measurementStatus == SI7005_STATUS_NOT_READY ) {
        mraa_i2c_address(m_i2cControlCtx, m_controlAddr);
        measurementStatus = mraa_i2c_read_byte_data(m_i2cControlCtx, SI7005_REG_STATUS);
    }

    if(measurementStatus == -1) { return -1; }

    // Read data registers
    mraa_i2c_address(m_i2cControlCtx, m_controlAddr);
    data[0] = mraa_i2c_read_byte_data(m_i2cControlCtx, SI7005_REG_DATA_HIGH);
    mraa_i2c_address(m_i2cControlCtx, m_controlAddr);
    data[1] = mraa_i2c_read_byte_data(m_i2cControlCtx, SI7005_REG_DATA_LOW);

    // Disable the sensor
    MraaUtils::setGpio(m_pin, 1);

    if(data[0] == -1 || data[1] == -1) { return -1; }

    // Merge MSB and LSB
    rawData  = ( data[0] << 8 );
    rawData |= data[1];

    return rawData;
}

bool
SI7005::isConfigured() {
    return configured;
}

bool
SI7005::isAvailable( )
{
    uint8_t deviceID;

    // Enable the sensor
    if(MraaUtils::setGpio(m_pin, 0) != MRAA_SUCCESS) { return -1; }

    // Wait for sensor to wake up
    usleep(SI7005_WAKE_UP_TIME);

    // Read id register
    mraa_i2c_address(m_i2cControlCtx, m_controlAddr);
    deviceID = mraa_i2c_read_byte_data(m_i2cControlCtx, SI7005_REG_ID);

    // Disable the sensor
    MraaUtils::setGpio(m_pin, 1);

    if(deviceID == -1) { return false; }

    return (( deviceID & SI7005_ID) == SI7005_ID );
}

void
SI7005::enableHeater( )
{
    config_reg |= SI7005_CONFIG_HEAT;
}

void
SI7005::disableHeater( )
{
    config_reg ^= SI7005_CONFIG_HEAT;
}

void
SI7005::enableFastConversionMode( )
{
    config_reg |= SI7005_CONFIG_FAST;
}

void
SI7005::disableFastConversionMode( )
{
    config_reg ^= SI7005_CONFIG_FAST;
}
