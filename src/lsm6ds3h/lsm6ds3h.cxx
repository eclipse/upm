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

#include "lsm6ds3h.hpp"

using namespace upm;
using namespace std;

// conversion from Celsius to Fahrenheit

static float c2f(float c)
{
    return (c * (9.0 / 5.0) + 32.0);
}

LSM6DS3H::LSM6DS3H(int bus, int addr, int cs) :
    m_lsm6ds3h(lsm6ds3h_init(bus, addr, cs))
{
    if (!m_lsm6ds3h)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6ds3h_init() failed");
}

LSM6DS3H::~LSM6DS3H()
{
    lsm6ds3h_close(m_lsm6ds3h);
}

void LSM6DS3H::init(LSM6DS3H_XL_ODR_T acc_odr, LSM6DS3H_XL_FS_T acc_fs,
                    LSM6DS3H_G_ODR_T gyr_odr,  LSM6DS3H_G_FS_T gyr_fs)
{
    if (lsm6ds3h_devinit(m_lsm6ds3h, acc_odr, acc_fs, gyr_odr, gyr_fs))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6ds3h_devinit() failed");
}

void LSM6DS3H::update()
{
    if (lsm6ds3h_update(m_lsm6ds3h))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6ds3h_update() failed");
}

uint8_t LSM6DS3H::readReg(uint8_t reg)
{
    return lsm6ds3h_read_reg(m_lsm6ds3h, reg);
}

int LSM6DS3H::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
    int rv = lsm6ds3h_read_regs(m_lsm6ds3h, reg, buffer, len);
    if (rv != len)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6ds3h_read_regs() failed");

    return rv;
}

void LSM6DS3H::writeReg(uint8_t reg, uint8_t val)
{
    if (lsm6ds3h_write_reg(m_lsm6ds3h, reg, val))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6ds3h_write_reg() failed");
}

uint8_t LSM6DS3H::getChipID()
{
    return lsm6ds3h_get_chip_id(m_lsm6ds3h);
}

void LSM6DS3H::getAccelerometer(float *x, float *y, float *z)
{
    lsm6ds3h_get_accelerometer(m_lsm6ds3h, x, y, z);
}

std::vector<float> LSM6DS3H::getAccelerometer()
{
    float v[3];

    getAccelerometer(&v[0], &v[1], &v[2]);
    return std::vector<float>(v, v+3);
}

std::vector<float> LSM6DS3H::getAcceleration()
{
    std::vector<float> v(3);

    lsm6ds3h_get_accelerometer(m_lsm6ds3h, &v[0], &v[1], &v[2]);
    return v;
}

void LSM6DS3H::getGyroscope(float *x, float *y, float *z)
{
    lsm6ds3h_get_gyroscope(m_lsm6ds3h, x, y, z);
}

std::vector<float> LSM6DS3H::getGyroscope()
{
    update();
    float v[3];

    getGyroscope(&v[0], &v[1], &v[2]);
    return std::vector<float>(v, v+3);
}

float LSM6DS3H::getTemperature(bool fahrenheit)
{
    float temperature = lsm6ds3h_get_temperature(m_lsm6ds3h);
    if (fahrenheit)
        return c2f(temperature);
    else
        return temperature;
}

void LSM6DS3H::reset()
{
    if (lsm6ds3h_reset(m_lsm6ds3h))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6ds3h_reset() failed");
}

void LSM6DS3H::setAccelerometerODR(LSM6DS3H_XL_ODR_T odr)
{
    if (lsm6ds3h_set_acc_odr(m_lsm6ds3h, odr))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6ds3h_set_acc_odr() failed");
}

void LSM6DS3H::setAccelerometerFullScale(LSM6DS3H_XL_FS_T fs)
{
    if (lsm6ds3h_set_acc_full_scale(m_lsm6ds3h, fs))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6ds3h_set_acc_full_scale() failed");
}

void LSM6DS3H::setGyroscopeODR(LSM6DS3H_G_ODR_T odr)
{
    if (lsm6ds3h_set_gyr_odr(m_lsm6ds3h, odr))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6ds3h_set_gyr_odr() failed");
}

void LSM6DS3H::setGyroscopeFullScale(LSM6DS3H_G_FS_T fs)
{
    if (lsm6ds3h_set_gyr_full_scale(m_lsm6ds3h, fs))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6ds3h_set_gyr_full_scale() failed");
}

void LSM6DS3H::setHighPerformance(bool enable)
{
    if (lsm6ds3h_high_performance(m_lsm6ds3h, enable))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6ds3h_high_performance() failed");
}

void LSM6DS3H::setInterruptActiveHigh(bool high)
{
    if (lsm6ds3h_set_interrupt_active_high(m_lsm6ds3h, high))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6ds3h_set_interrupt_active_high() failed");
}

void LSM6DS3H::setInterruptPushPull(bool pp)
{
    if (lsm6ds3h_set_interrupt_push_pull(m_lsm6ds3h, pp))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6ds3h_set_interrupt_push_pull() failed");
}

uint8_t LSM6DS3H::getStatus()
{
    return lsm6ds3h_get_status(m_lsm6ds3h);
}

void LSM6DS3H::installISR(LSM6DS3H_INTERRUPT_PINS_T intr, int gpio,
                          mraa::Edge level,
                          void (*isr)(void *), void *arg)
{
    if (lsm6ds3h_install_isr(m_lsm6ds3h, intr, gpio,
                             (mraa_gpio_edge_t)level, isr, arg))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6ds3h_install_isr() failed");
}

void LSM6DS3H::uninstallISR(LSM6DS3H_INTERRUPT_PINS_T intr)
{
    lsm6ds3h_uninstall_isr(m_lsm6ds3h, intr);
}
