/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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
#include <string>

#include "bmi160.hpp"

using namespace upm;
using namespace std;

BMI160::BMI160(int bus, int address, int csPin, bool enableMag) :
    m_bmi160(bmi160_init(bus, address, csPin, enableMag))
{
    if (!m_bmi160)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmi160_init() failed");
}

BMI160::~BMI160()
{
    bmi160_close(m_bmi160);
}

void BMI160::update()
{
    bmi160_update(m_bmi160);
}

void BMI160::setAccelerometerScale(BMI160_ACC_RANGE_T scale)
{
    bmi160_set_accelerometer_scale(m_bmi160, scale);
}

void BMI160::setGyroscopeScale(BMI160_GYRO_RANGE_T scale)
{
    bmi160_set_gyroscope_scale(m_bmi160, scale);
}

void BMI160::getAccelerometer(float *x, float *y, float *z)
{
    bmi160_get_accelerometer(m_bmi160, x, y, z);
}

void BMI160::getGyroscope(float *x, float *y, float *z)
{
    bmi160_get_gyroscope(m_bmi160, x, y, z);
}

void BMI160::getMagnetometer(float *x, float *y, float *z)
{
    bmi160_get_magnetometer(m_bmi160, x, y, z);
}

float *BMI160::getAccelerometer()
{
  static float values[3]; // x, y, and then z

  getAccelerometer(&values[0], &values[1], &values[2]);

  return values;
}

float *BMI160::getGyroscope()
{
  static float values[3]; // x, y, and then z

  getGyroscope(&values[0], &values[1], &values[2]);

  return values;
}

float *BMI160::getMagnetometer()
{
  static float values[3]; // x, y, and then z

  getMagnetometer(&values[0], &values[1], &values[2]);

  return values;
}

void BMI160::enableMagnetometer(bool enable)
{
    bmi160_enable_magnetometer(m_bmi160, enable);
}

unsigned int BMI160::getSensorTime()
{
    return bmi160_get_time(m_bmi160);
}

string BMI160::busRead(int addr, int reg, int len)
{
    u8 dev_addr = (u8)(addr & 0xff);
    u8 reg_addr = (u8)(reg & 0xff);
    u8 cnt = (u8)(len & 0xff);

    u8 *data = new u8[cnt];

    if (bmi160_bus_read(dev_addr, reg_addr, data, cnt))
    {
        delete [] data;
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmi160_bus_read() failed");
    }

    string dataStr((char *)data, cnt);
    delete [] data;

    return dataStr;
}

void BMI160::busWrite(int addr, int reg, string data)
{
    u8 dev_addr = (u8)(addr & 0xff);
    u8 reg_addr = (u8)(reg & 0xff);

    if (bmi160_bus_write(dev_addr, reg_addr, (u8 *)data.data(), data.size()))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmi160_bus_write() failed");
}
