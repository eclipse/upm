/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
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
#include <string>

#include "bmp280.hpp"

using namespace upm;
using namespace std;


// conversion from Celsius to Fahrenheit.

static float c2f(float c)
{
  return (c * (9.0 / 5.0) + 32.0);
}

BMP280::BMP280(int bus, int addr, int cs) :
    m_bmp280(bmp280_init(bus, addr, cs))
{
    if (!m_bmp280)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmp280_init() failed");
}

BMP280::~BMP280()
{
    bmp280_close(m_bmp280);
}

void BMP280::update()
{
    if (bmp280_update(m_bmp280))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmp280_update() failed");
}

void BMP280::setSeaLevelPreassure(float seaLevelhPA)
{
    bmp280_set_sea_level_pressure(m_bmp280, seaLevelhPA);
}

float BMP280::getAltitude()
{
    return bmp280_get_altitude(m_bmp280);
}

uint8_t BMP280::readReg(uint8_t reg)
{
    return bmp280_read_reg(m_bmp280, reg);
}

int BMP280::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
    return bmp280_read_regs(m_bmp280, reg, buffer, len);
}

void BMP280::writeReg(uint8_t reg, uint8_t val)
{
    if (bmp280_write_reg(m_bmp280, reg, val))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmp280_write_reg() failed");
}

uint8_t BMP280::getChipID()
{
    return bmp280_get_chip_id(m_bmp280);
}

void BMP280::reset()
{
    bmp280_reset(m_bmp280);
}


float BMP280::getTemperature(bool fahrenheit)
{
    float temperature = bmp280_get_temperature(m_bmp280);

    if (fahrenheit)
        return c2f(temperature);
    else
        return temperature;
}

float BMP280::getPressure()
{
    return bmp280_get_pressure(m_bmp280);
}

void BMP280::setFilter(BMP280_FILTER_T filter)
{
    bmp280_set_filter(m_bmp280, filter);
}

void BMP280::setTimerStandby(BMP280_T_SB_T tsb)
{
    bmp280_set_timer_standby(m_bmp280, tsb);
}

void BMP280::setMeasureMode(BMP280_MODES_T mode)
{
    bmp280_set_measure_mode(m_bmp280, mode);
}

void BMP280::setOversampleRatePressure(BMP280_OSRS_P_T rate)
{
    bmp280_set_oversample_rate_pressure(m_bmp280, rate);
}

void BMP280::setOversampleRateTemperature(BMP280_OSRS_T_T rate)
{
    bmp280_set_oversample_rate_temperature(m_bmp280, rate);
}

uint8_t BMP280::getStatus()
{
    return bmp280_get_status(m_bmp280);
}

void BMP280::setUsageMode(BMP280_USAGE_MODE_T mode)
{
    bmp280_set_usage_mode(m_bmp280, mode);
}

