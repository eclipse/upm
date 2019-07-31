/*
 * Author: Gerard Vidal <gerard.vidal@ens-lyon.fr>
 * Copyright (c) 2017 IFE-ENS-Lyon
 * Author: Keelan Lightfoot <keelanlightfoot@gmail.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "tca9548a.hpp"
#include <syslog.h>

#include <unistd.h>
#include <stdexcept>
#include <string>
#include <string.h>
#include <math.h>

using namespace upm;

TCA9548A::TCA9548A (int bus, uint8_t address){
    m_name = "tca9548a";
    if(!(i2c = new mraa::I2c(bus))){
        throw std::invalid_argument(std::string(__FUNCTION__)
                                    +": I2c.init() failed");
        return;
    }

    if((i2c->address(address) != mraa::SUCCESS)){
        throw std::invalid_argument(std::string(__FUNCTION__)
                                    + ": I2c.address() failed");
        return;
    }

    if(i2c->frequency( mraa::I2C_FAST) != mraa::SUCCESS){
        syslog(LOG_WARNING,
               "%s: I2c.frequency(I2C_FAST) failed, using default speed",
               std::string(__FUNCTION__).c_str());
    }
}

TCA9548A::~TCA9548A (){
    delete i2c;
}


bool
TCA9548A::getPort(int port) {
    uint8_t config;
    // Check range
    if (!validPort(port)) {
        throw std::invalid_argument(std::string(__FUNCTION__)
                                    + ": port index out of range");
        return false;
    }
    // Get current port configuration
    config = getPortConfig();
    // Check if the bit is set
    return (config & (0x01 << port));
}

void
TCA9548A::setPort(int port, TCA9548A_PORT_STATE state,
                  TCA9548A_PORT_MODE mode) {
    uint8_t config;
    // Check range
    if (!validPort(port)) {
        throw std::invalid_argument(std::string(__FUNCTION__)
                                    + ": port index out of range");
        return;
    }
    // If we're enabling more than one port at a time, we need to do a
    // read-modify-write.
    if (mode == INCLUSIVE) {
        config = getPortConfig();
    } else {
        config = TCA9548A_NO_PORTS;
    }

    // Convert port number to control byte
    if (state == ENABLED) {
        config |= (0x01 << port);
    } else {
        config &= ~(0x01 << port);
    }
    // Set port
    setPortConfig(config);
}

void
TCA9548A::disableAllPorts() {
    // Turn off all ports
    setPortConfig(TCA9548A_NO_PORTS);
}

void
TCA9548A::enableAllPorts() {
    // Turn on all ports
    setPortConfig(TCA9548A_ALL_PORTS);
}

//Private functions

uint8_t
TCA9548A::getPortConfig() {
    return i2c->readByte();
}

void
TCA9548A::setPortConfig(uint8_t config) {
    if(i2c->writeByte(config) != mraa::SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": I2c.write() failed");
        return;
    }
}

bool
TCA9548A::validPort(int port) {
    return (port >= TCA9548A_MIN_PORT && port <= TCA9548A_MAX_PORT);
}
