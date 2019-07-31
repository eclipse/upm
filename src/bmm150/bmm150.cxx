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
#include <string.h>

#include "upm_string_parser.hpp"
#include "upm_utilities.h"
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

BMM150::BMM150(std::string initStr) : mraaIo(initStr)
{
    mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
    std::vector<std::string> upmTokens;

    if(!mraaIo.getLeftoverStr().empty()) {
        upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
    }

    m_bmm150 = (bmm150_context)malloc(sizeof(struct _bmm150_context));
    if(!m_bmm150) {
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": bmm150_init() failed");
    }

    // zero out context
    memset((void *)m_bmm150, 0, sizeof(struct _bmm150_context));

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        bmm150_close(m_bmm150);
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": mraa_init() failed");
    }

    if(descs->spis) {
        m_bmm150->isSPI = true;
        if( !(m_bmm150->spi = descs->spis[0]) ) {
            bmm150_close(m_bmm150);
            throw std::runtime_error(std::string(__FUNCTION__)
                                     + ": mraa_spi_init() failed");
        }
        // Only create cs context if we are actually using a valid pin.
        // A hardware controlled pin should specify cs as -1.
        if(descs->gpios) {
            if( !(m_bmm150->gpioCS = descs->gpios[0]) ) {
                bmm150_close(m_bmm150);
                throw std::runtime_error(std::string(__FUNCTION__)
                                         + ": mraa_gpio_init() failed");
            }
            mraa_gpio_dir(m_bmm150->gpioCS, MRAA_GPIO_OUT);
        } else {
            throw std::runtime_error(std::string(__FUNCTION__)
                                     + ": mraa_gpio_init() failed");
        }
        mraa_spi_mode(m_bmm150->spi, MRAA_SPI_MODE0);
        if (mraa_spi_frequency(m_bmm150->spi, 5000000)) {
            bmm150_close(m_bmm150);
            throw std::runtime_error(std::string(__FUNCTION__)
                                     + ": mraa_spi_frequency() failed");
        }
    } else {
        // init the i2c context
        if(!descs->i2cs) {
            throw std::runtime_error(std::string(__FUNCTION__)
                                     + ": mraa_i2c_init() failed");
        } else {
            if( !(m_bmm150->i2c = descs->i2cs[0]) )
            {
                bmm150_close(m_bmm150);
                throw std::runtime_error(std::string(__FUNCTION__)
                                         + ": mraa_i2c_init() failed");
            }
        }
    }

    // power bit must be on for chip ID to be accessible
    bmm150_set_power_bit(m_bmm150, true);

    // not really, just need to set it to something valid until
    // bmm150_set_opmode() is called in bmm150_devinit().
    m_bmm150->opmode = BMM150_OPERATION_MODE_SLEEP;

    upm_delay_ms(50);

    // check the chip id

    uint8_t chipID = bmm150_get_chip_id(m_bmm150);

    if (chipID != BMM150_DEFAULT_CHIPID)
    {
        printf("%s: invalid chip id: %02x.  Expected %02x\n",
               __FUNCTION__, chipID, BMM150_DEFAULT_CHIPID);
        bmm150_close(m_bmm150);
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": bmm150_init() failed");
    }

    // call devinit with a default high resolution mode
    if (bmm150_devinit(m_bmm150, BMM150_USAGE_HIGH_ACCURACY))
    {
        printf("%s: bmm150_devinit() failed.\n", __FUNCTION__);
        bmm150_close(m_bmm150);
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": bmm150_init() failed");
    }

    std::string::size_type sz;
    for(std::string tok:upmTokens) {
        if(tok.substr(0, 5) == "init:") {
            BMM150_USAGE_PRESETS_T usage = (BMM150_USAGE_PRESETS_T)std::stoi(tok.substr(5), nullptr, 0);
            init(usage);
        }
        if(tok.substr(0, 9) == "writeReg:") {
            uint8_t reg = std::stoul(tok.substr(9), &sz, 0);
            tok = tok.substr(9);
            uint8_t val = std::stoul(tok.substr(sz+1), nullptr, 0);
            writeReg(reg, val);
        }
        if(tok.substr(0, 18) == "setOutputDataRate:") {
            BMM150_DATA_RATE_T odr = (BMM150_DATA_RATE_T)std::stoi(tok.substr(18), nullptr, 0);
            setOutputDataRate(odr);
        }
        if(tok.substr(0, 12) == "setPowerBit:") {
            bool power = std::stoi(tok.substr(12), &sz, 0);
            setPowerBit(power);
        }

        if(tok.substr(0, 10) == "setOpmode:") {
            BMM150_OPERATION_MODE_T opmode = (BMM150_OPERATION_MODE_T)std::stoi(tok.substr(10), nullptr, 0);
            setOpmode(opmode);
        }
        if(tok.substr(0, 19) == "setInterruptEnable:") {
            u_int8_t bits = (u_int8_t)std::stoul(tok.substr(19), nullptr, 0);
            setInterruptEnable(bits);
        }
        if(tok.substr(0, 19) == "setInterruptConfig:") {
            u_int8_t bits = (u_int8_t)std::stoul(tok.substr(19), nullptr, 0);
            setInterruptConfig(bits);
        }
        if(tok.substr(0, 17) == "setRepetitionsXY:") {
            u_int8_t reps = (u_int8_t)std::stoul(tok.substr(17), nullptr, 0);
            setRepetitionsXY(reps);
        }
        if(tok.substr(0, 16) == "setRepetitionsZ:") {
            u_int8_t reps = (u_int8_t)std::stoul(tok.substr(16), nullptr, 0);
            setRepetitionsZ(reps);
        }
        if(tok.substr(0, 14) == "setPresetMode:") {
            BMM150_USAGE_PRESETS_T usage = (BMM150_USAGE_PRESETS_T)std::stoi(tok.substr(14), nullptr, 0);
            setPresetMode(usage);
        }
    }
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
    update();
    float v[3];

    bmm150_get_magnetometer(m_bmm150, &v[0], &v[1], &v[2]);
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
