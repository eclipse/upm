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

#include "bmx055.hpp"
#include "upm_string_parser.hpp"

using namespace upm;
using namespace std;

BMX055::BMX055(int accelBus, int accelAddr, int accelCS,
               int gyroBus, int gyroAddr, int gyroCS,
               int magBus, int magAddr, int magCS) :
    m_accel(0), m_gyro(0), m_mag(0)
{
    // if -1 is supplied as a bus for any of these, we will not
    // instantiate them

    if (accelBus >= 0)
        m_accel = new BMA250E(accelBus, accelAddr, accelCS);

    if (gyroBus >= 0)
        m_gyro = new BMG160(gyroBus, gyroAddr, gyroCS);

    if (magBus >= 0)
        m_mag = new BMM150(magBus, magAddr, magCS);
}

BMX055::BMX055(std::string initStr) :m_accel(0), m_gyro(0), m_mag(0)
{
    int accelBus = BMA250E_DEFAULT_I2C_BUS;
    int accelAddr = BMA250E_DEFAULT_ADDR;
    int accelCS = -1;

    int gyroBus = BMG160_DEFAULT_I2C_BUS;
    int gyroAddr = BMG160_DEFAULT_ADDR;
    int gyroCS = -1;

    int magBus = BMM150_DEFAULT_I2C_BUS;
    int magAddr = BMX055_DEFAULT_MAG_I2C_ADDR;
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
        if(tok.substr(0, 8) == "gyroBus:")
            gyroBus = std::stoi(tok.substr(8), nullptr, 0);
        if(tok.substr(0, 9) == "gyroAddr:")
            gyroAddr = std::stoi(tok.substr(9), nullptr, 0);
        if(tok.substr(0, 7) == "gyroCS:")
            gyroCS = std::stoi(tok.substr(7), nullptr, 0);
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

    if (gyroBus >= 0)
        m_gyro = new BMG160(gyroBus, gyroAddr, gyroCS);

    if (magBus >= 0)
        m_mag = new BMM150(magBus, magAddr, magCS);

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
        if(tok.substr(0, 14) == "initGyroscope:")
        {
            BMG160_POWER_MODE_T pwr = (BMG160_POWER_MODE_T)std::stoi(tok.substr(14), &sz, 0);
            tok = tok.substr(14);
            prev_sz = sz;
            BMG160_RANGE_T range = (BMG160_RANGE_T)std::stoi(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            BMG160_BW_T bw = (BMG160_BW_T)std::stoi(tok.substr(sz + 1), nullptr, 0);
            initGyroscope(pwr, range, bw);
        }
        if(tok.substr(0, 17) == "initMagnetometer:")
        {
            BMM150_USAGE_PRESETS_T usage = (BMM150_USAGE_PRESETS_T)std::stoi(tok.substr(17), nullptr, 0);
            initMagnetometer(usage);
        }
    }
}

BMX055::~BMX055()
{
    if (m_accel)
        delete m_accel;

    if (m_gyro)
        delete m_gyro;

    if (m_mag)
        delete m_mag;
}

void BMX055::initAccelerometer(BMA250E_POWER_MODE_T pwr,
                               BMA250E_RANGE_T range,
                               BMA250E_BW_T bw)
{
    if (m_accel)
        m_accel->init(pwr, range, bw);
}

void BMX055::initGyroscope(BMG160_POWER_MODE_T pwr,
                           BMG160_RANGE_T range,
                           BMG160_BW_T bw)
{
    if (m_gyro)
        m_gyro->init(pwr, range, bw);
}

void BMX055::initMagnetometer(BMM150_USAGE_PRESETS_T usage)
{
    if (m_mag)
        m_mag->init(usage);
}

void BMX055::update()
{
    if (m_accel)
        m_accel->update();

    if (m_gyro)
        m_gyro->update();

    if (m_mag)
        m_mag->update();
}

void BMX055::getAccelerometer(float *x, float *y, float *z)
{
    if (m_accel)
        m_accel->getAccelerometer(x, y, z);
    else
    {
        if (x)
            *x = 0;
        if (y)
            *y = 0;
        if (z)
            *z = 0;
    }
}

std::vector<float> BMX055::getAccelerometer()
{
    if (m_accel)
        return m_accel->getAccelerometer();
    else
        return {0, 0, 0};
}

std::vector<float> BMX055::getAcceleration()
{
    if (m_accel)
        return m_accel->getAcceleration();
    else
        return {0, 0, 0};
}

void BMX055::getGyroscope(float *x, float *y, float *z)
{
    if (m_gyro)
        m_gyro->getGyroscope(x, y, z);
    else
    {
        if (x)
            *x = 0;
        if (y)
            *y = 0;
        if (z)
            *z = 0;
    }
}

std::vector<float> BMX055::getGyroscope()
{
    if (m_gyro)
        return m_gyro->getGyroscope();
    else
        return {0, 0, 0};
}

void BMX055::getMagnetometer(float *x, float *y, float *z)
{
    if (m_mag)
        m_mag->getMagnetometer(x, y, z);
    else
    {
        if (x)
            *x = 0;
        if (y)
            *y = 0;
        if (z)
            *z = 0;
    }
}

std::vector<float> BMX055::getMagnetometer()
{
    update();
    if (m_mag)
        return m_mag->getMagnetometer();
    else
        return {0, 0, 0};
}
