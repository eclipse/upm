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

#include "bmp280.hpp"
#include "upm_string_parser.hpp"
#include "upm_utilities.h"

using namespace upm;
using namespace std;

// conversion from Celsius to Fahrenheit.

static float c2f(float c)
{
  return (c * (9.0 / 5.0) + 32.0);
}

BMP280::BMP280(int bus, int addr, int cs) :
    m_bmp280(bmp280_init(bus, addr, cs))
{
    if (!m_bmp280)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmp280_init() failed");
}

BMP280::BMP280(std::string initStr) : mraaIo(initStr)
{
    mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
    std::vector<std::string> upmTokens;

    if(!mraaIo.getLeftoverStr().empty()) {
        upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
    }

    m_bmp280 = (bmp280_context)malloc(sizeof(struct _bmp280_context));
    if(!m_bmp280) {
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": bmp280_init() failed");
    }

    // zero out context
    memset((void *)m_bmp280, 0, sizeof(struct _bmp280_context));

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        bmp280_close(m_bmp280);
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": mraa_init() failed");
    }

    if(descs->spis) {
        m_bmp280->isSPI = true;
        if( !(m_bmp280->spi = descs->spis[0]) ) {
            bmp280_close(m_bmp280);
            throw std::runtime_error(std::string(__FUNCTION__)
                                     + ": mraa_spi_init() failed");
        }
        if(descs->gpios) {
            if( !(m_bmp280->gpio = descs->gpios[0]) ) {
                bmp280_close(m_bmp280);
                throw std::runtime_error(std::string(__FUNCTION__)
                                         + ": mraa_gpio_init() failed");
            }
            mraa_gpio_dir(m_bmp280->gpio, MRAA_GPIO_OUT);
        } else {
            throw std::runtime_error(std::string(__FUNCTION__)
                                     + ": mraa_gpio_init() failed");
        }
        mraa_spi_mode(m_bmp280->spi, MRAA_SPI_MODE0);
        if (mraa_spi_frequency(m_bmp280->spi, 5000000)) {
            bmp280_close(m_bmp280);
            throw std::runtime_error(std::string(__FUNCTION__)
                                     + ": mraa_spi_frequency() failed");
        }
        // toggle CS on/off so chip switches into SPI mode.  For a hw
        // CS pin, the first SPI transaction should accomplish this.
        _csOn(m_bmp280);
        upm_delay_ms(10);
        _csOff(m_bmp280);
    } else {
        // init the i2c context
        if(!descs->i2cs) {
            throw std::runtime_error(std::string(__FUNCTION__)
                                     + ": mraa_i2c_init() failed");
        } else {
            if( !(m_bmp280->i2c = descs->i2cs[0]) )
            {
                bmp280_close(m_bmp280);
                throw std::runtime_error(std::string(__FUNCTION__)
                                         + ": mraa_i2c_init() failed");
            }
        }
    }
    // check the chip id

    uint8_t chipID = bmp280_read_reg(m_bmp280, BMP280_REG_CHIPID);

    switch(chipID)
    {
    case BMP280_CHIPID: // BMP280
        m_bmp280->isBME = false;
        break;

    case BME280_CHIPID: // BME280
        m_bmp280->isBME = true;
        break;

    default: // ??
        printf("%s: invalid chip id: %02x.  Expected either %02x "
               "(bmp280) or %02x (bme280)\n",
               __FUNCTION__, chipID, BMP280_CHIPID, BME280_CHIPID);
        bmp280_close(m_bmp280);
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": bmp280_init() failed");

    }

    // set sleep mode for now
    bmp280_set_measure_mode(m_bmp280, BMP280_MODE_SLEEP);

    // read calibration data
    if (_read_calibration_data(m_bmp280))
    {
        printf("%s: _read_calibration_data() failed.", __FUNCTION__);
        bmp280_close(m_bmp280);
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": bmp280_init() failed");
    }

    // set the default mode to the highest resolution mode
    bmp280_set_usage_mode(m_bmp280, BMP280_USAGE_MODE_INDOOR_NAV);

    // set the default sea level pressure in hPA
    m_bmp280->sea_level_hPA = BMP280_SEA_LEVEL_HPA;

    std::string::size_type sz;
    for(std::string tok:upmTokens) {
        if(tok.substr(0, 21) == "setSeaLevelPreassure:") {
            float seaLevelhPA = std::stof(tok.substr(21));
            setSeaLevelPreassure(seaLevelhPA);
        }
        if(tok.substr(0, 9) == "writeReg:") {
            uint8_t reg = std::stoul(tok.substr(9), &sz, 0);
            tok = tok.substr(9);
            uint8_t val = std::stoul(tok.substr(sz+1), nullptr, 0);
            writeReg(reg, val);
        }
        if(tok.substr(0, 10) == "setFilter:") {
            BMP280_FILTER_T filter = (BMP280_FILTER_T)std::stoi(tok.substr(10), nullptr, 0);
            setFilter(filter);
        }
        if(tok.substr(0, 16) == "setTimerStandby:") {
            BMP280_T_SB_T tsb = (BMP280_T_SB_T)std::stoi(tok.substr(16), nullptr, 0);
            setTimerStandby(tsb);
        }
        if(tok.substr(0, 15) == "setMeasureMode:") {
            BMP280_MODES_T mode = (BMP280_MODES_T)std::stoi(tok.substr(15), nullptr, 0);
            setMeasureMode(mode);
        }
        if(tok.substr(0, 26) == "setOversampleRatePressure:") {
            BMP280_OSRS_P_T rate = (BMP280_OSRS_P_T)std::stoi(tok.substr(26), nullptr, 0);
            setOversampleRatePressure(rate);
        }
        if(tok.substr(0, 29) == "setOversampleRateTemperature:") {
            BMP280_OSRS_T_T rate = (BMP280_OSRS_T_T)std::stoi(tok.substr(29), nullptr, 0);
            setOversampleRateTemperature(rate);
        }
        if(tok.substr(0, 13) == "setUsageMode:") {
            BMP280_USAGE_MODE_T mode = (BMP280_USAGE_MODE_T)std::stoi(tok.substr(13), nullptr, 0);
            setUsageMode(mode);
        }
    }
}

BMP280::~BMP280()
{
    bmp280_close(m_bmp280);
}

void BMP280::update()
{
    if (bmp280_update(m_bmp280))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmp280_update() failed");
}

void BMP280::setSeaLevelPreassure(float seaLevelhPA)
{
    bmp280_set_sea_level_pressure(m_bmp280, seaLevelhPA);
}

float BMP280::getAltitude()
{
    return bmp280_get_altitude(m_bmp280);
}

uint8_t BMP280::readReg(uint8_t reg)
{
    return bmp280_read_reg(m_bmp280, reg);
}

int BMP280::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
    return bmp280_read_regs(m_bmp280, reg, buffer, len);
}

void BMP280::writeReg(uint8_t reg, uint8_t val)
{
    if (bmp280_write_reg(m_bmp280, reg, val))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmp280_write_reg() failed");
}

uint8_t BMP280::getChipID()
{
    return bmp280_get_chip_id(m_bmp280);
}

void BMP280::reset()
{
    bmp280_reset(m_bmp280);
}


float BMP280::getTemperature(bool fahrenheit)
{
    float temperature = bmp280_get_temperature(m_bmp280);

    if (fahrenheit)
        return c2f(temperature);
    else
        return temperature;
}

float BMP280::getTemperature()
{
    return getTemperature(false);
}

float BMP280::getPressure()
{
    return bmp280_get_pressure(m_bmp280);
}

void BMP280::setFilter(BMP280_FILTER_T filter)
{
    bmp280_set_filter(m_bmp280, filter);
}

void BMP280::setTimerStandby(BMP280_T_SB_T tsb)
{
    bmp280_set_timer_standby(m_bmp280, tsb);
}

void BMP280::setMeasureMode(BMP280_MODES_T mode)
{
    bmp280_set_measure_mode(m_bmp280, mode);
}

void BMP280::setOversampleRatePressure(BMP280_OSRS_P_T rate)
{
    bmp280_set_oversample_rate_pressure(m_bmp280, rate);
}

void BMP280::setOversampleRateTemperature(BMP280_OSRS_T_T rate)
{
    bmp280_set_oversample_rate_temperature(m_bmp280, rate);
}

uint8_t BMP280::getStatus()
{
    return bmp280_get_status(m_bmp280);
}

void BMP280::setUsageMode(BMP280_USAGE_MODE_T mode)
{
    bmp280_set_usage_mode(m_bmp280, mode);
}

