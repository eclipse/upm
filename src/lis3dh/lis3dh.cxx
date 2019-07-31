/*
 * Author: Alex Tereschenko <alext.mkrs@gmail.com>
 * Copyright (c) 2018 Alex Tereschenko.
 *
 * Based on LIS2DS12 module by
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>

#include "lis3dh.hpp"

using namespace upm;
using namespace std;

// conversion from Celsius to Fahrenheit

static float
c2f(float c)
{
    return (c * (9.0 / 5.0) + 32.0);
}

LIS3DH::LIS3DH(int bus, int addr, int cs) : m_lis3dh(lis3dh_init(bus, addr, cs))
{
    if (!m_lis3dh) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_init() failed");
    }
}

LIS3DH::~LIS3DH()
{
    lis3dh_close(m_lis3dh);
}

void
LIS3DH::init(LIS3DH_ODR_T odr, LIS3DH_FS_T fs, bool high_res)
{
    if (lis3dh_devinit(m_lis3dh, odr, fs, high_res)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_devinit() failed");
    }
}

void
LIS3DH::enableAxes(bool x_axis_enable, bool y_axis_enable, bool z_axis_enable)
{
    if (lis3dh_enable_axes(m_lis3dh, x_axis_enable, y_axis_enable, z_axis_enable)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_enable_axes() failed");
    }
}

void
LIS3DH::enableBDUMode(bool bdu_enable)
{
    if (lis3dh_enable_bdu_mode(m_lis3dh, bdu_enable)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_enable_bdu_mode() failed");
    }
}

void
LIS3DH::enableLPMode(bool lp_enable)
{
    if (lis3dh_enable_lp_mode(m_lis3dh, lp_enable)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_enable_lp_mode() failed");
    }
}

void
LIS3DH::enableHRMode(bool hr_enable)
{
    if (lis3dh_enable_hr_mode(m_lis3dh, hr_enable)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_enable_hr_mode() failed");
    }
}

void
LIS3DH::enableNormalMode()
{
    if (lis3dh_enable_normal_mode(m_lis3dh)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_enable_normal_mode() failed");
    }
}

void
LIS3DH::enableHPFiltering(bool filter)
{
    if (lis3dh_enable_hp_filtering(m_lis3dh, filter)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_enable_hp_filtering() failed");
    }
}

void
LIS3DH::enableInterruptLatching(bool int1_latch, bool int2_latch)
{
    if (lis3dh_enable_interrupt_latching(m_lis3dh, int1_latch, int2_latch)) {
        throw std::runtime_error(string(__FUNCTION__) +
                                 ": lis3dh_enable_interrupt_latching() failed");
    }
}

void
LIS3DH::enableADC(bool adc_enable)
{
    if (lis3dh_enable_adc(m_lis3dh, adc_enable)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_enable_adc() failed");
    }
}

void
LIS3DH::enableTemperature(bool temperature_enable)
{
    if (lis3dh_enable_temperature(m_lis3dh, temperature_enable)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_enable_temperature() failed");
    }
}

void
LIS3DH::setODR(LIS3DH_ODR_T odr)
{
    if (lis3dh_set_odr(m_lis3dh, odr)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_set_odr() failed");
    }
}

void
LIS3DH::setFullScale(LIS3DH_FS_T fs)
{
    if (lis3dh_set_full_scale(m_lis3dh, fs)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_set_full_scale() failed");
    }
}

void
LIS3DH::setInterruptActiveHigh(bool high)
{
    if (lis3dh_set_interrupt_active_high(m_lis3dh, high)) {
        throw std::runtime_error(string(__FUNCTION__) +
                                 ": lis3dh_set_interrupt_active_high() failed");
    }
}

void
LIS3DH::setInt1Config(uint8_t cfg)
{
    if (lis3dh_set_int1_config(m_lis3dh, cfg)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_set_int1_config() failed");
    }
}

void
LIS3DH::setInt2Config(uint8_t cfg)
{
    if (lis3dh_set_int2_config(m_lis3dh, cfg)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_set_int2_config() failed");
    }
}

uint8_t
LIS3DH::readReg(uint8_t reg)
{
    return lis3dh_read_reg(m_lis3dh, reg);
}

int
LIS3DH::readRegs(uint8_t reg, uint8_t* buffer, int len)
{
    int rv = lis3dh_read_regs(m_lis3dh, reg, buffer, len);
    if (rv != len) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_read_regs() failed");
    }

    return rv;
}

void
LIS3DH::writeReg(uint8_t reg, uint8_t val)
{
    if (lis3dh_write_reg(m_lis3dh, reg, val)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_write_reg() failed");
    }
}

void
LIS3DH::update()
{
    if (lis3dh_update(m_lis3dh)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_update() failed");
    }
}

uint8_t
LIS3DH::getChipID()
{
    return lis3dh_get_chip_id(m_lis3dh);
}

void
LIS3DH::getAccelerometer(float* x, float* y, float* z)
{
    lis3dh_get_accelerometer(m_lis3dh, x, y, z);
}

std::vector<float>
LIS3DH::getAccelerometer()
{
    float v[3];

    getAccelerometer(&v[0], &v[1], &v[2]);
    return std::vector<float>(v, v + 3);
}

std::vector<float>
LIS3DH::getAcceleration()
{
    std::vector<float> v(3);

    lis3dh_get_accelerometer(m_lis3dh, &v[0], &v[1], &v[2]);
    return v;
}

float
LIS3DH::getTemperature(bool fahrenheit)
{
    float temperature = lis3dh_get_temperature(m_lis3dh);
    if (fahrenheit) {
        return c2f(temperature);
    } else {
        return temperature;
    }
}

uint8_t
LIS3DH::getStatus()
{
    return lis3dh_get_status(m_lis3dh);
}

uint8_t
LIS3DH::getStatusAux()
{
    return lis3dh_get_status_aux(m_lis3dh);
}

void
LIS3DH::installISR(LIS3DH_INTERRUPT_PINS_T intr,
                   int gpio,
                   mraa::Edge level,
                   void (*isr)(void*),
                   void* arg)
{
    if (lis3dh_install_isr(m_lis3dh, intr, gpio, (mraa_gpio_edge_t) level, isr, arg)) {
        throw std::runtime_error(string(__FUNCTION__) + ": lis3dh_install_isr() failed");
    }
}

void
LIS3DH::uninstallISR(LIS3DH_INTERRUPT_PINS_T intr)
{
    lis3dh_uninstall_isr(m_lis3dh, intr);
}
