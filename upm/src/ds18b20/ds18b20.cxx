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

#include <iostream>
#include <stdexcept>

#include "ds18b20.hpp"

using namespace upm;
using namespace std;

// conversion from celsius to fahrenheit
static float c2f(float c)
{
    return (c * (9.0 / 5.0) + 32.0);
}

DS18B20::DS18B20(int uart) :
    m_ds18b20(ds18b20_init(uart))
{
    if (!m_ds18b20)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ds18b20_init() failed");
}

DS18B20::~DS18B20()
{
    ds18b20_close(m_ds18b20);
}

void DS18B20::init()
{
    // deprecated function.  Handled in ctor now.
}

void DS18B20::update(int index)
{
    if (index >= (int)ds18b20_devices_found(m_ds18b20))
        throw std::out_of_range(string(__FUNCTION__)
                                + ": Invalid index");

    ds18b20_update(m_ds18b20, index);
}

float DS18B20::getTemperature(unsigned int index, bool fahrenheit)
{
    if (index >= ds18b20_devices_found(m_ds18b20))
        throw std::out_of_range(string(__FUNCTION__)
                                + ": Invalid index");

    if (fahrenheit)
        return c2f(ds18b20_get_temperature(m_ds18b20, index));
    else
        return ds18b20_get_temperature(m_ds18b20, index);
}

void DS18B20::setResolution(unsigned int index, DS18B20_RESOLUTIONS_T res)
{
    if (index >= ds18b20_devices_found(m_ds18b20))
    {
        throw std::out_of_range(std::string(__FUNCTION__) +
                                ": device index out of range");
    }

    ds18b20_set_resolution(m_ds18b20, index, res);
}

void DS18B20::copyScratchPad(unsigned int index)
{
    if (index >= ds18b20_devices_found(m_ds18b20))
    {
        throw std::out_of_range(std::string(__FUNCTION__) +
                                ": device index out of range");
    }

    ds18b20_copy_scratchpad(m_ds18b20, index);
}

void DS18B20::recallEEPROM(unsigned int index)
{
    if (index >= ds18b20_devices_found(m_ds18b20))
    {
        throw std::out_of_range(std::string(__FUNCTION__) +
                                ": device index out of range");
    }

    ds18b20_recall_eeprom(m_ds18b20, index);
}

string DS18B20::getId(unsigned int index)
{
    if (index >= ds18b20_devices_found(m_ds18b20))
    {
        throw std::out_of_range(std::string(__FUNCTION__) +
                                ": device index out of range");
    }
    return string((char *)ds18b20_get_id(m_ds18b20, index),
                  MRAA_UART_OW_ROMCODE_SIZE);
}
