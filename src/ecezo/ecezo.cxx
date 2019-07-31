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

#include "ecezo.hpp"

using namespace upm;
using namespace std;

ECEZO::ECEZO(unsigned int bus, unsigned int addrBaud, bool isI2C) :
    m_ecezo(nullptr)
{
    if (isI2C)
        m_ecezo = ecezo_i2c_init(bus, addrBaud);
    else
        m_ecezo = ecezo_uart_init(bus, addrBaud);

    if (!m_ecezo)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ecezo_*_init() failed");
}

ECEZO::~ECEZO()
{
    ecezo_close(m_ecezo);
}

void ECEZO::update()
{
    if (ecezo_update(m_ecezo))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ecezo_update() failed");
}

void ECEZO::setTemperature(float temp)
{
    if (ecezo_set_temperature(m_ecezo, temp))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ecezo_set_temperature() failed");
}

void ECEZO::setKValue(float k)
{
    if (ecezo_set_k_value(m_ecezo, k))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ecezo_set_k_value() failed");
}

void ECEZO::setSleep(bool enable)
{
    if (ecezo_set_sleep(m_ecezo, enable))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ecezo_set_sleep() failed");
}

float ECEZO::getEC()
{
    return ecezo_get_ec(m_ecezo);
}

float ECEZO::getECValue()
{
    ECEZO::update();
    return ECEZO::getEC();
}

float ECEZO::getTDS()
{
    return ecezo_get_tds(m_ecezo);
}

float ECEZO::getSalinity()
{
    return ecezo_get_salinity(m_ecezo);
}

float ECEZO::getSG()
{
    return ecezo_get_sg(m_ecezo);
}

void ECEZO::calibrate(ECEZO_CALIBRATION_T cal, float ec)
{
    if (ecezo_calibrate(m_ecezo, cal, ec))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ecezo_calibrate() failed");
}

void ECEZO::setContinuous(bool enable)
{
    if (ecezo_set_continuous(m_ecezo, enable))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ecezo_set_continuous() failed");
}

string ECEZO::sendCommand(string cmd)
{
    char buffer[ECEZO_MAX_BUFFER_LEN];

    int rv;

    if ((rv = ecezo_send_command(m_ecezo, (char *)cmd.c_str(),
                                 buffer, ECEZO_MAX_BUFFER_LEN)) < 0)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ecezo_send_command() failed");

    return string(buffer, rv);
}

string ECEZO::read()
{
    char buffer[ECEZO_MAX_BUFFER_LEN];

    int rv;

    if ((rv = ecezo_read(m_ecezo, buffer, ECEZO_MAX_BUFFER_LEN)) < 0)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ecezo_read() failed");

    return string(buffer, rv);
}

void ECEZO::write(std::string data)
{
    if (ecezo_write(m_ecezo, (char*)data.data(),
                    data.size()))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ecezo_write() failed");
}
