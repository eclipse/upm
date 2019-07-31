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

#include "bmi055.hpp"
#include "upm_string_parser.hpp"

using namespace upm;
using namespace std;

BMI055::BMI055(int accelBus, int accelAddr, int accelCS,
               int gyroBus, int gyroAddr, int gyroCS) :
    m_accel(0), m_gyro(0)
{
    // if -1 is supplied as a bus for any of these, we will not
    // instantiate them

    if (accelBus >= 0)
        m_accel = new BMA250E(accelBus, accelAddr, accelCS);

    if (gyroBus >= 0)
        m_gyro = new BMG160(gyroBus, gyroAddr, gyroCS);

    // now initialize them...
    if (m_accel)
        m_accel->init();

    if (m_gyro)
        m_gyro->init();
}

BMI055::BMI055(std::string initStr) : m_accel(0), m_gyro(0)
{
    int accelBus = BMA250E_DEFAULT_I2C_BUS;
    int accelAddr = BMA250E_DEFAULT_ADDR;
    int accelCS = -1;
    int gyroBus = BMG160_DEFAULT_I2C_BUS;
    int gyroAddr = BMG160_DEFAULT_ADDR;
    int gyroCS = -1;

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
    }

    // if -1 is supplied as a bus for any of these, we will not
    // instantiate them

    if (accelBus >= 0)
        m_accel = new BMA250E(accelBus, accelAddr, accelCS);

    if (gyroBus >= 0)
        m_gyro = new BMG160(gyroBus, gyroAddr, gyroCS);

    // now initialize them...
    if (m_accel)
        m_accel->init();

    if (m_gyro)
        m_gyro->init();

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
    }

}

BMI055::~BMI055()
{
    if (m_accel)
        delete m_accel;

    if (m_gyro)
        delete m_gyro;
}

void BMI055::initAccelerometer(BMA250E_POWER_MODE_T pwr,
                               BMA250E_RANGE_T range,
                               BMA250E_BW_T bw)
{
    if (m_accel)
        m_accel->init(pwr, range, bw);
}

void BMI055::initGyroscope(BMG160_POWER_MODE_T pwr,
                           BMG160_RANGE_T range,
                           BMG160_BW_T bw)
{
    if (m_gyro)
        m_gyro->init(pwr, range, bw);
}

void BMI055::update()
{
    if (m_accel)
        m_accel->update();

    if (m_gyro)
        m_gyro->update();
}

void BMI055::getAccelerometer(float *x, float *y, float *z)
{
    if (m_accel)
        m_accel->getAccelerometer(x, y, z);
}

std::vector<float> BMI055::getAccelerometer()
{
    if (m_accel)
        return m_accel->getAccelerometer();
    else
        return {0, 0, 0};
}

std::vector<float> BMI055::getAcceleration()
{
    if (m_accel)
        return m_accel->getAcceleration();
    else
        return {0, 0, 0};
}

void BMI055::getGyroscope(float *x, float *y, float *z)
{
    if (m_gyro)
        m_gyro->getGyroscope(x, y, z);
}

std::vector<float> BMI055::getGyroscope()
{
    if (m_gyro)
        return m_gyro->getGyroscope();
    else
        return {0, 0, 0};
}
