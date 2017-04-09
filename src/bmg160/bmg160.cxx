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

#include "bmg160.hpp"

using namespace upm;
using namespace std;

// conversion from Celsius to Fahrenheit

static float c2f(float c)
{
    return (c * (9.0 / 5.0) + 32.0);
}

BMG160::BMG160(int bus, int addr, int cs) :
    m_bmg160(bmg160_init(bus, addr, cs))
{
    if (!m_bmg160)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_init() failed");
}

BMG160::~BMG160()
{
    bmg160_close(m_bmg160);
}

void BMG160::init(BMG160_POWER_MODE_T pwr, BMG160_RANGE_T range,
                  BMG160_BW_T bw)
{
    if (bmg160_devinit(m_bmg160, pwr, range, bw))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_devinit() failed");
}

void BMG160::update()
{
    if (bmg160_update(m_bmg160))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_update() failed");
}

void BMG160::enableFIFO(bool useFIFO)
{
    bmg160_enable_fifo(m_bmg160, useFIFO);
}

uint8_t BMG160::readReg(uint8_t reg)
{
    return bmg160_read_reg(m_bmg160, reg);
}

int BMG160::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
    int rv = bmg160_read_regs(m_bmg160, reg, buffer, len);
    if (rv < 0)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_read_regs() failed");

    return rv;
}

void BMG160::writeReg(uint8_t reg, uint8_t val)
{
    if (bmg160_write_reg(m_bmg160, reg, val))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_write_reg() failed");
}

uint8_t BMG160::getChipID()
{
    return bmg160_get_chip_id(m_bmg160);
}

void BMG160::getGyroscope(float *x, float *y, float *z)
{
    bmg160_get_gyroscope(m_bmg160, x, y, z);
}

std::vector<float> BMG160::getGyroscope()
{
    float v[3];

    getGyroscope(&v[0], &v[1], &v[2]);
    return std::vector<float>(v, v+3);
}

float BMG160::getTemperature(bool fahrenheit)
{
    float temperature = bmg160_get_temperature(m_bmg160);
    if (fahrenheit)
        return c2f(temperature);
    else
        return temperature;
}

void BMG160::reset()
{
    if (bmg160_reset(m_bmg160))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_reset() failed");
}

void BMG160::setRange(BMG160_RANGE_T range)
{
    if (bmg160_set_range(m_bmg160, range))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_set_range() failed");
}

void BMG160::setBandwidth(BMG160_BW_T bw)
{
    if (bmg160_set_bandwidth(m_bmg160, bw))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_set_bandwidth() failed");
}

void BMG160::setPowerMode(BMG160_POWER_MODE_T power)
{
    if (bmg160_set_power_mode(m_bmg160, power))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_set_power_mode() failed");
}

void BMG160::fifoSetWatermark(int wm)
{
    if (bmg160_fifo_set_watermark(m_bmg160, wm))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_fifo_set_watermark() failed");
}

void BMG160::fifoConfig(BMG160_FIFO_MODE_T mode, BMG160_FIFO_DATA_SEL_T axes)
{
    if (bmg160_fifo_config(m_bmg160, mode, axes))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_fifo_config() failed");
}

uint8_t BMG160::getInterruptEnable0()
{
    return bmg160_get_interrupt_enable0(m_bmg160);
}

void BMG160::setInterruptEnable0(uint8_t bits)
{
    if (bmg160_set_interrupt_enable0(m_bmg160, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_set_interrupt_enable0() failed");
}

uint8_t BMG160::getInterruptMap0()
{
    return bmg160_get_interrupt_map0(m_bmg160);
}

void BMG160::setInterruptMap0(uint8_t bits)
{
    if (bmg160_set_interrupt_map0(m_bmg160, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_set_interrupt_map0() failed");
}

uint8_t BMG160::getInterruptMap1()
{
    return bmg160_get_interrupt_map1(m_bmg160);
}

void BMG160::setInterruptMap1(uint8_t bits)
{
    if (bmg160_set_interrupt_map1(m_bmg160, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_set_interrupt_map1() failed");
}

uint8_t BMG160::getInterruptSrc()
{
    return bmg160_get_interrupt_src(m_bmg160);
}

void BMG160::setInterruptSrc(uint8_t bits)
{
    if (bmg160_set_interrupt_src(m_bmg160, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_set_interrupt_src() failed");
}

uint8_t BMG160::getInterruptOutputControl()
{
    return bmg160_get_interrupt_output_control(m_bmg160);
}

void BMG160::setInterruptOutputControl(uint8_t bits)
{
    if (bmg160_set_interrupt_output_control(m_bmg160, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_set_interrupt_output_control() failed");
}

void BMG160::clearInterruptLatches()
{
    if (bmg160_clear_interrupt_latches(m_bmg160))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_clear_interrupt_latches() failed");
}

BMG160_RST_LATCH_T BMG160::getInterruptLatchBehavior()
{
    return bmg160_get_interrupt_latch_behavior(m_bmg160);
}

void BMG160::setInterruptLatchBehavior(BMG160_RST_LATCH_T latch)
{
    if (bmg160_set_interrupt_latch_behavior(m_bmg160, latch))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_set_interrupt_latch_behavior() failed");
}

void BMG160::enableRegisterShadowing(bool shadow)
{
    if (bmg160_enable_register_shadowing(m_bmg160, shadow))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_enable_register_shadowing() failed");
}

void BMG160::enableOutputFiltering(bool filter)
{
    if (bmg160_enable_output_filtering(m_bmg160, filter))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_enable_output_filtering() failed");
}

uint8_t BMG160::getInterruptStatus0()
{
    return bmg160_get_interrupt_status0(m_bmg160);
}

uint8_t BMG160::getInterruptStatus1()
{
    return bmg160_get_interrupt_status1(m_bmg160);
}

uint8_t BMG160::getInterruptStatus2()
{
    return bmg160_get_interrupt_status2(m_bmg160);
}

uint8_t BMG160::getInterruptStatus3()
{
    return bmg160_get_interrupt_status3(m_bmg160);
}

void BMG160::installISR(BMG160_INTERRUPT_PINS_T intr, int gpio,
                        mraa::Edge level,
                        void (*isr)(void *), void *arg)
{
    if (bmg160_install_isr(m_bmg160, intr, gpio,
                           (mraa_gpio_edge_t)level, isr, arg))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmg160_install_isr() failed");
}

void BMG160::uninstallISR(BMG160_INTERRUPT_PINS_T intr)
{
    bmg160_uninstall_isr(m_bmg160, intr);
}
