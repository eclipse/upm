/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
