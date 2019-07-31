/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "upm_string_parser.hpp"
#include "bmc150.hpp"

using namespace upm;
using namespace std;

BMC150::BMC150(int accelBus, int accelAddr, int accelCS,
               int magBus, int magAddr, int magCS) :
    m_accel(0), m_mag(0)
{
    // if -1 is supplied as a bus for any of these, we will not
    // instantiate them

    if (accelBus >= 0)
        m_accel = new BMA250E(accelBus, accelAddr, accelCS);

    if (magBus >= 0)
        m_mag = new BMM150(magBus, magAddr, magCS);

    // now initialize them...
    if (m_accel)
        m_accel->init();

    if (m_mag)
        m_mag->init();
}

BMC150::BMC150(std::string initStr)
{
    int accelBus = BMC150_DEFAULT_BUS;
    int accelAddr = BMC150_DEFAULT_ACC_ADDR;
    int accelCS = -1;
    int magBus = BMC150_DEFAULT_BUS;
    int magAddr = BMC150_DEFAULT_MAG_ADDR;
    int magCS = -1;

    std::vector<std::string> upmTokens;
    upmTokens = UpmStringParser::parse(initStr);

    for(std::string tok : upmTokens)
    {
        if(tok.substr(0, 9) == "accelBus:")
            accelBus = std::stoi(tok.substr(9), nullptr, 0);
        if(tok.substr(0, 10) == "accelAddr:")
            accelAddr = std::stoi(tok.substr(10), nullptr, 0);
        if(tok.substr(0, 8) == "accelCS:")
            accelCS = std::stoi(tok.substr(8), nullptr, 0);
        if(tok.substr(0, 7) == "magBus:")
            magBus = std::stoi(tok.substr(7), nullptr, 0);
        if(tok.substr(0, 8) == "magAddr:")
            magAddr = std::stoi(tok.substr(8), nullptr, 0);
        if(tok.substr(0, 6) == "magCS:")
            magCS = std::stoi(tok.substr(6), nullptr, 0);
    }

    // if -1 is supplied as a bus for any of these, we will not
    // instantiate them

    if (accelBus >= 0)
        m_accel = new BMA250E(accelBus, accelAddr, accelCS);

    if (magBus >= 0)
        m_mag = new BMM150(magBus, magAddr, magCS);

    // now initialize them...
    if (m_accel)
        m_accel->init();

    if (m_mag)
        m_mag->init();

    std::string::size_type sz, prev_sz;
    for(std::string tok : upmTokens)
    {
        if(tok.substr(0, 18) == "initAccelerometer:")
        {
            BMA250E_POWER_MODE_T pwr = (BMA250E_POWER_MODE_T)std::stoi(tok.substr(18), &sz, 0);
            tok = tok.substr(18);
            prev_sz = sz;
            BMA250E_RANGE_T range = (BMA250E_RANGE_T)std::stoi(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            BMA250E_BW_T bw = (BMA250E_BW_T)std::stoi(tok.substr(sz + 1), nullptr, 0);
            initAccelerometer(pwr, range, bw);
        }
        if(tok.substr(0, 17) == "initMagnetometer:")
        {
            BMM150_USAGE_PRESETS_T usage = (BMM150_USAGE_PRESETS_T)std::stoi(tok.substr(17), nullptr, 0);
            initMagnetometer(usage);
        }
    }
}

BMC150::~BMC150()
{
    if (m_accel)
        delete m_accel;

    if (m_mag)
        delete m_mag;
}

void BMC150::initAccelerometer(BMA250E_POWER_MODE_T pwr,
                               BMA250E_RANGE_T range,
                               BMA250E_BW_T bw)
{
    if (m_accel)
        m_accel->init(pwr, range, bw);
}

void BMC150::initMagnetometer(BMM150_USAGE_PRESETS_T usage)
{
    if (m_mag)
        m_mag->init(usage);
}

void BMC150::update()
{
    if (m_accel)
        m_accel->update();

    if (m_mag)
        m_mag->update();
}

void BMC150::getAccelerometer(float *x, float *y, float *z)
{
    if (m_accel)
        m_accel->getAccelerometer(x, y, z);
}

std::vector<float> BMC150::getAccelerometer()
{
    if (m_accel)
        return m_accel->getAccelerometer();
    else
        return {0, 0, 0};
}

std::vector<float> BMC150::getAcceleration()
{
    if (m_accel)
        return m_accel->getAcceleration();
    else
        return {0, 0, 0};
}

void BMC150::getMagnetometer(float *x, float *y, float *z)
{
    if (m_mag)
        m_mag->getMagnetometer(x, y, z);
}

std::vector<float> BMC150::getMagnetometer()
{
    update();
    if (m_mag)
        return m_mag->getMagnetometer();
    else
        return {0, 0, 0};
}
