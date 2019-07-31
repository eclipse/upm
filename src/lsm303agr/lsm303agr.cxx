/*
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

#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <string.h>

#include "lsm303agr.hpp"

using namespace upm;
using namespace std;

LSM303AGR::LSM303AGR(int bus, int acc_addr, int mag_addr) :
    m_lsm303agr(lsm303agr_init(bus, acc_addr, mag_addr))
{
    if (!m_lsm303agr)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303agr_init() failed");
}

LSM303AGR::~LSM303AGR()
{
    lsm303agr_close(m_lsm303agr);
}

void LSM303AGR::init(LSM303AGR_POWER_MODE_T usage)
{
    if (lsm303agr_devinit(m_lsm303agr, usage))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303agr_devinit() failed");
}

void LSM303AGR::update()
{
    if (lsm303agr_update(m_lsm303agr))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303agr_update() failed");
}

uint8_t LSM303AGR::readReg(uint8_t reg)
{
    return lsm303agr_read_reg(m_lsm303agr, reg);
}

int LSM303AGR::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
    int rv = lsm303agr_read_regs(m_lsm303agr, reg, buffer, len);
    if (rv < 0)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303agr_read_regs() failed");

    return rv;
}

void LSM303AGR::writeReg(uint8_t reg, uint8_t val)
{
    if (lsm303agr_write_reg(m_lsm303agr, reg, val))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303agr_write_reg() failed");
}

void LSM303AGR::getMagnetometer(float *x, float *y, float *z)
{
    lsm303agr_get_magnetometer(m_lsm303agr, x, y, z);
}

std::vector<float> LSM303AGR::getMagnetometer()
{
    update();
    float v[3];

    lsm303agr_get_magnetometer(m_lsm303agr, &v[0], &v[1], &v[2]);
    return std::vector<float>(v, v+3);
}

void LSM303AGR::getAccelerometer(float *x, float *y, float *z)
{
    lsm303agr_get_accelerometer(m_lsm303agr, x, y, z);
}

std::vector<float> LSM303AGR::getAccelerometer()
{
    float v[3];

    getAccelerometer(&v[0], &v[1], &v[2]);
    return std::vector<float>(v, v+3);
}

std::vector<float> LSM303AGR::getAcceleration()
{
    std::vector<float> v(3);

    lsm303agr_get_accelerometer(m_lsm303agr, &v[0], &v[1], &v[2]);
    return v;
}

float LSM303AGR::getTemperature()
{
    return lsm303agr_get_temperature(m_lsm303agr);
}

void LSM303AGR::setFullScale(LSM303AGR_A_FS_T fs)
{
    if (lsm303agr_set_full_scale(m_lsm303agr, fs))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303agr_set_full_scale() failed");
}

void LSM303AGR::setPowerMode(LSM303AGR_POWER_MODE_T mode)
{
    if (lsm303agr_set_power_mode(m_lsm303agr, mode))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303agr_set_power_mode() failed");
}

void LSM303AGR::setAccelerometerODR(LSM303AGR_A_ODR_T odr)
{
    if (lsm303agr_set_acc_odr(m_lsm303agr, odr))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303agr_set_acc_odr() failed");
}

void LSM303AGR::setMagnetometerODR(LSM303AGR_CFG_A_M_ODR_T odr)
{
    if (lsm303agr_set_mag_odr(m_lsm303agr, odr))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303agr_set_mag_odr() failed");
}

uint8_t LSM303AGR::getAccelerometerInt1Config()
{
    return lsm303agr_get_acc_int1_config(m_lsm303agr);
}

uint8_t LSM303AGR::getAccelerometerInt2Config()
{
    return lsm303agr_get_acc_int2_config(m_lsm303agr);
}

void LSM303AGR::setAccelerometerInt1Config(uint8_t bits)
{
    if (lsm303agr_set_acc_int1_config(m_lsm303agr, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303agr_set_acc_int1_config() failed");
}

void LSM303AGR::setAccelerometerInt2Config(uint8_t bits)
{
    if (lsm303agr_set_acc_int2_config(m_lsm303agr, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303agr_set_acc_int2_config() failed");
}

uint8_t LSM303AGR::getMagnetometerIntConfig()
{
    return lsm303agr_get_mag_int_config(m_lsm303agr);
}

void LSM303AGR::setMagnetometerIntConfig(uint8_t bits)
{
    if (lsm303agr_set_mag_int_config(m_lsm303agr, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303agr_set_mag_int_config() failed");
}

uint8_t LSM303AGR::getAccelerometerInt1Src()
{
    return lsm303agr_get_acc_int1_src(m_lsm303agr);
}

uint8_t LSM303AGR::getAccelerometerInt2Src()
{
    return lsm303agr_get_acc_int2_src(m_lsm303agr);
}

uint8_t LSM303AGR::getMagnetometerIntSrc()
{
    return lsm303agr_get_mag_int_src(m_lsm303agr);
}


void LSM303AGR::installISR(LSM303AGR_INTERRUPT_PINS_T intr, int gpio,
                           mraa::Edge level,
                           void (*isr)(void *), void *arg)
{
    if (lsm303agr_install_isr(m_lsm303agr, intr, gpio, (mraa_gpio_edge_t)level,
                              isr, arg))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm303agr_install_isr() failed");
}

void LSM303AGR::uninstallISR(LSM303AGR_INTERRUPT_PINS_T intr)
{
    lsm303agr_uninstall_isr(m_lsm303agr, intr);
}
