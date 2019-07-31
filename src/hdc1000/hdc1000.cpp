/*
 * Author: Norbert Wesp <nwesp@phytec.de>
 * Copyright (c) 2017 Phytec Messtechnik GmbH.
 *
 * based on: RIOT-driver hdc1000 by Johann Fischer <j.fischer@phytec.de>
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
#include <stdlib.h>
#include <endian.h>

#include "hdc1000.hpp"

using namespace upm;

HDC1000::HDC1000(int bus, int devAddr) : m_i2ControlCtx(bus) {
    m_temperature = 0;
    m_humidity = 0;

    m_name = HDC1000_NAME;

    m_controlAddr = devAddr;
    m_bus = bus;

    mraa::Result ret = m_i2ControlCtx.address(m_controlAddr);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_address() failed");
    }

    if (checkID() != 0) {
        /* sensor_id does not match! maybe wrong sensor chosen? */
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": checkID() failed");
    }

    sampleData();
}

int
HDC1000::checkID(void)
{
    uint8_t tmp[2];
    uint16_t id;
    int re = 0;

    re = m_i2ControlCtx.readBytesReg(HDC1000_DEVICE_ID_REG, tmp, 2);
    if (re != 2) {
        /* not enough bytes were read! */
        return -1;
    }

    id = ((uint16_t)tmp[0] << 8) | tmp[1];

    if (id != HDC1000_DEVICE_ID) {
        return -1;
    }

    return 0;
}

void
HDC1000::resetSensor(void)
{
    mraa::Result ret = m_i2ControlCtx.writeByte(0);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte() failed");
    }
    usleep(SLEEP_SEC);
}

void
HDC1000::sampleData(void)
{
    uint8_t itemp[4];
    uint16_t traw, hraw;
    int re = 0;

    resetSensor();

    re = m_i2ControlCtx.read(itemp, 4);
    if (re != 4) {
        /* not enough bytes were read! */
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_read(4) failed");
    }

    traw = ((uint16_t)itemp[0] << 8) | itemp[1];
    m_temperature = ((((int32_t)traw * 16500) >> 16) -4000);

    hraw = ((uint16_t)itemp[2] << 8) | itemp[3];
    m_humidity = (((int32_t)hraw * 10000) >> 16);
}

float
HDC1000::getTemperature(int bSampleData)
{
    if (bSampleData) {
        sampleData();
    }
    return (float)(m_temperature * 0.01);
}

float
HDC1000::getTemperature()
{
    return getTemperature(false);
}

float
HDC1000::getHumidity(int bSampleData)
{
    if (bSampleData) {
        sampleData();
    }
    return (float)(m_humidity * 0.01);
}

float
HDC1000::getHumidity()
{
  return getHumidity(false);
}
