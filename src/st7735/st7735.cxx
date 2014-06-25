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
#include <stdio.h>
#include <string.h>

#include "st7735.h"

using namespace upm;

ST7735::ST7735 (uint8_t csLCD, uint8_t cSD, uint8_t rs, uint8_t rst) : GFX (160, 128, m_map, font) {
    mraa_init();

    m_csLCD = csLCD;
    m_cSD   = cSD;
    m_rST   = rst;
    m_rS    = rs;

    initModule ();
    configModule ();
}

ST7735::~ST7735 () {
    mraa_result_t error = MRAA_SUCCESS;
    error = mraa_spi_stop(m_spi);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
    error = mraa_gpio_close (m_csLCDPinCtx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
    error = mraa_gpio_close (m_cSDPinCtx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
    error = mraa_gpio_close (m_rSTPinCtx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
    error = mraa_gpio_close (m_rSPinCtx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
}

void
ST7735::initModule () {
    mraa_result_t error = MRAA_SUCCESS;

    m_height = 160;
    m_width  = 128;

    m_csLCDPinCtx = mraa_gpio_init (m_csLCD);
    if (m_csLCDPinCtx == NULL) {
        fprintf (stderr, "Are you sure that pin%d you requested is valid on your platform?", m_csLCD);
        exit (1);
    }

    m_cSDPinCtx = mraa_gpio_init (m_cSD);
    if (m_cSDPinCtx == NULL) {
        fprintf (stderr, "Are you sure that pin%d you requested is valid on your platform?", m_cSD);
        exit (1);
    }

    m_rSTPinCtx = mraa_gpio_init (m_rST);
    if (m_rSTPinCtx == NULL) {
        fprintf (stderr, "Are you sure that pin%d you requested is valid on your platform?", m_rST);
        exit (1);
    }

    m_rSPinCtx = mraa_gpio_init (m_rS);
    if (m_rSPinCtx == NULL) {
        fprintf (stderr, "Are you sure that pin%d you requested is valid on your platform?", m_rS);
        exit (1);
    }

    error = mraa_gpio_dir (m_csLCDPinCtx, MRAA_GPIO_OUT);
    if (error != MRAA_SUCCESS) {
        mraa_result_print (error);
    }

    error = mraa_gpio_dir (m_cSDPinCtx, MRAA_GPIO_OUT);
    if (error != MRAA_SUCCESS) {
        mraa_result_print (error);
    }

    error = mraa_gpio_dir (m_rSTPinCtx, MRAA_GPIO_OUT);
    if (error != MRAA_SUCCESS) {
        mraa_result_print (error);
    }

    error = mraa_gpio_dir (m_rSPinCtx, MRAA_GPIO_OUT);
    if (error != MRAA_SUCCESS) {
        mraa_result_print (error);
    }

    m_spi = mraa_spi_init (0);
    error = mraa_spi_frequency(m_spi, 15 * 1000000);
    if (error != MRAA_SUCCESS) {
        mraa_result_print (error);
    }

    lcdCSOn ();
}

void
ST7735::write (uint8_t value) {
    rsLOW ();
    mraa_spi_write (m_spi, value);
}

void
ST7735::data (uint8_t value) {
    rsHIGH ();
    mraa_spi_write (m_spi, value);
}

void
ST7735::executeCMDList(const uint8_t *addr) {
    uint8_t  numCommands, numArgs;
    uint16_t ms;

    numCommands = *(addr++);           // Number of commands to follow
    while(numCommands--) {             // For each command...
        write (*(addr++));             // Read, issue command
        numArgs  = *(addr++);          // Number of args to follow
        ms       = numArgs & DELAY;    // If hibit set, delay follows args
        numArgs &= ~DELAY;             // Mask out delay bit
        while(numArgs--) {             // For each argument...
            data (*(addr++));          // Read, issue argument
        }

        if(ms) {
            ms = *(addr++);            // Read post-command delay time (ms)
            if (ms == 255) {
                ms = 500;              // If 255, delay for 500 ms
            }
            usleep (ms * 1000);
        }
    }
}

void
ST7735::setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    uint8_t colstart, rowstart;
    colstart  = rowstart = 0;

    write (ST7735_CASET);                       // Column addr set

    rsHIGH ();
    m_spiBuffer[0] = 0x00;
    m_spiBuffer[1] = x0 + colstart;             // XSTART
    m_spiBuffer[2] = 0x00;
    m_spiBuffer[3] = x1 + colstart;             // XEND
    mraa_spi_write_buf(m_spi, m_spiBuffer, 4);

    write (ST7735_RASET);                       // Row addr set

    rsHIGH ();
    m_spiBuffer[0] = 0x00;
    m_spiBuffer[1] = y0 + rowstart;             // YSTART
    m_spiBuffer[2] = 0x00;
    m_spiBuffer[3] = y1 + rowstart;             // YEND
    mraa_spi_write_buf(m_spi, m_spiBuffer, 4);

    write (ST7735_RAMWR);                       // write to RAM
}

void
ST7735::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (MRAA_SUCCESS != setPixel (x, y, color)) {
        return;
    }

    refresh ();
}

void
ST7735::refresh () {
    rsHIGH ();

    int fragmentSize = m_height * m_width * 2 / 20;
    for (int fragment = 0; fragment < 20; fragment++) {
        mraa_spi_write_buf(m_spi, &m_map[fragment * fragmentSize], fragmentSize);
    }
}

void
ST7735::configModule() {
    rsHIGH ();
    lcdCSOff ();
    lcdCSOn ();

    mraa_gpio_write (m_rSTPinCtx, HIGH);
    usleep (500000);
    mraa_gpio_write (m_rSTPinCtx, LOW);
    usleep (500000);
    mraa_gpio_write (m_rSTPinCtx, HIGH);
    usleep (500000);

    executeCMDList (Rcmd1);
    executeCMDList (Rcmd2red);
    executeCMDList (Rcmd3);

    write (ST7735_MADCTL);
    data (0xC0);

    setAddrWindow (0, 0, m_width - 1, m_height - 1);

    fillScreen (ST7735_BLACK);
    refresh ();
}

mraa_result_t
ST7735::lcdCSOn () {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_gpio_write (m_csLCDPinCtx, LOW);
    if (error != MRAA_SUCCESS) {
        mraa_result_print (error);
    }

    error = mraa_gpio_write (m_cSDPinCtx, HIGH);
    if (error != MRAA_SUCCESS) {
        mraa_result_print (error);
    }

    return error;
}

mraa_result_t
ST7735::lcdCSOff () {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_gpio_write (m_csLCDPinCtx, HIGH);
    if (error != MRAA_SUCCESS) {
        mraa_result_print (error);
    }

    return error;
}

mraa_result_t
ST7735::sdCSOn () {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_gpio_write (m_cSDPinCtx, LOW);
    if (error != MRAA_SUCCESS) {
        mraa_result_print (error);
    }

    error = mraa_gpio_write (m_csLCDPinCtx, HIGH);
    if (error != MRAA_SUCCESS) {
        mraa_result_print (error);
    }

    return error;
}

mraa_result_t
ST7735::sdCSOff () {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_gpio_write (m_cSDPinCtx, HIGH);
    if (error != MRAA_SUCCESS) {
        mraa_result_print (error);
    }

    return error;
}

mraa_result_t
ST7735::rsHIGH () {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_gpio_write (m_rSPinCtx, HIGH);
    if (error != MRAA_SUCCESS) {
        mraa_result_print (error);
    }

    return error;
}

mraa_result_t
ST7735::rsLOW () {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_gpio_write (m_rSPinCtx, LOW);
    if (error != MRAA_SUCCESS) {
        mraa_result_print (error);
    }

    return error;
}
