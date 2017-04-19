/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
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

#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <string.h>

#include "lsm303d.hpp"

using namespace upm;
using namespace std;

LSM303D::LSM303D(int bus, int addr) :
    m_lsm303d(lsm303d_init(bus, addr))
{
    if (!m_lsm303d)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303d_init() failed");
}

LSM303D::~LSM303D()
{
    lsm303d_close(m_lsm303d);
}

void LSM303D::init(LSM303D_M_RES_T res)
{
    if (lsm303d_devinit(m_lsm303d, res))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303d_devinit() failed");
}

void LSM303D::update()
{
    if (lsm303d_update(m_lsm303d))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303d_update() failed");
}

uint8_t LSM303D::readReg(uint8_t reg)
{
    return lsm303d_read_reg(m_lsm303d, reg);
}

int LSM303D::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
    int rv = lsm303d_read_regs(m_lsm303d, reg, buffer, len);
    if (rv < 0)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303d_read_regs() failed");

    return rv;
}

void LSM303D::writeReg(uint8_t reg, uint8_t val)
{
    if (lsm303d_write_reg(m_lsm303d, reg, val))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303d_write_reg() failed");
}

void LSM303D::getMagnetometer(float *x, float *y, float *z)
{
    lsm303d_get_magnetometer(m_lsm303d, x, y, z);
}

std::vector<float> LSM303D::getMagnetometer()
{
    float v[3];

    getMagnetometer(&v[0], &v[1], &v[2]);
    return std::vector<float>(v, v+3);
}

void LSM303D::getAccelerometer(float *x, float *y, float *z)
{
    lsm303d_get_accelerometer(m_lsm303d, x, y, z);
}

std::vector<float> LSM303D::getAccelerometer()
{
    float v[3];

    getAccelerometer(&v[0], &v[1], &v[2]);
    return std::vector<float>(v, v+3);
}

float LSM303D::getTemperature()
{
    return lsm303d_get_temperature(m_lsm303d);
}

void LSM303D::setAccelerometerFullScale(LSM303D_AFS_T fs)
{
    if (lsm303d_set_acc_full_scale(m_lsm303d, fs))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303d_set_acc_full_scale() failed");
}

void LSM303D::setMagnetometerFullScale(LSM303D_MFS_T fs)
{
    if (lsm303d_set_mag_full_scale(m_lsm303d, fs))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303d_set_mag_full_scale() failed");
}
void LSM303D::setAccelerometerODR(LSM303D_AODR_T odr)
{
    if (lsm303d_set_acc_odr(m_lsm303d, odr))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303d_set_acc_odr() failed");
}

void LSM303D::setMagnetometerODR(LSM303D_MODR_T odr)
{
    if (lsm303d_set_mag_odr(m_lsm303d, odr))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303d_set_mag_odr() failed");
}
