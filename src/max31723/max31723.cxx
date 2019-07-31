/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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
#include <stdexcept>

#include "max31723.hpp"

using namespace upm;

MAX31723::MAX31723 (int bus, int csn) : m_spi(bus), m_csnPinCtx(csn) {
    mraa::Result error = mraa::SUCCESS;
    m_name = "MAX31723";

    error = m_csnPinCtx.dir (mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": m_csnPinCtx.dir() failed");
    }

    CSOff ();

    // set spi mode to mode2 (CPOL = 1, CPHA = 0)
    m_spi.mode (mraa::SPI_MODE2);
    // set ontinuously perform temperature conversions
    writeRegister (R_STS_WRITE_CMD, B_CONT_READING);
}

float
MAX31723::getTemperature () {
    uint8_t msb = 0;
    short temperature = 0;

    readRegister (R_TEMPERATURE_LSB);
    msb = readRegister (R_TEMPERATURE_MSB);

    if ((msb & 0x80) != 0) {
        msb &= 0x7F;
        temperature = 0 - msb;

    } else {
        temperature = msb;
    }

    return temperature;
}

/*
 * **************
 *  private area
 * **************
 */

uint8_t
MAX31723::readRegister (uint8_t reg) {
    uint8_t     data[2]     = { 0x00, 0x00 };
    uint8_t*    sensorData  = NULL;

    CSOn ();
    data[0] = reg;
    sensorData = m_spi.write(data, 2);
    CSOff ();

    return sensorData[1];
}

void
MAX31723::writeRegister (uint8_t reg, uint8_t data) {
    uint8_t     buffer[2]   = { 0x00, 0x00 };

    CSOn ();
    buffer[0] = reg;
    buffer[1] = data;
    m_spi.write(buffer, 2);
    CSOff ();
}

mraa::Result
MAX31723::CSOn () {
    return m_csnPinCtx.write (HIGH);
}

mraa::Result
MAX31723::CSOff () {
    return m_csnPinCtx.write (LOW);
}
