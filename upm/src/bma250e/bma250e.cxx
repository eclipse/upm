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

#include "bma250e.hpp"

using namespace upm;
using namespace std;

// conversion from celsius to fahrenheit

static float c2f(float c)
{
    return (c * (9.0 / 5.0) + 32.0);
}

BMA250E::BMA250E(int bus, int addr, int cs) :
    m_bma250e(bma250e_init(bus, addr, cs))
{
    if (!m_bma250e)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_init() failed");
}

BMA250E::~BMA250E()
{
    bma250e_close(m_bma250e);
}

void BMA250E::init(BMA250E_POWER_MODE_T pwr, BMA250E_RANGE_T range,
                   BMA250E_BW_T bw)
{
    if (bma250e_devinit(m_bma250e, pwr, range, bw))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_devinit() failed");
}

void BMA250E::update()
{
    if (bma250e_update(m_bma250e))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_update() failed");
}

void BMA250E::enableFIFO(bool useFIFO)
{
    bma250e_enable_fifo(m_bma250e, useFIFO);
}

uint8_t BMA250E::readReg(uint8_t reg)
{
    return bma250e_read_reg(m_bma250e, reg);
}

int BMA250E::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
    int rv = bma250e_read_regs(m_bma250e, reg, buffer, len);
    if (rv < 0)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_read_regs() failed");

    return rv;
}

void BMA250E::writeReg(uint8_t reg, uint8_t val)
{
    if (bma250e_write_reg(m_bma250e, reg, val))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_write_reg() failed");
}

uint8_t BMA250E::getChipID()
{
    return bma250e_get_chip_id(m_bma250e);
}

void BMA250E::getAccelerometer(float *x, float *y, float *z)
{
    bma250e_get_accelerometer(m_bma250e, x, y, z);
}

std::vector<float> BMA250E::getAccelerometer()
{
    float v[3];

    getAccelerometer(&v[0], &v[1], &v[2]);
    return std::vector<float>(v, v+3);
}

float BMA250E::getTemperature(bool fahrenheit)
{
    float temperature = bma250e_get_temperature(m_bma250e);
    if (fahrenheit)
        return c2f(temperature);
    else
        return temperature;
}

void BMA250E::reset()
{
    if (bma250e_reset(m_bma250e))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_reset() failed");
}

void BMA250E::setRange(BMA250E_RANGE_T range)
{
    if (bma250e_set_range(m_bma250e, range))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_set_range() failed");
}

void BMA250E::setBandwidth(BMA250E_BW_T bw)
{
    if (bma250e_set_bandwidth(m_bma250e, bw))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_set_power_mode() failed");
}

void BMA250E::setPowerMode(BMA250E_POWER_MODE_T power)
{
    if (bma250e_set_power_mode(m_bma250e, power))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_set_power_mode() failed");
}

void BMA250E::fifoSetWatermark(int wm)
{
    if (bma250e_fifo_set_watermark(m_bma250e, wm))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_fifo_set_watermark() failed");
}

void BMA250E::fifoConfig(BMA250E_FIFO_MODE_T mode,
                         BMA250E_FIFO_DATA_SEL_T axes)
{
    if (bma250e_fifo_config(m_bma250e, mode, axes))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_fifo_config() failed");
}

void BMA250E::setSelfTest(bool sign, bool amp, BMA250E_SELFTTEST_AXIS_T axis)
{
    if (bma250e_set_self_test(m_bma250e, sign, amp, axis))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_fifo_config() failed");
}

uint8_t BMA250E::getInterruptEnable0()
{
    return bma250e_get_interrupt_enable0(m_bma250e);
}

void BMA250E::setInterruptEnable0(uint8_t bits)
{
    if (bma250e_set_interrupt_enable0(m_bma250e, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_set_interrupt_enable0() failed");
}

uint8_t BMA250E::getInterruptEnable1()
{
    return bma250e_get_interrupt_enable1(m_bma250e);
}

void BMA250E::setInterruptEnable1(uint8_t bits)
{
    if (bma250e_set_interrupt_enable1(m_bma250e, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_set_interrupt_enable1() failed");
}

uint8_t BMA250E::getInterruptEnable2()
{
    return bma250e_get_interrupt_enable2(m_bma250e);
}

void BMA250E::setInterruptEnable2(uint8_t bits)
{
    if (bma250e_set_interrupt_enable2(m_bma250e, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_set_interrupt_enable2() failed");
}

uint8_t BMA250E::getInterruptMap0()
{
    return bma250e_get_interrupt_map0(m_bma250e);
}

void BMA250E::setInterruptMap0(uint8_t bits)
{
    if (bma250e_set_interrupt_map0(m_bma250e, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_set_interrupt_map0() failed");
}

uint8_t BMA250E::getInterruptMap1()
{
    return bma250e_get_interrupt_map1(m_bma250e);
}

void BMA250E::setInterruptMap1(uint8_t bits)
{
    if (bma250e_set_interrupt_map1(m_bma250e, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_set_interrupt_map1() failed");
}

uint8_t BMA250E::getInterruptMap2()
{
    return bma250e_get_interrupt_map2(m_bma250e);
}

void BMA250E::setInterruptMap2(uint8_t bits)
{
    if (bma250e_set_interrupt_map2(m_bma250e, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_set_interrupt_map2() failed");
}

uint8_t BMA250E::getInterruptSrc()
{
    return bma250e_get_interrupt_src(m_bma250e);
}

void BMA250E::setInterruptSrc(uint8_t bits)
{
    if (bma250e_set_interrupt_src(m_bma250e, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_set_interrupt_src() failed");
}

uint8_t BMA250E::getInterruptOutputControl()
{
    return bma250e_get_interrupt_output_control(m_bma250e);
}

void BMA250E::setInterruptOutputControl(uint8_t bits)
{
    if (bma250e_set_interrupt_output_control(m_bma250e, bits))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_set_interrupt_output_control() failed");
}

void BMA250E::clearInterruptLatches()
{
    if (bma250e_clear_interrupt_latches(m_bma250e))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_clear_interrupt_latches() failed");
}

BMA250E_RST_LATCH_T BMA250E::getInterruptLatchBehavior()
{
    return bma250e_get_interrupt_latch_behavior(m_bma250e);
}

void BMA250E::setInterruptLatchBehavior(BMA250E_RST_LATCH_T latch)
{
    if (bma250e_set_interrupt_latch_behavior(m_bma250e, latch))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_set_interrupt_latch_behavior() failed");
}

void BMA250E::enableRegisterShadowing(bool shadow)
{
    if (bma250e_enable_register_shadowing(m_bma250e, shadow))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_enable_register_shadowing() failed");
}

void BMA250E::enableOutputFiltering(bool filter)
{
    if (bma250e_enable_output_filtering(m_bma250e, filter))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_enable_output_filtering() failed");
}

uint8_t BMA250E::getInterruptStatus0()
{
    return bma250e_get_interrupt_status0(m_bma250e);
}

uint8_t BMA250E::getInterruptStatus1()
{
    return bma250e_get_interrupt_status1(m_bma250e);
}

uint8_t BMA250E::getInterruptStatus2()
{
    return bma250e_get_interrupt_status2(m_bma250e);
}

uint8_t BMA250E::getInterruptStatus3Bits()
{
    return bma250e_get_interrupt_status3_bits(m_bma250e);
}

BMA250E_ORIENT_T BMA250E::getInterruptStatus3Orientation()
{
    return bma250e_get_interrupt_status3_orientation(m_bma250e);
}

void BMA250E::setLowPowerMode2()
{
    if (bma250e_set_low_power_mode2(m_bma250e))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_set_low_power_mode2() failed");
}

void BMA250E::installISR(BMA250E_INTERRUPT_PINS_T intr, int gpio,
                         mraa::Edge level,
                         void (*isr)(void *), void *arg)
{
    if (bma250e_install_isr(m_bma250e, intr, gpio,
                           (mraa_gpio_edge_t)level, isr, arg))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bma250e_install_isr() failed");
}

void BMA250E::uninstallISR(BMA250E_INTERRUPT_PINS_T intr)
{
    bma250e_uninstall_isr(m_bma250e, intr);
}
