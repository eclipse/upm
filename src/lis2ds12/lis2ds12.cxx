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

#include "lis2ds12.hpp"

using namespace upm;
using namespace std;

// conversion from Celsius to Fahrenheit

static float c2f(float c)
{
    return (c * (9.0 / 5.0) + 32.0);
}

LIS2DS12::LIS2DS12(int bus, int addr, int cs) :
    m_lis2ds12(lis2ds12_init(bus, addr, cs))
{
    if (!m_lis2ds12)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_init() failed");
}

LIS2DS12::~LIS2DS12()
{
    lis2ds12_close(m_lis2ds12);
}

void LIS2DS12::init(LIS2DS12_ODR_T odr, LIS2DS12_FS_T fs)
{
    if (lis2ds12_devinit(m_lis2ds12, odr, fs))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_devinit() failed");
}

void LIS2DS12::update()
{
    if (lis2ds12_update(m_lis2ds12))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_update() failed");
}

uint8_t LIS2DS12::readReg(uint8_t reg)
{
    return lis2ds12_read_reg(m_lis2ds12, reg);
}

int LIS2DS12::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
    int rv = lis2ds12_read_regs(m_lis2ds12, reg, buffer, len);
    if (rv != len)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_read_regs() failed");

    return rv;
}

void LIS2DS12::writeReg(uint8_t reg, uint8_t val)
{
    if (lis2ds12_write_reg(m_lis2ds12, reg, val))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_write_reg() failed");
}

uint8_t LIS2DS12::getChipID()
{
    return lis2ds12_get_chip_id(m_lis2ds12);
}

void LIS2DS12::getAccelerometer(float *x, float *y, float *z)
{
    lis2ds12_get_accelerometer(m_lis2ds12, x, y, z);
}

std::vector<float> LIS2DS12::getAccelerometer()
{
    float v[3];

    getAccelerometer(&v[0], &v[1], &v[2]);
    return std::vector<float>(v, v+3);
}

std::vector<float> LIS2DS12::getAcceleration()
{
    std::vector<float> v(3);

    lis2ds12_get_accelerometer(m_lis2ds12, &v[0], &v[1], &v[2]);
    return v;
}

float LIS2DS12::getTemperature(bool fahrenheit)
{
    float temperature = lis2ds12_get_temperature(m_lis2ds12);
    if (fahrenheit)
        return c2f(temperature);
    else
        return temperature;
}

void LIS2DS12::reset()
{
    if (lis2ds12_reset(m_lis2ds12))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_reset() failed");
}

void LIS2DS12::setODR(LIS2DS12_ODR_T odr)
{
    if (lis2ds12_set_odr(m_lis2ds12, odr))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_set_odr() failed");
}

void LIS2DS12::setFullScale(LIS2DS12_FS_T fs)
{
    if (lis2ds12_set_full_scale(m_lis2ds12, fs))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_set_full_scale() failed");
}

void LIS2DS12::enableHPFiltering(bool filter)
{
    if (lis2ds12_enable_hp_filtering(m_lis2ds12, filter))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_enable_hp_filtering() failed");
}

void LIS2DS12::enableInterruptLatching(bool latch)
{
    if (lis2ds12_enable_interrupt_latching(m_lis2ds12, latch))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_enable_interrupt_latching() failed");
}

void LIS2DS12::setInterruptActiveHigh(bool high)
{
    if (lis2ds12_set_interrupt_active_high(m_lis2ds12, high))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_set_interrupt_active_high() failed");
}

void LIS2DS12::setInterruptPushPull(bool pp)
{
    if (lis2ds12_set_interrupt_push_pull(m_lis2ds12, pp))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_set_interrupt_push_pull() failed");
}

void LIS2DS12::setInt1Config(uint8_t cfg)
{
    if (lis2ds12_set_int1_config(m_lis2ds12, cfg))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_set_int1_config() failed");
}

void LIS2DS12::setInt2Config(uint8_t cfg)
{
    if (lis2ds12_set_int2_config(m_lis2ds12, cfg))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_set_int2_config() failed");
}

uint8_t LIS2DS12::getStatus()
{
    return lis2ds12_get_status(m_lis2ds12);
}

void LIS2DS12::installISR(LIS2DS12_INTERRUPT_PINS_T intr, int gpio,
                          mraa::Edge level,
                          void (*isr)(void *), void *arg)
{
    if (lis2ds12_install_isr(m_lis2ds12, intr, gpio,
                             (mraa_gpio_edge_t)level, isr, arg))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lis2ds12_install_isr() failed");
}

void LIS2DS12::uninstallISR(LIS2DS12_INTERRUPT_PINS_T intr)
{
    lis2ds12_uninstall_isr(m_lis2ds12, intr);
}
