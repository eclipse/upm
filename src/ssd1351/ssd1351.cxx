/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Based on Adafruit SSD1351 library.
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
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>

#include "ssd1351.hpp"

using namespace upm;
using namespace std;

SSD1351::SSD1351 (uint8_t oc, uint8_t dc, uint8_t rst) :
        GFX(SSD1351WIDTH, SSD1351HEIGHT),
        m_spi(0), m_oc(oc), m_dc(dc), m_rst(rst) {

    m_name = "SSD1351";
    m_usemap = true;

    // Setup SPI bus
    m_spi.frequency(8 * 1000000);
    m_spi.mode(mraa::SPI_MODE0);
    m_spi.writeByte(0x00); // Need to bring clk high before init

    // Init pins
    if (m_oc.dir(mraa::DIR_OUT) != mraa::SUCCESS) {
        throw std::runtime_error(string(__FUNCTION__) +
                               ": Could not initialize CS pin");
        return;
    }
    m_oc.useMmap(true);
    if (m_dc.dir(mraa::DIR_OUT) != mraa::SUCCESS) {
        throw std::runtime_error(string(__FUNCTION__) +
                               ": Could not initialize data/cmd pin");
        return;
    }
    m_dc.useMmap(true);
    if (m_rst.dir(mraa::DIR_OUT) != mraa::SUCCESS) {
        throw std::runtime_error(string(__FUNCTION__) +
                               ": Could not initialize reset pin");
        return;
    }

    // Toggle reset pin
    ocLow();
    m_rst.write(1);
    usleep(500000);
    m_rst.write(0);
    usleep(500000);
    m_rst.write(1);
    usleep(500000);

    // Configure and init display
    writeCommand(SSD1351_CMD_COMMANDLOCK);
    writeData(0x12);

    writeCommand(SSD1351_CMD_COMMANDLOCK);
    writeData(0xB1);

    writeCommand(SSD1351_CMD_DISPLAYOFF);

    writeCommand(SSD1351_CMD_CLOCKDIV);
    writeCommand(0xF1);

    writeCommand(SSD1351_CMD_MUXRATIO);
    writeData(127);

    writeCommand(SSD1351_CMD_SETREMAP);
    writeData(0x74);

    writeCommand(SSD1351_CMD_SETCOLUMN);
    writeData(0x00);
    writeData(0x7F);

    writeCommand(SSD1351_CMD_SETROW);
    writeData(0x00);
    writeData(0x7F);

    writeCommand(SSD1351_CMD_STARTLINE);
    if (SSD1351HEIGHT == 96) {
        writeData(96);
    } else {
        writeData(0);
    }

    writeCommand(SSD1351_CMD_DISPLAYOFFSET);
    writeData(0x0);

    writeCommand(SSD1351_CMD_SETGPIO);
    writeData(0x00);

    writeCommand(SSD1351_CMD_FUNCTIONSELECT);
    writeData(0x01);

    writeCommand(SSD1351_CMD_PRECHARGE);
    writeCommand(0x32);

    writeCommand(SSD1351_CMD_VCOMH);
    writeCommand(0x05);

    writeCommand(SSD1351_CMD_NORMALDISPLAY);

    writeCommand(SSD1351_CMD_CONTRASTABC);
    writeData(0xC8);
    writeData(0x80);
    writeData(0xC8);

    writeCommand(SSD1351_CMD_CONTRASTMASTER);
    writeData(0x0F);

    writeCommand(SSD1351_CMD_SETVSL );
    writeData(0xA0);
    writeData(0xB5);
    writeData(0x55);

    writeCommand(SSD1351_CMD_PRECHARGE2);
    writeData(0x01);

    writeCommand(SSD1351_CMD_DISPLAYON);
}

SSD1351::~SSD1351() {
}

void
SSD1351::writeCommand (uint8_t value) {
    dcLow();
    m_spi.writeByte(value);
}

void
SSD1351::writeData (uint8_t value) {
    dcHigh ();
    m_spi.writeByte(value);
}

void
SSD1351::drawPixel(int16_t x, int16_t y, uint16_t color) {
      if ((x < 0) || (y < 0) || (x >= SSD1351WIDTH) || (y >= SSD1351HEIGHT))
          return;

      if(m_usemap) {
          int index = (y * SSD1351WIDTH + x) * 2;
          m_map[index] = color >> 8;
          m_map[index + 1] = color;
      } else {
          writeCommand(SSD1351_CMD_SETCOLUMN);
          writeData(x);
          writeData(SSD1351WIDTH-1);

          writeCommand(SSD1351_CMD_SETROW);
          writeData(y);
          writeData(SSD1351HEIGHT-1);

          writeCommand(SSD1351_CMD_WRITERAM);
          writeData(color >> 8);
          writeData(color);
      }
}
void
SSD1351::refresh () {
    writeCommand(SSD1351_CMD_WRITERAM);
    int blockSize = SSD1351HEIGHT * SSD1351WIDTH * 2 / BLOCKS;
    dcHigh();
    for (int block = 0; block < BLOCKS; block++) {
        m_spi.write(&m_map[block * blockSize], blockSize);
    }
}
void
SSD1351::ocLow() {
    if (m_oc.write(LOW) != mraa::SUCCESS) {
        throw std::runtime_error(string(__FUNCTION__) +
                               ": Failed to write CS pin");
    }
}
void
SSD1351::ocHigh() {
    if (m_oc.write(HIGH) != mraa::SUCCESS) {
        throw std::runtime_error(string(__FUNCTION__) +
                               ": Failed to write CS pin");
    }
}
void
SSD1351::dcLow() {
    if (m_dc.write(LOW) != mraa::SUCCESS) {
        throw std::runtime_error(string(__FUNCTION__) +
                               ": Failed to write data/cmd pin");
    }
}
void
SSD1351::dcHigh() {
    if (m_dc.write(HIGH) != mraa::SUCCESS) {
        throw std::runtime_error(string(__FUNCTION__) +
                               ": Failed to write data/cmd pin");
    }
}
void
upm::SSD1351::useMemoryMap(bool var) {
    m_usemap = var;
}
