/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string.h>

#include "bmm150.hpp"

using namespace upm;
using namespace std;

BMM150::BMM150(int bus, int addr, int cs) :
    m_bmm150(bmm150_init(bus, addr, cs))
{
    if (!m_bmm150)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_init() failed");
}

BMM150::~BMM150()
{
    bmm150_close(m_bmm150);
}

void BMM150::init(BMM150_USAGE_PRESETS_T usage)
{
    if (bmm150_devinit(m_bmm150, usage))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_devinit() failed");
}

void BMM150::update()
{
    if (bmm150_update(m_bmm150))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_update() failed");
}

uint8_t BMM150::readReg(uint8_t reg)
{
    return bmm150_read_reg(m_bmm150, reg);
}

int BMM150::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
    int rv = bmm150_read_regs(m_bmm150, reg, buffer, len);
    if (rv < 0)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_read_regs() failed");

    return rv;
}

void BMM150::writeReg(uint8_t reg, uint8_t val)
{
    if (bmm150_write_reg(m_bmm150, reg, val))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_write_reg() failed");
}

uint8_t BMM150::getChipID()
{
    return bmm150_get_chip_id(m_bmm150);
}

void BMM150::getMagnetometer(float *x, float *y, float *z)
{
    bmm150_get_magnetometer(m_bmm150, x, y, z);
}

std::vector<float> BMM150::getMagnetometer()
{
    float v[3];

    getMagnetometer(&v[0], &v[1], &v[2]);
    return std::vector<float>(v, v+3);
}

void BMM150::reset()
{
    if (bmm150_reset(m_bmm150))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_reset() failed");
}

void BMM150::setOutputDataRate(BMM150_DATA_RATE_T odr)
{
    if (bmm150_set_output_data_rate(m_bmm150, odr))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_set_output_data_rate() failed");
}

void BMM150::setPowerBit(bool power)
{
    if (bmm150_set_power_bit(m_bmm150, power))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_set_set_power_bit() failed");
}

void BMM150::setOpmode(BMM150_OPERATION_MODE_T opmode)
{
    if (bmm150_set_opmode(m_bmm150, opmode))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_set_opmode() failed");
}

BMM150_OPERATION_MODE_T BMM150::getOpmode()
{
    return bmm150_get_opmode(m_bmm150);
}

uint8_t BMM150::getInterruptEnable()
{
    return bmm150_get_interrupt_enable(m_bmm150);
}

void BMM150::setInterruptEnable(uint8_t bits)
{
    if (bmm150_set_interrupt_enable(m_bmm150, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_set_interrupt_enable() failed");
}

uint8_t BMM150::getInterruptConfig()
{
    return bmm150_get_interrupt_config(m_bmm150);
}

void BMM150::setInterruptConfig(uint8_t bits)
{
    if (bmm150_set_interrupt_config(m_bmm150, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_set_interrupt_config() failed");
}

uint8_t BMM150::getInterruptStatus()
{
    return bmm150_get_interrupt_status(m_bmm150);
}

void BMM150::setRepetitionsXY(uint8_t reps)
{
    if (bmm150_set_repetitions_xy(m_bmm150, reps))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_set_repetitions_xy() failed");
}

void BMM150::setRepetitionsZ(uint8_t reps)
{
    if (bmm150_set_repetitions_z(m_bmm150, reps))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_set_repetitions_z() failed");
}

void BMM150::setPresetMode(BMM150_USAGE_PRESETS_T usage)
{
    if (bmm150_set_preset_mode(m_bmm150, usage))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_set_preset_mode() failed");
}

void BMM150::installISR(BMM150_INTERRUPT_PINS_T intr, int gpio,
                        mraa::Edge level,
                        void (*isr)(void *), void *arg)
{
    if (bmm150_install_isr(m_bmm150, intr, gpio, (mraa_gpio_edge_t) level,
                           isr, arg))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmm150_install_isr() failed");
}

void BMM150::uninstallISR(BMM150_INTERRUPT_PINS_T intr)
{
    bmm150_uninstall_isr(m_bmm150, intr);
}
