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

#include "lsm6dsl.hpp"

using namespace upm;
using namespace std;

// conversion from Celsius to Fahrenheit

static float c2f(float c)
{
    return (c * (9.0 / 5.0) + 32.0);
}

LSM6DSL::LSM6DSL(int bus, int addr, int cs) :
    m_lsm6dsl(lsm6dsl_init(bus, addr, cs))
{
    if (!m_lsm6dsl)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_init() failed");
}

LSM6DSL::~LSM6DSL()
{
    lsm6dsl_close(m_lsm6dsl);
}

void LSM6DSL::init(LSM6DSL_XL_ODR_T acc_odr, LSM6DSL_XL_FS_T acc_fs,
                   LSM6DSL_G_ODR_T gyr_odr,  LSM6DSL_G_FS_T gyr_fs)
{
    if (lsm6dsl_devinit(m_lsm6dsl, acc_odr, acc_fs, gyr_odr, gyr_fs))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_devinit() failed");
}

void LSM6DSL::update()
{
    if (lsm6dsl_update(m_lsm6dsl))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_update() failed");
}

uint8_t LSM6DSL::readReg(uint8_t reg)
{
    return lsm6dsl_read_reg(m_lsm6dsl, reg);
}

int LSM6DSL::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
    int rv = lsm6dsl_read_regs(m_lsm6dsl, reg, buffer, len);
    if (rv != len)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_read_regs() failed");

    return rv;
}

void LSM6DSL::writeReg(uint8_t reg, uint8_t val)
{
    if (lsm6dsl_write_reg(m_lsm6dsl, reg, val))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_write_reg() failed");
}

uint8_t LSM6DSL::getChipID()
{
    return lsm6dsl_get_chip_id(m_lsm6dsl);
}

void LSM6DSL::getAccelerometer(float *x, float *y, float *z)
{
    lsm6dsl_get_accelerometer(m_lsm6dsl, x, y, z);
}

std::vector<float> LSM6DSL::getAccelerometer()
{
    float v[3];

    getAccelerometer(&v[0], &v[1], &v[2]);
    return std::vector<float>(v, v+3);
}

std::vector<float> LSM6DSL::getAcceleration()
{
    std::vector<float> v(3);

    lsm6dsl_get_accelerometer(m_lsm6dsl, &v[0], &v[1], &v[2]);

    return v;
}

void LSM6DSL::getGyroscope(float *x, float *y, float *z)
{
    lsm6dsl_get_gyroscope(m_lsm6dsl, x, y, z);
}

std::vector<float> LSM6DSL::getGyroscope()
{
    update();
    float v[3];

    getGyroscope(&v[0], &v[1], &v[2]);
    return std::vector<float>(v, v+3);
}

float LSM6DSL::getTemperature(bool fahrenheit)
{
    float temperature = lsm6dsl_get_temperature(m_lsm6dsl);
    if (fahrenheit)
        return c2f(temperature);
    else
        return temperature;
}

void LSM6DSL::reset()
{
    if (lsm6dsl_reset(m_lsm6dsl))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_reset() failed");
}

void LSM6DSL::setAccelerometerODR(LSM6DSL_XL_ODR_T odr)
{
    if (lsm6dsl_set_acc_odr(m_lsm6dsl, odr))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_set_acc_odr() failed");
}

void LSM6DSL::setAccelerometerFullScale(LSM6DSL_XL_FS_T fs)
{
    if (lsm6dsl_set_acc_full_scale(m_lsm6dsl, fs))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_set_acc_full_scale() failed");
}

void LSM6DSL::setGyroscopeODR(LSM6DSL_G_ODR_T odr)
{
    if (lsm6dsl_set_gyr_odr(m_lsm6dsl, odr))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_set_gyr_odr() failed");
}

void LSM6DSL::setGyroscopeFullScale(LSM6DSL_G_FS_T fs)
{
    if (lsm6dsl_set_gyr_full_scale(m_lsm6dsl, fs))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_set_gyr_full_scale() failed");
}

void LSM6DSL::setHighPerformance(bool enable)
{
    if (lsm6dsl_high_performance(m_lsm6dsl, enable))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_high_performance() failed");
}

void LSM6DSL::setInterruptActiveHigh(bool high)
{
    if (lsm6dsl_set_interrupt_active_high(m_lsm6dsl, high))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_set_interrupt_active_high() failed");
}

void LSM6DSL::setInterruptPushPull(bool pp)
{
    if (lsm6dsl_set_interrupt_push_pull(m_lsm6dsl, pp))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_set_interrupt_push_pull() failed");
}

uint8_t LSM6DSL::getStatus()
{
    return lsm6dsl_get_status(m_lsm6dsl);
}

void LSM6DSL::setAccelerometerOffsets(int x, int y, int z, bool weight)
{
    if (lsm6dsl_set_acc_offsets(m_lsm6dsl, x, y, z, weight))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_set_acc_offsets() failed");
}

void LSM6DSL::installISR(LSM6DSL_INTERRUPT_PINS_T intr, int gpio,
                         mraa::Edge level,
                         void (*isr)(void *), void *arg)
{
    if (lsm6dsl_install_isr(m_lsm6dsl, intr, gpio,
                            (mraa_gpio_edge_t)level, isr, arg))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lsm6dsl_install_isr() failed");
}

void LSM6DSL::uninstallISR(LSM6DSL_INTERRUPT_PINS_T intr)
{
    lsm6dsl_uninstall_isr(m_lsm6dsl, intr);
}
