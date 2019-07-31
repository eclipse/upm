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

#include "bma250e.hpp"
#include "upm_string_parser.hpp"

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

BMA250E::BMA250E(std::string initStr) : mraaIo(initStr)
{
    mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
    std::vector<std::string> upmTokens;

    if(!mraaIo.getLeftoverStr().empty()) {
        upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
    }

    m_bma250e = (bma250e_context)malloc(sizeof(struct _bma250e_context));
    if(!m_bma250e) {
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": bma250e_init() failed");
    }

    // zero out context
    memset((void *)m_bma250e, 0, sizeof(struct _bma250e_context));

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        bma250e_close(m_bma250e);
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": mraa_init() failed");
    }

    if(descs->spis) {
        m_bma250e->isSPI = true;
        if( !(m_bma250e->spi = descs->spis[0]) ) {
            bma250e_close(m_bma250e);
            throw std::runtime_error(std::string(__FUNCTION__)
                                     + ": mraa_spi_init() failed");
        }
        if(descs->gpios) {
            if( !(m_bma250e->gpioCS = descs->gpios[0]) ) {
                bma250e_close(m_bma250e);
                throw std::runtime_error(std::string(__FUNCTION__)
                                         + ": mraa_gpio_init() failed");
            }
            mraa_gpio_dir(m_bma250e->gpioCS, MRAA_GPIO_OUT);
        } else {
            throw std::runtime_error(std::string(__FUNCTION__)
                                     + ": mraa_gpio_init() failed");
        }
        mraa_spi_mode(m_bma250e->spi, MRAA_SPI_MODE0);
        if (mraa_spi_frequency(m_bma250e->spi, 5000000)) {
            bma250e_close(m_bma250e);
            throw std::runtime_error(std::string(__FUNCTION__)
                                     + ": mraa_spi_frequency() failed");
        }
    } else {
        // init the i2c context
        if(!descs->i2cs) {
            throw std::runtime_error(std::string(__FUNCTION__)
                                     + ": mraa_i2c_init() failed");
        } else {
            if( !(m_bma250e->i2c = descs->i2cs[0]) )
            {
                bma250e_close(m_bma250e);
                throw std::runtime_error(std::string(__FUNCTION__)
                                         + ": mraa_i2c_init() failed");
            }
        }
    }

    // check the chip id

    uint8_t chipID = bma250e_get_chip_id(m_bma250e);

    // check the various chips id's and set appropriate capabilities.
    // Bail if the chip id is unknown.
    switch (chipID)
    {
    case 0xf9: // standalone bma250e
        m_bma250e->resolution = BMA250E_RESOLUTION_10BITS;
        m_bma250e->fifoAvailable = true;

        break;

    case 0xfa: // bmx055, bmi055 variants, 12b resolution
        m_bma250e->resolution = BMA250E_RESOLUTION_12BITS;
        m_bma250e->fifoAvailable = true;

        break;

    case 0x03: // bmc050 variant, no FIFO, 12b resolution
        m_bma250e->resolution = BMA250E_RESOLUTION_12BITS;
        m_bma250e->fifoAvailable = false;

        break;

    default:
        printf("%s: invalid chip id: %02x.  Expected f9, fa, or 03\n",
               __FUNCTION__, chipID);
        bma250e_close(m_bma250e);
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": bma250e_init() failed");
    }

    // call devinit with default options
    if (bma250e_devinit(m_bma250e, BMA250E_POWER_MODE_NORMAL, BMA250E_RANGE_2G,
                        BMA250E_BW_250))
    {
        printf("%s: bma250e_devinit() failed.\n", __FUNCTION__);
        bma250e_close(m_bma250e);
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": bma250e_init() failed");
    }

    std::string::size_type sz;
    for(std::string tok : upmTokens) {
        if(tok.substr(0, 11) == "enableFIFO:") {
            bool useFIFO = std::stoi(tok.substr(11), &sz, 0);
            enableFIFO(useFIFO);
        }
        if(tok.substr(0, 9) == "writeReg:") {
            uint8_t reg = std::stoul(tok.substr(9), &sz, 0);
            tok = tok.substr(9);
            uint8_t val = std::stoul(tok.substr(sz+1), nullptr, 0);
            writeReg(reg, val);
        }
        if(tok.substr(0, 9) == "setRange:") {
            BMA250E_RANGE_T scale = (BMA250E_RANGE_T)std::stoi(tok.substr(9), nullptr, 0);
            setRange(scale);
        }
        if(tok.substr(0, 13) == "setBandwidth:") {
            BMA250E_BW_T bw = (BMA250E_BW_T)std::stoi(tok.substr(13), nullptr, 0);
            setBandwidth(bw);
        }
        if(tok.substr(0, 13) == "setPowerMode:") {
            BMA250E_POWER_MODE_T power = (BMA250E_POWER_MODE_T)std::stoi(tok.substr(13), nullptr, 0);
            setPowerMode(power);
        }
        if(tok.substr(0, 17) == "fifoSetWatermark:") {
            BMA250E_RANGE_T wm = (BMA250E_RANGE_T)std::stoi(tok.substr(17), nullptr, 0);
            fifoSetWatermark(wm);
        }
        if(tok.substr(0, 11) == "fifoConfig:") {
            BMA250E_FIFO_MODE_T mode = (BMA250E_FIFO_MODE_T)std::stoi(tok.substr(11), &sz, 0);
            tok = tok.substr(11);
            BMA250E_FIFO_DATA_SEL_T axes = (BMA250E_FIFO_DATA_SEL_T)std::stoi(tok.substr(sz+1), nullptr, 0);
            fifoConfig(mode, axes);
        }
        if(tok.substr(0, 20) == "setInterruptEnable0:") {
            u_int8_t bits = (u_int8_t)std::stoul(tok.substr(20), nullptr, 0);
            setInterruptEnable0(bits);
        }
        if(tok.substr(0, 20) == "setInterruptEnable1:") {
            u_int8_t bits = (u_int8_t)std::stoul(tok.substr(20), nullptr, 0);
            setInterruptEnable1(bits);
        }
        if(tok.substr(0, 20) == "setInterruptEnable2:") {
            u_int8_t bits = (u_int8_t)std::stoul(tok.substr(20), nullptr, 0);
            setInterruptEnable2(bits);
        }
        if(tok.substr(0, 17) == "setInterruptMap0:") {
            u_int8_t bits = (u_int8_t)std::stoul(tok.substr(17), nullptr, 0);
            setInterruptMap0(bits);
        }
        if(tok.substr(0, 17) == "setInterruptMap1:") {
            u_int8_t bits = (u_int8_t)std::stoul(tok.substr(17), nullptr, 0);
            setInterruptMap1(bits);
        }
        if(tok.substr(0, 17) == "setInterruptMap2:") {
            u_int8_t bits = (u_int8_t)std::stoul(tok.substr(17), nullptr, 0);
            setInterruptMap2(bits);
        }
        if(tok.substr(0, 16) == "setInterruptSrc:") {
            u_int8_t bits = (u_int8_t)std::stoul(tok.substr(16), nullptr, 0);
            setInterruptSrc(bits);
        }
        if(tok.substr(0, 26) == "setInterruptOutputControl:") {
            u_int8_t bits = (u_int8_t)std::stoul(tok.substr(26), nullptr, 0);
            setInterruptOutputControl(bits);
        }
        if(tok.substr(0, 26) == "setInterruptLatchBehavior:") {
            BMA250E_RST_LATCH_T latch = (BMA250E_RST_LATCH_T)std::stoi(tok.substr(26), nullptr, 0);
            setInterruptLatchBehavior(latch);
        }
        if(tok.substr(0, 24) == "enableRegisterShadowing:") {
            bool shadow = std::stoi(tok.substr(24), nullptr, 0);
            enableRegisterShadowing(shadow);
        }
        if(tok.substr(0, 22) == "enableOutputFiltering:") {
            bool filter = std::stoi(tok.substr(22), nullptr, 0);
            enableOutputFiltering(filter);
        }
    }
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

std::vector<float> BMA250E::getAcceleration()
{
    std::vector<float> v(3);

    bma250e_get_accelerometer(m_bma250e, &v[0], &v[1], &v[2]);
    return v;
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
