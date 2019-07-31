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
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "st7735.hpp"

using namespace upm;

ST7735::ST7735 (int csLCD, int cSD, int rs, int rst)
    : GFX (160, 128, m_map, font), m_spi(0), m_csLCDPinCtx(csLCD), m_cSDPinCtx(cSD),
      m_rSTPinCtx(rst), m_rSPinCtx(rs) {

      initModule ();
    configModule ();
}

void
ST7735::initModule () {
    mraa::Result error = mraa::SUCCESS;

    m_height = 160;
    m_width  = 128;

    m_spi.frequency(15 * 1000000);

    error = m_csLCDPinCtx.dir(mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    error = m_cSDPinCtx.dir(mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    error = m_rSTPinCtx.dir(mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    error = m_rSPinCtx.dir(mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    error = m_spi.frequency(15 * 1000000);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    lcdCSOn ();
}

void
ST7735::write (uint8_t value) {
    rsLOW ();
    m_spi.writeByte(value);
}

void
ST7735::data (uint8_t value) {
    rsHIGH ();
    m_spi.writeByte(value);
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
    free(m_spi.write(m_spiBuffer, 4));

    write (ST7735_RASET);                       // Row addr set

    rsHIGH ();
    m_spiBuffer[0] = 0x00;
    m_spiBuffer[1] = y0 + rowstart;             // YSTART
    m_spiBuffer[2] = 0x00;
    m_spiBuffer[3] = y1 + rowstart;             // YEND
    free(m_spi.write(m_spiBuffer, 4));

    write (ST7735_RAMWR);                       // write to RAM
}

void
ST7735::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (mraa::SUCCESS != setPixel (x, y, color)) {
        return;
    }

    refresh ();
}

void
ST7735::refresh () {
    rsHIGH ();

    int fragmentSize = m_height * m_width * 2 / 20;
    for (int fragment = 0; fragment < 20; fragment++) {
        free(m_spi.write(&m_map[fragment * fragmentSize], fragmentSize));
    }
}

void
ST7735::configModule() {
    rsHIGH ();
    lcdCSOff ();
    lcdCSOn ();

    m_rSTPinCtx.write(HIGH);
    usleep (500000);
    m_rSTPinCtx.write(LOW);
    usleep (500000);
    m_rSTPinCtx.write(HIGH);
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

mraa::Result
ST7735::lcdCSOn () {
    mraa::Result error = mraa::SUCCESS;

    error = m_csLCDPinCtx.write(LOW);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    error = m_cSDPinCtx.write(HIGH);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    return error;
}

mraa::Result
ST7735::lcdCSOff () {
    mraa::Result error = mraa::SUCCESS;

    error = m_csLCDPinCtx.write(HIGH);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    return error;
}

mraa::Result
ST7735::sdCSOn () {
    mraa::Result error = mraa::SUCCESS;

    error = m_cSDPinCtx.write(LOW);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    error = m_csLCDPinCtx.write(HIGH);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    return error;
}

mraa::Result
ST7735::sdCSOff () {
    mraa::Result error = mraa::SUCCESS;

    error = m_cSDPinCtx.write(HIGH);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    return error;
}

mraa::Result
ST7735::rsHIGH () {
    mraa::Result error = mraa::SUCCESS;

    error = m_rSPinCtx.write(HIGH);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    return error;
}

mraa::Result
ST7735::rsLOW () {
    mraa::Result error = mraa::SUCCESS;

    error = m_rSPinCtx.write(LOW);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    return error;
}
