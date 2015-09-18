/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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
#include <string>
#include <stdexcept>
#include <unistd.h>
#include "math.h"
#include "itg3200.h"

#define READ_BUFFER_LENGTH 8

//address and id
#define ITG3200_I2C_ADDR 0x68
#define ITG3200_ID 0x00

//configuration registers
#define ITG3200_SMPLRT_DIV 0x15
#define ITG3200_DLPF_FS 0x16

//interrupt registers
#define ITG3200_INT_CFG 0x17
#define ITG3200_INT_STATUS 0x1A

//data registers (read only)
#define ITG3200_TEMP_H 0x1B
#define ITG3200_TEMP_L 0x1C
#define ITG3200_XOUT_H 0x1D
#define ITG3200_XOUT_L 0x1E
#define ITG3200_YOUT_H 0x1F
#define ITG3200_YOUT_L 0x20
#define ITG3200_ZOUT_H 0x21
#define ITG3200_ZOUT_L 0x22
#define DATA_REG_SIZE 8

//power management
#define ITG3200_PWR_MGM 0x3E

//useful values
#define ITG3200_RESET 0x80
#define ITG3200_SLEEP 0x40
#define ITG3200_WAKEUP 0x00

using namespace upm;

Itg3200::Itg3200(int bus) : m_i2c(bus)
{
    m_i2c.address(ITG3200_I2C_ADDR);
    m_buffer[0] = ITG3200_PWR_MGM;
    m_buffer[1] = ITG3200_RESET;
    m_i2c.write(m_buffer, 2);

    Itg3200::calibrate();
    Itg3200::update();
}

void
Itg3200::calibrate(void)
{
    int reads = 600;
    int delay = 4000; // 4 milliseconds
    int skip = 5; // initial samples to skip
    int temp[3] = {0};

    for(int i = 0; i < reads; i++){

        Itg3200::update();
        if (i > skip){
            for (int j = 0; j < 3; j++){
                temp[j] += m_rotation[j];
            }
        }
        usleep(delay);
    }

    for(int i = 0; i < 3; i++){
        m_offsets[i] = (-1) * temp[i] / (reads - skip);
    }
}

float
Itg3200::getTemperature()
{
    return 35.0 + (m_temperature + 13200.0) / 280.0;
}

float*
Itg3200::getRotation()
{
    for(int i = 0; i < 3; i++){
        m_angle[i] = m_rotation[i]/14.375;
    }
    return &m_angle[0];
}

int16_t*
Itg3200::getRawValues()
{
    return &m_rotation[0];
}

int16_t
Itg3200::getRawTemp()
{
    return m_temperature;
}

mraa::Result
Itg3200::update(void)
{
    m_i2c.address(ITG3200_I2C_ADDR);
    m_i2c.writeByte(ITG3200_TEMP_H);

    m_i2c.address(ITG3200_I2C_ADDR);
    m_i2c.read(m_buffer, DATA_REG_SIZE);

    //temp
    //
    m_temperature = (m_buffer[0] << 8 ) | m_buffer[1];
    // x
    m_rotation[0] = ((m_buffer[2] << 8 ) | m_buffer[3]) + m_offsets[0];
    // y
    m_rotation[1] = ((m_buffer[4] << 8 ) | m_buffer[5]) + m_offsets[1];
    // z
    m_rotation[2] = ((m_buffer[6] << 8 ) | m_buffer[7]) + m_offsets[2];

    return mraa::SUCCESS;
}
