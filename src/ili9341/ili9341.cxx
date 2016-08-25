/*
 * Author: Shawn Hymel
 * Copyright (c) 2015 SparkFun Electronics
 *
 * Credits to Adafruit.
 * Based on Adafruit ST7735 library, see original license in license.txt file.
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
#include <stdio.h>
#include <string.h>

#include "ili9341.hpp"

using namespace upm;

ILI9341::ILI9341(uint8_t csLCD, uint8_t csSD, uint8_t dc, uint8_t rst) :
    GFX(ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT), m_csLCDPinCtx(csLCD), 
    m_csSDPinCtx(csSD), m_dcPinCtx(dc), m_rstPinCtx(rst), m_spi(0) {
    
    initModule();
    configModule();
}

void ILI9341::initModule() {
    mraa::Result error = mraa::SUCCESS;
    
    error = m_csLCDPinCtx.dir(mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        mraa::printError(error);
    }
    
    error = m_csSDPinCtx.dir(mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        mraa::printError(error);
    }
    
    error = m_dcPinCtx.dir(mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        mraa::printError(error);
    }
    
    error = m_rstPinCtx.dir(mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        mraa::printError(error);
    }
    
    error = m_spi.frequency(SPI_FREQ);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }
    
    lcdCSOff();
}

void ILI9341::setRotation(uint8_t r) {

    writecommand(ILI9341_MADCTL);
    r = r % 4; // can't be higher than 3
    switch(r) {
    case 0:
        writedata(MADCTL_MX | MADCTL_BGR);
        _width  = ILI9341_TFTWIDTH;
        _height = ILI9341_TFTHEIGHT;
        break;
    case 1:
        writedata(MADCTL_MV | MADCTL_BGR);
        _width  = ILI9341_TFTHEIGHT;
        _height = ILI9341_TFTWIDTH;
        break;
    case 2:
        writedata(MADCTL_MY | MADCTL_BGR);
        _width  = ILI9341_TFTWIDTH;
        _height = ILI9341_TFTHEIGHT;
        break;
    case 3:
        writedata(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
        _width  = ILI9341_TFTHEIGHT;
        _height = ILI9341_TFTWIDTH;
        break;
    }
}

void ILI9341::configModule() {
    
    // Toggle RST low to reset
    rstHigh();
    usleep(5000);
    rstLow();
    usleep(20000);
    rstHigh();
    usleep(150000);

    // Send initialization commands
    writecommand(0xEF);
    writedata(0x03);
    writedata(0x80);
    writedata(0x02);

    writecommand(0xCF);  
    writedata(0x00); 
    writedata(0XC1); 
    writedata(0X30); 

    writecommand(0xED);  
    writedata(0x64); 
    writedata(0x03); 
    writedata(0X12); 
    writedata(0X81); 

    writecommand(0xE8);  
    writedata(0x85); 
    writedata(0x00); 
    writedata(0x78); 

    writecommand(0xCB);  
    writedata(0x39); 
    writedata(0x2C); 
    writedata(0x00); 
    writedata(0x34); 
    writedata(0x02); 

    writecommand(0xF7);  
    writedata(0x20); 

    writecommand(0xEA);  
    writedata(0x00); 
    writedata(0x00); 

    writecommand(ILI9341_PWCTR1);    //Power control 
    writedata(0x23);   //VRH[5:0] 

    writecommand(ILI9341_PWCTR2);    //Power control 
    writedata(0x10);   //SAP[2:0];BT[3:0] 

    writecommand(ILI9341_VMCTR1);    //VCM control 
    writedata(0x3e);
    writedata(0x28); 

    writecommand(ILI9341_VMCTR2);    //VCM control2 
    writedata(0x86);  //--

    writecommand(ILI9341_MADCTL);    // Memory Access Control 
    writedata(0x48);

    writecommand(ILI9341_PIXFMT);    
    writedata(0x55); 

    writecommand(ILI9341_FRMCTR1);    
    writedata(0x00);  
    writedata(0x18); 

    writecommand(ILI9341_DFUNCTR);    // Display Function Control 
    writedata(0x08); 
    writedata(0x82);
    writedata(0x27);  

    writecommand(0xF2);    // 3Gamma Function Disable 
    writedata(0x00); 

    writecommand(ILI9341_GAMMASET);    //Gamma curve selected 
    writedata(0x01); 

    writecommand(ILI9341_GMCTRP1);    //Set Gamma 
    writedata(0x0F); 
    writedata(0x31); 
    writedata(0x2B); 
    writedata(0x0C); 
    writedata(0x0E); 
    writedata(0x08); 
    writedata(0x4E); 
    writedata(0xF1); 
    writedata(0x37); 
    writedata(0x07); 
    writedata(0x10); 
    writedata(0x03); 
    writedata(0x0E); 
    writedata(0x09); 
    writedata(0x00); 

    writecommand(ILI9341_GMCTRN1);    //Set Gamma 
    writedata(0x00); 
    writedata(0x0E); 
    writedata(0x14); 
    writedata(0x03); 
    writedata(0x11); 
    writedata(0x07); 
    writedata(0x31); 
    writedata(0xC1); 
    writedata(0x48); 
    writedata(0x08); 
    writedata(0x0F); 
    writedata(0x0C); 
    writedata(0x31); 
    writedata(0x36); 
    writedata(0x0F);
    
    writecommand(ILI9341_SLPOUT);
    usleep(120000);
    writecommand(ILI9341_DISPON);
}

void ILI9341::setAddrWindow(uint16_t x0, 
                            uint16_t y0, 
                            uint16_t x1, 
                            uint16_t y1) {
    
    writecommand(ILI9341_CASET); // Column addr set
    writedata(x0 >> 8);
    writedata(x0 & 0xFF);     // XSTART 
    writedata(x1 >> 8);
    writedata(x1 & 0xFF);     // XEND
    
    writecommand(ILI9341_PASET); // Row addr set
    writedata(y0>>8);
    writedata(y0);     // YSTART
    writedata(y1>>8);
    writedata(y1);     // YEND

    writecommand(ILI9341_RAMWR); // write to RAM
}

void ILI9341::drawPixel(int16_t x, int16_t y, uint16_t color) {
    
    if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) {
        return;
    }
    
    setAddrWindow(x, y, x + 1, y + 1);
    
    writedata(color >> 8);
    writedata(color);
}

void ILI9341::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {

    // Rudimentary clipping
    if((x >= _width) || (y >= _height)) {
        return;
    }
    
    if((y+h-1) >= _height) {
        h = _height-y;
    }

    setAddrWindow(x, y, x, y+h-1);

    uint8_t hi = color >> 8;
    uint8_t lo = color;
    
    lcdCSOn();
    dcHigh();

    while (h--) {
        m_spi.writeByte(hi);
        m_spi.writeByte(lo);
    }

    lcdCSOff();
}

void ILI9341::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {

    // Rudimentary clipping
    if((x >= _width) || (y >= _height)) {
        return;
    }

    if((x+w-1) >= _width) {
        w = _width - x;
    }

    setAddrWindow(x, y, x+w-1, y);

    uint8_t hi = color >> 8;
    uint8_t lo = color;

    lcdCSOn();
    dcHigh();

    while (w--) {
        m_spi.writeByte(hi);
        m_spi.writeByte(lo);
    }

    lcdCSOff();
}

void ILI9341::fillRect(int16_t x, 
                       int16_t y, 
                       int16_t w, 
                       int16_t h,
                       uint16_t color) {
                       
    // rudimentary clipping (drawChar w/big text requires this)
    if((x >= _width) || (y >= _height)) return;
    if((x + w - 1) >= _width)  w = _width  - x;
    if((y + h - 1) >= _height) h = _height - y;

    setAddrWindow(x, y, x+w-1, y+h-1);

    uint8_t hi = color >> 8;
    uint8_t lo = color;
  
    lcdCSOn();
    dcHigh();

    for(y = h; y > 0; y--) {
        for(x = w; x > 0; x--) {
            m_spi.writeByte(hi);
            m_spi.writeByte(lo);
        }
    }

    lcdCSOff();
}

void ILI9341::fillScreen(uint16_t color) {
    fillRect(0, 0,  _width, _height, color);
}

void ILI9341::invertDisplay(bool i) {
    writecommand(i ? ILI9341_INVON : ILI9341_INVOFF);
}

uint16_t ILI9341::color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void ILI9341::executeCMDList(const uint8_t *addr) {
    uint8_t numCommands, numArgs;
    uint16_t ms;
        
    numCommands = *(addr++);        // Number of commands to follow
    while (numCommands--) {         // For each command...
        writecommand(*(addr++));    // Read, issue command
        numArgs = *(addr++);        // Number of args to follow
        ms = numArgs & DELAY;       // If hibit set, delay follows args
        numArgs &= ~DELAY;          // Mask out delay bit
        while (numArgs--) {         // For each argument...
            writedata(*(addr++));   // Read, issue argument
        }
        
        if (ms) {
            ms = *(addr++);         // Read post-command delay time (ms)
            if (ms == 255) {
                ms = 500;           // If 255, delay for 500 ms
            }
            usleep(ms * 1000);
        }
    }
}

void ILI9341::writecommand(uint8_t c) {
    lcdCSOn();
    dcLow();
    m_spi.writeByte(c);
    lcdCSOff();
}

void ILI9341::writedata(uint8_t d) {
    lcdCSOn();
    dcHigh();
    m_spi.writeByte(d);
    lcdCSOff();
}

mraa::Result ILI9341::lcdCSOn() {
    mraa::Result error = mraa::SUCCESS;

    error = m_csLCDPinCtx.write(LOW);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    error = m_csSDPinCtx.write(HIGH);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    return error;
}

mraa::Result ILI9341::lcdCSOff() {
    mraa::Result error = mraa::SUCCESS;

    error = m_csLCDPinCtx.write(HIGH);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    return error;
}

mraa::Result ILI9341::sdCSOn() {
    mraa::Result error = mraa::SUCCESS;

    error = m_csSDPinCtx.write(LOW);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    error = m_csLCDPinCtx.write(HIGH);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    return error;
}

mraa::Result ILI9341::sdCSOff() {
    mraa::Result error = mraa::SUCCESS;

    error = m_csSDPinCtx.write(HIGH);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    return error;
}

mraa::Result ILI9341::dcHigh() {
    mraa::Result error = mraa::SUCCESS;

    error = m_dcPinCtx.write(HIGH);
    if (error != mraa::SUCCESS) {
        mraa::printError(error);
    }

    return error;
}

mraa::Result ILI9341::dcLow() {
    mraa::Result error = mraa::SUCCESS;

    error = m_dcPinCtx.write(LOW);
    if (error != mraa::SUCCESS) {
        mraa::printError(error);
    }

    return error;
}

mraa::Result ILI9341::rstHigh() {
    mraa::Result error = mraa::SUCCESS;
    
    error = m_rstPinCtx.write(HIGH);
    if (error != mraa::SUCCESS) {
        mraa::printError(error);
    }
    
    return error;
}

mraa::Result ILI9341::rstLow() {
    mraa::Result error = mraa::SUCCESS;

    error = m_rstPinCtx.write(LOW);
    if (error != mraa::SUCCESS) {
        mraa::printError(error);
    }

    return error;
}