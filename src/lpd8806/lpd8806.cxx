/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <stdexcept>

#include "lpd8806.h"

using namespace upm;

LPD8806::LPD8806 (uint16_t pixelCount, uint8_t csn) {
    mraa_result_t error = MRAA_SUCCESS;
    m_name = "LPD8806";

    m_pixels = NULL;

    m_csnPinCtx = mraa_gpio_init (csn);
    if (m_csnPinCtx == NULL) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": GPIO failed to initialize");
    }

    error = mraa_gpio_dir (m_csnPinCtx, MRAA_GPIO_OUT);
    if (error != MRAA_SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": GPIO failed to set direction");
    }

    CSOff ();

    m_spi = mraa_spi_init (0);
    if (m_spi == NULL) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": SPI failed to initialize");
    }

    // set spi mode to mode2 (CPOL = 0, CPHA = 0)
    mraa_spi_mode (m_spi, MRAA_SPI_MODE0);

    CSOn ();
    // issue initial latch/reset to strip:
    for (uint16_t i = ((pixelCount + 31) / 32); i > 0; i--) {
        mraa_spi_write (m_spi, 0);
    }
    CSOff ();

    m_pixelsCount = pixelCount;

    uint8_t  latchBytes;
    uint16_t dataBytes, totalBytes;
    uint16_t numBytes = 0;

    dataBytes  = m_pixelsCount * 3;
    latchBytes = (m_pixelsCount + 31) / 32;
    totalBytes = dataBytes + latchBytes;
    if ((m_pixels = (uint8_t *) malloc(totalBytes))) {
        numBytes = totalBytes;
        memset ( m_pixels           , 0x80, dataBytes);  // Init to RGB 'off' state
        memset (&m_pixels[dataBytes], 0   , latchBytes); // Clear latch bytes
    }
}

LPD8806::~LPD8806() {
    mraa_result_t error = MRAA_SUCCESS;

    if (m_pixels) {
        free(m_pixels);
    }
    
    error = mraa_spi_stop(m_spi);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
    error = mraa_gpio_close (m_csnPinCtx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
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
        mraa_spi_write (m_spi, *ptr++);
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

mraa_result_t
LPD8806::CSOn () {
    return mraa_gpio_write (m_csnPinCtx, HIGH);
}

mraa_result_t
LPD8806::CSOff () {
    return mraa_gpio_write (m_csnPinCtx, LOW);
}
