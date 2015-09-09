/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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

#include <string>
#include <stdexcept>

#include "math.h"
#include "hmc5883l.h"

#define MAX_BUFFER_LENGTH 6
#define HMC5883L_I2C_ADDR 0x1E

//configuration registers
#define HMC5883L_CONF_REG_A 0x00
#define HMC5883L_CONF_REG_B 0x01

//mode register
#define HMC5883L_MODE_REG 0x02

//data register
#define HMC5883L_X_MSB_REG 0
#define HMC5883L_X_LSB_REG 1
#define HMC5883L_Z_MSB_REG 2
#define HMC5883L_Z_LSB_REG 3
#define HMC5883L_Y_MSB_REG 4
#define HMC5883L_Y_LSB_REG 5
#define DATA_REG_SIZE 6

//status register
#define HMC5883L_STATUS_REG 0x09

//ID registers
#define HMC5883L_ID_A_REG 0x0A
#define HMC5883L_ID_B_REG 0x0B
#define HMC5883L_ID_C_REG 0x0C

#define HMC5883L_CONT_MODE 0x00
#define HMC5883L_DATA_REG 0x03

//scales
#define GA_0_88_REG 0x00 << 5
#define GA_1_3_REG 0x01 << 5
#define GA_1_9_REG 0x02 << 5
#define GA_2_5_REG 0x03 << 5
#define GA_4_0_REG 0x04 << 5
#define GA_4_7_REG 0x05 << 5
#define GA_5_6_REG 0x06 << 5
#define GA_8_1_REG 0x07 << 5

//digital resolutions
#define SCALE_0_73_MG 0.73
#define SCALE_0_92_MG 0.92
#define SCALE_1_22_MG 1.22
#define SCALE_1_52_MG 1.52
#define SCALE_2_27_MG 2.27
#define SCALE_2_56_MG 2.56
#define SCALE_3_03_MG 3.03
#define SCALE_4_35_MG 4.35

using namespace upm;

Hmc5883l::Hmc5883l(int bus) : m_i2c(bus)
{
    mraa::Result error;
    error = m_i2c.address(HMC5883L_I2C_ADDR);
    if(error != mraa::SUCCESS){
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": i2c.address() failed");
        return;
    }
    m_rx_tx_buf[0] = HMC5883L_CONF_REG_B;
    m_rx_tx_buf[1] = GA_1_3_REG;
    error = m_i2c.write(m_rx_tx_buf, 2);
    if(error != mraa::SUCCESS){
        throw std::runtime_error(std::string(__FUNCTION__) +
                                    ": i2c.write() configuration failed");
        return;
    }

    error = m_i2c.address(HMC5883L_I2C_ADDR);
    if(error != mraa::SUCCESS){
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": i2c.address() failed");
        return;
    }
    m_rx_tx_buf[0] = HMC5883L_MODE_REG;
    m_rx_tx_buf[1] = HMC5883L_CONT_MODE;
    error = m_i2c.write(m_rx_tx_buf, 2);
    if(error != mraa::SUCCESS){
        throw std::runtime_error(std::string(__FUNCTION__) +
                                    ": i2c.write() mode failed");
        return;
    }

    Hmc5883l::update();
}

mraa::Result
Hmc5883l::update(void)
{
    m_i2c.address(HMC5883L_I2C_ADDR);
    m_i2c.writeByte(HMC5883L_DATA_REG);

    m_i2c.address(HMC5883L_I2C_ADDR);
    m_i2c.read(m_rx_tx_buf, DATA_REG_SIZE);

    // x
    m_coor[0] = (m_rx_tx_buf[HMC5883L_X_MSB_REG] << 8 ) | m_rx_tx_buf[HMC5883L_X_LSB_REG];
    // z
    m_coor[2] = (m_rx_tx_buf[HMC5883L_Z_MSB_REG] << 8 ) | m_rx_tx_buf[HMC5883L_Z_LSB_REG];
    // y
    m_coor[1] = (m_rx_tx_buf[HMC5883L_Y_MSB_REG] << 8 ) | m_rx_tx_buf[HMC5883L_Y_LSB_REG];

    return mraa::SUCCESS;
}

float
Hmc5883l::direction(void)
{
    return atan2(m_coor[1] * SCALE_0_92_MG, m_coor[0] * SCALE_0_92_MG) + m_declination;
}

float
Hmc5883l::heading(void)
{
    float dir = Hmc5883l::direction() * 180/M_PI;
    if(dir < 0){
        dir += 360.0;
    }
    return dir;
}

int16_t*
Hmc5883l::coordinates(void)
{
    return &m_coor[0];
}

void
Hmc5883l::set_declination(float dec)
{
    m_declination = dec;
}

float
Hmc5883l::get_declination()
{
    return m_declination;
}
