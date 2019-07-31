/*
 * Author: Sergey Kiselev <sergey.kiselev@intel.com>
 * Copyright (c) 2017 Sergey Kiselev
 * Based on the apa102 driver writen by Yannick Adam <yannick.adam@gmail.com>
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

#include "p9813.hpp"

using namespace upm;

P9813::P9813(uint16_t ledCount, uint16_t clkPin, uint16_t dataPin, bool batchMode)
        : m_leds(ledCount), m_clkGpio(clkPin), m_dataGpio(dataPin), m_batchMode(batchMode)
{
    mraa::Result res = mraa::SUCCESS;

    // Set GPIO pins to output direction
    res = m_clkGpio.dir(mraa::DIR_OUT);
    if (res != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": GPIO failed to set direction");
    }
    res = m_dataGpio.dir(mraa::DIR_OUT);
    if (res != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": GPIO failed to set direction");
    }
}

void
P9813::setLed(uint16_t ledIdx, uint8_t r, uint8_t g, uint8_t b)
{
    setLeds(ledIdx, ledIdx, r, g, b);
}

void
P9813::setAllLeds(uint8_t r, uint8_t g, uint8_t b)
{
    setLeds(0, m_leds.size() - 1, r, g, b);
}

void
P9813::setLeds(uint16_t startIdx, uint16_t endIdx, uint8_t r, uint8_t g, uint8_t b)
{
    for (uint16_t i = startIdx; i <= endIdx; i++) {
	m_leds[i].red = r;
	m_leds[i].green = g;
	m_leds[i].blue = b;
    }

    if (!m_batchMode) {
        pushState();
    }
}

void
P9813::setLeds(uint16_t startIdx, uint16_t endIdx, std::vector<RgbColor> colors)
{
    for (uint16_t i = startIdx, j = 0; i <= endIdx; i++) {
	m_leds[i].red = colors[j].red;
	m_leds[i].green = colors[j].green;
	m_leds[i].blue = colors[j].blue;
    }

    if (!m_batchMode) {
        pushState();
    }
}

void
P9813::pushState(void)
{
    // Begin data frame
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);

    for (uint16_t i = 0; i < m_leds.size(); i++) {
	uint8_t red = m_leds[i].red;
        uint8_t green = m_leds[i].green;
        uint8_t blue = m_leds[i].blue;
        // The first byte is 11 followed by inverted bits 7 and 6 of blue, green, and red
        uint8_t byte0 = 0xFF;
        byte0 ^= (blue >> 2) & 0x30; // XOR bits 4-5
        byte0 ^= (green >> 4) & 0x0C; // XOR bits 2-3
        byte0 ^= (red >> 6) & 0x03; // XOR bits 0-1
	sendByte(byte0);
        sendByte(blue);
        sendByte(green);
        sendByte(red);
    }

    // End data frame
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
}

/*
 * **************
 *  private area
 * **************
*/

void
P9813::sendByte(uint8_t data)
{
    for (uint16_t i = 0; i < 8; i++) {
        // Write the data bit
        m_dataGpio.write((data & 0x80) >> 7);
	// Pulse the clock
        m_clkGpio.write(1);
        usleep(20);
        m_clkGpio.write(0);
        usleep(20);

	// Shift to the next data bit
        data <<= 1;
    }
}
