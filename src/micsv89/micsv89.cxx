/*
 * Author: Marc Graham <marc@m2ag.net>
 * Copyright (c) 2015 Intel Corporation.
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

#include "micsv89.h"

using namespace upm;

MICSV89::MICSV89 (int bus, uint8_t address) {
    m_name = "micsv89";
    m_valid = false;
    m_address = address;
    i2c = new mraa::I2c(bus);
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
    if(i2c->address(m_address) != mraa::SUCCESS){
      throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.address() failed");
      return;
    }
    if(i2c->write(tx_buf, 3) != mraa::SUCCESS){
      throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.write() failed");
      return;
    }
    sleep(1); //Give the device time to make the measurement.
    if(i2c->address(m_address) != mraa::SUCCESS){
      throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.address() failed");
      return;
    }
    if(i2c->read(rx_buf, 6) != 6){
      throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.read() failed");
      return;
    }
    m_valid = true;
}

float MICSV89::co2equ() {
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
