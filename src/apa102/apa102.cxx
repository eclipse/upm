/*
 * Author: Yannick Adam <yannick.adam@gmail.com>
 * Copyright (c) 2016 Yannick Adam
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

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <unistd.h>

#include "apa102.h"

using namespace upm;

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

APA102::~APA102()
{
    // Clear leds
    if (m_leds) {
        free(m_leds);
    }

    // Clear SPI
    if (m_spi) {
        delete m_spi;
    }

    // Clear GPIO
    if (m_csnPinCtx) {
        delete m_csnPinCtx;
    }
}


void
APA102::setLed(uint16_t ledIdx, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b)
{
    setLeds(ledIdx, ledIdx, brightness, r, g, b);
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
APA102::setLeds(uint16_t startIdx, uint16_t endIdx, uint8_t* colors)
{
    uint16_t s_idx = (startIdx + 1) * 4;
    memcpy(&m_leds[s_idx], colors, (endIdx - startIdx + 1) * 4);

    if (!m_batchMode) {
        pushState();
    }
}

void
APA102::pushState(void)
{
    CSOn();
    m_spi->write(m_leds, m_frameLength);
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
