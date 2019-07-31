/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <stdexcept>

#include "lpd8806.hpp"

using namespace upm;

LPD8806::LPD8806 (int bus, int csn, uint16_t pixelCount) :m_spi(bus), m_csnPinCtx(csn) {
    mraa::Result error = mraa::SUCCESS;
    m_name = "LPD8806";

    m_pixels = NULL;

    error = m_csnPinCtx.dir (mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": GPIO failed to set direction");
    }

    CSOff ();

    // set spi mode to mode2 (CPOL = 0, CPHA = 0)
    m_spi.mode (mraa::SPI_MODE0);

    CSOn ();
    // issue initial latch/reset to strip:
    for (uint16_t i = ((pixelCount + 31) / 32); i > 0; i--) {
        m_spi.writeByte (0);
    }
    CSOff ();

    m_pixelsCount = pixelCount;

    uint8_t  latchBytes;
    uint16_t dataBytes, totalBytes;

    dataBytes  = m_pixelsCount * 3;
    latchBytes = (m_pixelsCount + 31) / 32;
    totalBytes = dataBytes + latchBytes;
    if ((m_pixels = (uint8_t *) malloc(totalBytes))) {
        memset ( m_pixels           , 0x80, dataBytes);  // Init to RGB 'off' state
        memset (&m_pixels[dataBytes], 0   , latchBytes); // Clear latch bytes
    }
}

LPD8806::~LPD8806() {
    if (m_pixels) {
        free(m_pixels);
    }
}

void
LPD8806::setPixelColor (uint16_t pixelOffset, uint8_t r, uint8_t g, uint8_t b) {
    if (pixelOffset < m_pixelsCount) { // Arrays are 0-indexed, thus NOT '<='
        uint8_t *ptr = &m_pixels[pixelOffset * 3];
        *ptr++ = g | 0x80; // Strip color order is GRB,
        *ptr++ = r | 0x80; // not the more common RGB,
        *ptr++ = b | 0x80; // so the order here is intentional; don't "fix"
    }
}

void
LPD8806::show (void) {
    uint8_t  *ptr   = m_pixels;
    uint16_t byte   = (m_pixelsCount * 3) + ((m_pixelsCount + 31) / 32);
    
    while (byte--) {
        m_spi.writeByte (*ptr++);
    }
}

uint16_t
LPD8806::getStripLength (void) {
    return m_pixelsCount;
}

/*
 * **************
 *  private area
 * **************
 */

mraa::Result
LPD8806::CSOn () {
    return m_csnPinCtx.write (HIGH);
}

mraa::Result
LPD8806::CSOff () {
    return m_csnPinCtx.write (LOW);
}
