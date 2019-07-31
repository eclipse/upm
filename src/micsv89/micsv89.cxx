/*
 * Author: Marc Graham <marc@m2ag.net>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>

#include "micsv89.hpp"

#include "mraa/i2c.hpp"

using namespace upm;

MICSV89::MICSV89 (int bus, uint8_t address) {
    m_name = "micsv89";
    m_valid = false;
    m_address = address;
    i2c = new mraa::I2c(bus);
     if(i2c->address(m_address) != mraa::SUCCESS){
      throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.address() failed");
      return;
    }
    if(i2c->frequency(mraa::I2C_STD) != mraa::SUCCESS){
      throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.frequency(I2C_STD) failed");
      return;
    }
    tx_buf[0] = 0x09;
    tx_buf[1] = 0x00;
    tx_buf[2] = 0x00;
}

void MICSV89::update() {
    m_valid = false;   
    if(i2c->write(tx_buf, 3) != mraa::SUCCESS){
      throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.write() failed");
      return;
    }
    sleep(1); //Give the device time to make the measurement.
    if(i2c->read(rx_buf, 6) != 6){
      throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.read() failed");
      return;
    }
    m_valid = true;
}

float MICSV89::co2equ() {
    return ((rx_buf[0] - 13) * (1600/229) + 400);
}

float MICSV89::getConcentration() {
    update();
    return ((rx_buf[0] - 13) * (1600/229) + 400);
}

int MICSV89::vocshort() {
    return rx_buf[1];
}

float MICSV89::tvoc() {
    return rx_buf[2] * (1000/229);
}

float MICSV89::resistor() {
    return 10 * (rx_buf[3] + (256 * rx_buf[4]) + (65536 * rx_buf[5]));
}

MICSV89::~MICSV89() {
    delete i2c;
}
