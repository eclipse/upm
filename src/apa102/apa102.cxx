/*
 * Author: Yannick Adam <yannick.adam@gmail.com>
 * Copyright (c) 2016 Yannick Adam
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <unistd.h>

#include "upm_string_parser.hpp"
#include "apa102.hpp"

using namespace upm;

static bool operator!(mraa::MraaIo &mraaIo)
{
  return mraaIo.getMraaDescriptors() == NULL;
}

APA102::APA102(uint16_t ledCount, uint8_t spiBus, bool batchMode, int8_t csn)
        : m_ledCount(ledCount), m_batchMode(batchMode)
{
    mraa::Result res = mraa::SUCCESS;
    m_leds = NULL;

    // Optional chip select pin
    m_csnPinCtx = NULL;
    if (csn > -1) {
        m_csnPinCtx = new mraa::Gpio(csn);
        res = m_csnPinCtx->dir(mraa::DIR_OUT);
        if (res != mraa::SUCCESS) {
            throw std::invalid_argument(std::string(__FUNCTION__) +
                                        ": GPIO failed to set direction");
        }
    }

    CSOff();
    // Initialize SPI
    m_spi = new mraa::Spi(spiBus);

    // Initialize LED array
    uint16_t endFrameLength = (m_ledCount + 15) / 16; // End frame should be (leds/2) bits
    m_frameLength = endFrameLength + (m_ledCount + 1) * 4;
    if ((m_leds = (uint8_t*) malloc(m_frameLength))) {
        memset(m_leds, 0x00, m_frameLength - 4);                               // Clear state
        memset(&m_leds[m_frameLength - endFrameLength], 0xFF, endFrameLength); // Frame End

        // Need to set the brightness to "0" for each Led
        for (int i = 1; i <= m_ledCount; i++) {
            m_leds[i * 4] = 224;
        }

    } else {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": Failed to allocate memory for LED Strip");
    }
}

APA102::APA102(std::string initStr) : mraaIo(initStr)
{
    mraa::Result res = mraa::SUCCESS;
    m_leds = NULL;
    m_batchMode = false;

    std::vector<std::string> upmTokens;

    if(!mraaIo.getLeftoverStr().empty()) {
      upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
    }

    std::string::size_type sz, prev_sz;;
    for (std::string tok : upmTokens) {
        if(tok.substr(0, 9) == "ledCount:") {
            m_ledCount = (uint16_t)std::stoi(tok.substr(9), nullptr, 0);
        }
        if(tok.substr(0, 10) == "batchMode:") {
            m_batchMode = std::stoi(tok.substr(10), nullptr, 0);
        }
    }

    // Optional chip select pin
    m_csnPinCtx = NULL;
    if(!mraaIo.gpios.empty()) {
        m_csnPinCtx = &mraaIo.gpios[0];
        res = m_csnPinCtx->dir(mraa::DIR_OUT);
        if (res != mraa::SUCCESS) {
            throw std::invalid_argument(std::string(__FUNCTION__) +
                                        ": GPIO failed to set direction");
        }
    }

    CSOff();
    // Initialize SPI
    if(!mraaIo.spis.empty()) {
        m_spi = &mraaIo.spis[0];
    } else {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_spi_init failed");
    }

    // Initialize LED array
    uint16_t endFrameLength = (m_ledCount + 15) / 16; // End frame should be (leds/2) bits
    m_frameLength = endFrameLength + (m_ledCount + 1) * 4;
    if ((m_leds = (uint8_t*) malloc(m_frameLength))) {
        memset(m_leds, 0x00, m_frameLength - 4);                               // Clear state
        memset(&m_leds[m_frameLength - endFrameLength], 0xFF, endFrameLength); // Frame End

        // Need to set the brightness to "0" for each Led
        for (int i = 1; i <= m_ledCount; i++) {
            m_leds[i * 4] = 224;
        }
    } else {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": Failed to allocate memory for LED Strip");
    }


    for (std::string tok : upmTokens) {
        if(tok.substr(0, 7) == "setLed:") {
            uint16_t ledIdx = std::stoul(tok.substr(7), &sz, 0);
            tok = tok.substr(7);
            prev_sz = sz;
            uint8_t brightness = std::stoul(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            prev_sz = sz;
            uint8_t r = std::stoul(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            prev_sz = sz;
            uint8_t g = std::stoul(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            prev_sz = sz;
            uint8_t b = std::stoul(tok.substr(prev_sz + 1), &sz, 0);
            setLed(ledIdx, brightness, r, g, b);
        }
        if(tok.substr(0, 17) == "setLedBrightness:") {
            uint16_t ledIdx = std::stoul(tok.substr(17), &sz, 0);
            tok = tok.substr(17);
            uint8_t brightness = std::stoul(tok.substr(sz + 1), nullptr, 0);
            setLedBrightness(ledIdx, brightness);
        }
        if(tok.substr(0, 11) == "setAllLeds:") {
            uint16_t brightness = std::stoul(tok.substr(11), &sz, 0);
            tok = tok.substr(11);
            prev_sz = sz;
            uint8_t r = std::stoul(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            prev_sz = sz;
            uint8_t g = std::stoul(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            prev_sz = sz;
            uint8_t b = std::stoul(tok.substr(prev_sz + 1), &sz, 0);
            setAllLeds(brightness, r, g, b);
        }
        if(tok.substr(0, 8) == "setLeds:") {
            uint16_t startIdx = std::stoul(tok.substr(8), &sz, 0);
            tok = tok.substr(8);
            prev_sz = sz;
            uint16_t endIdx = std::stoul(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            prev_sz = sz;
            uint8_t brightness = std::stoul(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            prev_sz = sz;
            uint8_t r = std::stoul(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            prev_sz = sz;
            uint8_t g = std::stoul(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            prev_sz = sz;
            uint8_t b = std::stoul(tok.substr(prev_sz + 1), &sz, 0);
            setLeds(startIdx, endIdx, brightness, r, g, b);
        }
        if(tok.substr(0, 18) == "setLedsBrightness:") {
            uint16_t startIdx = std::stoul(tok.substr(18), &sz, 0);
            tok = tok.substr(18);
            prev_sz = sz;
            uint16_t endIdx = std::stoul(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            prev_sz = sz;
            uint8_t brightness = std::stoul(tok.substr(prev_sz + 1), &sz, 0);
            setLedsBrightness(startIdx, endIdx, brightness);
        }
        if(tok.substr(0, 12) == "setBusSpeed:") {
            int hz = std::stoi(tok.substr(12), nullptr, 0);
            setBusSpeed(hz);
        }
    }
}


APA102::~APA102()
{
    // Clear leds
    if (m_leds) {
        free(m_leds);
    }
    if(!mraaIo) {
        // Clear SPI
        if (m_spi) {
            delete m_spi;
        }

        // Clear GPIO
        if (m_csnPinCtx) {
            delete m_csnPinCtx;
        }
    }
}


void
APA102::setLed(uint16_t ledIdx, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b)
{
    setLeds(ledIdx, ledIdx, brightness, r, g, b);
}

void
APA102::setLedBrightness(uint16_t ledIdx, uint8_t brightness)
{
    setLedsBrightness(ledIdx, ledIdx, brightness);
}

void
APA102::setAllLeds(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b)
{
    setLeds(0, m_ledCount - 1, brightness, r, g, b);
}

void
APA102::setLeds(uint16_t startIdx, uint16_t endIdx, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t s_idx = (startIdx + 1) * 4;
    uint16_t e_idx = (endIdx + 1) * 4;

    for (uint16_t i = s_idx; i <= e_idx; i += 4) {
        m_leds[i] = brightness | 224;
        m_leds[i + 1] = b;
        m_leds[i + 2] = g;
        m_leds[i + 3] = r;
    }

    if (!m_batchMode) {
        pushState();
    }
}

void
APA102::setLedsBrightness(uint16_t startIdx, uint16_t endIdx, uint8_t brightness)
{
    uint16_t s_idx = (startIdx + 1) * 4;
    uint16_t e_idx = (endIdx + 1) * 4;

    for (uint16_t i = s_idx; i <= e_idx; i += 4) {
        m_leds[i] = brightness | 224;
    }

    if (!m_batchMode) {
        pushState();
    }
}

void
APA102::setLeds(uint16_t startIdx, uint16_t endIdx, uint8_t* colors)
{
    uint16_t s_idx = (startIdx + 1) * 4;
    memcpy(&m_leds[s_idx], colors, (endIdx - startIdx + 1) * 4);

    if (!m_batchMode) {
        pushState();
    }
}

void
APA102::setBusSpeed(int hz)
{
    if (m_spi->frequency(hz) != mraa::SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                    ": Failed to change SPI bus speed");
    }
}

void
APA102::pushState(void)
{
    CSOn();
    uint8_t* recv = m_spi->write(m_leds, m_frameLength);
    if (recv != NULL) free(recv);
    CSOff();
}

/*
 * **************
 *  private area
 * **************
 */

mraa::Result
APA102::CSOn()
{
    if (m_csnPinCtx) {
        return m_csnPinCtx->write(HIGH);
    }

    return mraa::ERROR_FEATURE_NOT_SUPPORTED;
}

mraa::Result
APA102::CSOff()
{
    if (m_csnPinCtx) {
        return m_csnPinCtx->write(LOW);
    }

    return mraa::ERROR_FEATURE_NOT_SUPPORTED;
}
