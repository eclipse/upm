/*
 * Author: Scott Ware <scott.r.ware@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include "mraa-utils.hpp"
#include "si7005.hpp"

/* REGISTER ADDRESSES */
#define SI7005_REG_STATUS                  ( 0x00 )
#define SI7005_REG_DATA_LENGTH             ( 2 )
#define SI7005_REG_DATA_START              ( 0x01 )
#define SI7005_REG_DATA_HIGH               ( 0 )
#define SI7005_REG_DATA_LOW                ( 1 )
#define SI7005_REG_CONFIG                  ( 0x03 )
#define SI7005_REG_ID                      ( 0x11 )

/* STATUS REGISTER */
#define SI7005_STATUS_NOT_READY            ( 0x01 )

/* CONFIG REGISTER */
#define SI7005_CONFIG_START                ( 0x01 )
#define SI7005_CONFIG_HEAT                 ( 0x02 )
#define SI7005_CONFIG_HUMIDITY             ( 0x00 )
#define SI7005_CONFIG_TEMPERATURE          ( 0x10 )
#define SI7005_CONFIG_FAST                 ( 0x20 )
#define SI7005_CONFIG_RESET                ( 0x00 )

/* ID REGISTER */
#define SI7005_ID                          ( 0x50 )

/* COEFFICIENTS */
#define SI7005_TEMPERATURE_OFFSET          ( 50 )
#define SI7005_TEMPERATURE_SLOPE           ( 32 )
#define SI7005_HUMIDITY_OFFSET             ( 24 )
#define SI7005_HUMIDITY_SLOPE              ( 16 )
#define A0                                 ( -4.7844 )
#define A1                                 ( 0.4008 )
#define A2                                 ( -0.00393 )
#define Q0                                 ( 0.1973 )
#define Q1                                 ( 0.00237 )

/* MISCELLANEOUS */
#define SI7005_WAKE_UP_TIME                ( 15000 )

using namespace upm;

SI7005::SI7005 (int bus, int pin) {

    m_controlAddr = SI7005_ADDRESS;
    m_bus = bus;
    m_pin = pin;
    last_temperature = 25.0;
    config_reg = SI7005_CONFIG_RESET;

    // Disable chip until we need to do something with it
    MraaUtils::setGpio(m_pin, 1);
    m_i2c = new mraa::I2c(m_bus);
    status = m_i2c->address(m_controlAddr);
    if (!isAvailable())
        throw std::runtime_error(std::string(__FUNCTION__) + ": config failure");
}

SI7005::~SI7005() {
    delete m_i2c;
}

uint16_t
SI7005::getTemperatureRaw () {
    return getMeasurement( SI7005_CONFIG_TEMPERATURE );
}

int
SI7005::getTemperatureCelsius () {
    uint16_t rawTemperature = getTemperatureRaw();
    rawTemperature = ((rawTemperature >> 2) & 0xFFFF);
    last_temperature = ((float)rawTemperature) / SI7005_TEMPERATURE_SLOPE - SI7005_TEMPERATURE_OFFSET;
    return static_cast<int>(last_temperature + 0.5);
}

float
SI7005::getTemperature() {
    return getTemperatureCelsius();
}

uint16_t
SI7005::getHumidityRaw () {
    return getMeasurement( SI7005_CONFIG_HUMIDITY );
}

int
SI7005::getHumidityRelative () {
    uint16_t rawHumidity = getHumidityRaw();
    rawHumidity = ((rawHumidity >> 4) & 0xFFFF);
    float linearHumidity = ((float)rawHumidity) / SI7005_HUMIDITY_SLOPE - SI7005_HUMIDITY_OFFSET;
    linearHumidity -= A2 * linearHumidity * linearHumidity + A1 * linearHumidity + A0;
    linearHumidity += ( last_temperature - 30 ) * ( Q1 * linearHumidity + Q0 );
    return static_cast<int>(linearHumidity + 0.5);
}

float
SI7005::getHumidity () {
    return getHumidityRelative();
}

uint16_t SI7005::getMeasurement(uint8_t configValue) {

    uint16_t rawData;
    uint8_t data[SI7005_REG_DATA_LENGTH];
    uint8_t measurementStatus;

    // Enable the sensor
    MraaUtils::setGpio(m_pin, 0);

    // Wait for sensor to wake up
    usleep(SI7005_WAKE_UP_TIME);

    // Setup config register
    status = m_i2c->writeReg(SI7005_REG_CONFIG, SI7005_CONFIG_START | configValue | config_reg);

    // FIXME: readReg() returns 0 on failure which is same as "reading ready" status
    // FIXME: no timeout if device never gets to "reading ready" status
    // Wait for the measurement to finish
    measurementStatus = SI7005_STATUS_NOT_READY;
    while ( measurementStatus == SI7005_STATUS_NOT_READY ) {
        measurementStatus = m_i2c->readReg(SI7005_REG_STATUS);
    }

    // Read data registers
    int length = m_i2c->readBytesReg(SI7005_REG_DATA_START, data, SI7005_REG_DATA_LENGTH);

    // Disable the sensor
    MraaUtils::setGpio(m_pin, 1);

    // Check we got the data we need
    if(length != SI7005_REG_DATA_LENGTH)
        throw std::runtime_error(std::string(__FUNCTION__) + ": read error");

    // Merge MSB and LSB
    rawData  = ((uint16_t)( data[SI7005_REG_DATA_LOW] & 0xFFFF )) + ( (uint16_t)(( data[SI7005_REG_DATA_HIGH] & 0xFFFF ) << 8 ));

    return rawData;
}


bool
SI7005::isAvailable( )
{
    // Enable the sensor
    MraaUtils::setGpio(m_pin, 0);

    // Wait for sensor to wake up
    usleep(SI7005_WAKE_UP_TIME);

    // Read id register
    uint8_t  deviceID = m_i2c->readReg(SI7005_REG_ID);

    // Disable the sensor
    MraaUtils::setGpio(m_pin, 1);

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
