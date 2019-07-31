/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <utility>
#include "math.h"
#include "adxl345.hpp"
#include "upm_string_parser.hpp"


#define READ_BUFFER_LENGTH 6

//address and id
#define ADXL345_I2C_ADDR 0x53
#define ADXL345_ID 0x00

//control registers
#define ADXL345_OFSX 0x1E
#define ADXL345_OFSY 0x1F
#define ADXL345_OFSZ 0x20
#define ADXL345_TAP_THRESH 0x1D
#define ADXL345_TAP_DUR 0x21
#define ADXL345_TAP_LATENCY 0x22
#define ADXL345_ACT_THRESH 0x24
#define ADXL345_INACT_THRESH 0x25
#define ADXL345_INACT_TIME 0x26
#define ADXL345_INACT_ACT_CTL 0x27
#define ADXL345_FALL_THRESH 0x28
#define ADXL345_FALL_TIME 0x29
#define ADXL345_TAP_AXES 0x2A
#define ADXL345_ACT_TAP_STATUS 0x2B

//interrupt registers
#define ADXL345_INT_ENABLE 0x2E
#define ADXL345_INT_MAP 0x2F
#define ADXL345_INT_SOURCE 0x30

//data registers (read only)
#define ADXL345_XOUT_L 0x32
#define ADXL345_XOUT_H 0x33
#define ADXL345_YOUT_L 0x34
#define ADXL345_YOUT_H 0x35
#define ADXL345_ZOUT_L 0x36
#define ADXL345_ZOUT_H 0x37
#define DATA_REG_SIZE 6

//data and power management
#define ADXL345_BW_RATE 0x2C
#define ADXL345_POWER_CTL 0x2D
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_FIFO_CTL 0x38
#define ADXL345_FIFO_STATUS 0x39

//useful values
#define ADXL345_POWER_ON 0x08
#define ADXL345_AUTO_SLP 0x30
#define ADXL345_STANDBY 0x00

//scales and resolution
#define ADXL345_FULL_RES 0x08
#define ADXL345_10BIT 0x00
#define ADXL345_2G 0x00
#define ADXL345_4G 0x01
#define ADXL345_8G 0x02
#define ADXL345_16G 0x03

using namespace upm;

Adxl345::Adxl345(int bus) : m_i2c(bus)
{
    //init bus and reset chip
    if ( m_i2c.address(ADXL345_I2C_ADDR) != mraa::SUCCESS ){
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": i2c.address() failed");
        return;
    }

    m_buffer[0] = ADXL345_POWER_CTL;
    m_buffer[1] = ADXL345_POWER_ON;
    if( m_i2c.write(m_buffer, 2) != mraa::SUCCESS){
        throw std::runtime_error(std::string(__FUNCTION__) +
                                    ": i2c.write() control register failed");
        return;
    }

    m_buffer[0] = ADXL345_DATA_FORMAT;
    m_buffer[1] = ADXL345_16G | ADXL345_FULL_RES;
    if( m_i2c.write(m_buffer, 2) != mraa::SUCCESS){
        throw std::runtime_error(std::string(__FUNCTION__) +
                                    ": i2c.write() mode register failed");
        return;
    }

    //2.5V sensitivity is 256 LSB/g = 0.00390625 g/bit
    //3.3V x and y sensitivity is 265 LSB/g = 0.003773584 g/bit, z is the same

    m_offsets[0] = 0.003773584;
    m_offsets[1] = 0.003773584;
    m_offsets[2] = 0.00390625;

    Adxl345::update();
}

Adxl345::Adxl345(std::string initStr) : m_i2c(nullptr), mraaIo(initStr)
{
    mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
    std::vector<std::string> upmTokens;

    if(!mraaIo.getLeftoverStr().empty()) {
      upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
    }

    if(!descs->i2cs) {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                              ": mraa_i2c_init() failed");      
    } else {
      m_i2c = descs->i2cs[0];
    }

    m_buffer[0] = ADXL345_POWER_CTL;
    m_buffer[1] = ADXL345_POWER_ON;
    if( m_i2c.write(m_buffer, 2) != mraa::SUCCESS ){
        throw std::runtime_error(std::string(__FUNCTION__) +
                                    ": i2c.write() control register failed");
        return;
    }

    m_buffer[0] = ADXL345_DATA_FORMAT;
    m_buffer[1] = ADXL345_16G | ADXL345_FULL_RES;
    if( m_i2c.write(m_buffer, 2) != mraa::SUCCESS){
        throw std::runtime_error(std::string(__FUNCTION__) +
                                    ": i2c.write() mode register failed");
        return;
    }

    //2.5V sensitivity is 256 LSB/g = 0.00390625 g/bit
    //3.3V x and y sensitivity is 265 LSB/g = 0.003773584 g/bit, z is the same

    m_offsets[0] = 0.003773584;
    m_offsets[1] = 0.003773584;
    m_offsets[2] = 0.00390625;

    Adxl345::update();
}

Adxl345::~Adxl345()
{
}

float*
Adxl345::getAcceleration()
{
    for(int i = 0; i < 3; i++){
        m_accel[i] = m_rawaccel[i] * m_offsets[i];
    }
    return &m_accel[0];
}

// std::vector<float>
// Adxl345::getAcceleration()
// {
//     update();

//     std::vector<float> v(3);

//     for(int i = 0; i < 3; i++)
//     {
//         v[i] = m_rawaccel[i] * m_offsets[i];
//     }

//     return v;
// }

int16_t*
Adxl345::getRawValues()
{
    return &m_rawaccel[0];
}

uint8_t
Adxl345::getScale(){

    uint8_t result;

    m_i2c.writeByte(ADXL345_DATA_FORMAT);

    result = m_i2c.readByte();

    return pow(2, (result & 0x03) + 1);
}

mraa::Result
Adxl345::update(void)
{
    m_i2c.writeByte(ADXL345_XOUT_L);

    m_i2c.read(m_buffer, DATA_REG_SIZE);

    // x
    m_rawaccel[0] = ((m_buffer[1] << 8 ) | m_buffer[0]);
    // y
    m_rawaccel[1] = ((m_buffer[3] << 8 ) | m_buffer[2]);
    // z
    m_rawaccel[2] = ((m_buffer[5] << 8 ) | m_buffer[4]);

    return mraa::SUCCESS;
}
