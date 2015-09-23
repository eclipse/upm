/*
 * Author: Marc Graham <marc@m2ag.net>
 * Copyright (c) 2015 Intel Corporation
 *
 * Adapted from ssd1308 library.
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

#pragma once

#include <string>
#include <mraa/i2c.hpp>
#include "lcd.h"
#include "ssd.h"

namespace upm
{
const uint8_t  DISPLAY_CMD_SET_NORMAL_1306 = 0xA6;
const uint8_t  SSD1306_SETCONTRAST = 0x81;
const uint8_t  SSD1306_DISPLAYALLON_RESUME =0xA4;
const uint8_t  SSD1306_DISPLAYALLON = 0xA5;
const uint8_t  DISPLAY_CMD_SET_INVERT_1306 = 0xA7;

const uint8_t  SSD1306_SETDISPLAYOFFSET =0xD3;
const uint8_t  SSD1306_SETCOMPINS = 0xDA;

const uint8_t  SSD1306_SETVCOMDETECT = 0xDB;

const uint8_t  SSD1306_SETDISPLAYCLOCKDIV = 0xD5;
const uint8_t  SSD1306_SETPRECHARGE = 0xD9;

const uint8_t  SSD1306_SETMULTIPLEX = 0xA8;

const uint8_t  SSD1306_SETLOWCOLUM = 0x00;
const uint8_t  SSD1306_SETHIGHCOLUMN = 0x10;

const uint8_t  SSD1306_SETSTARTLINE = 0x40;

const uint8_t  SSD1306_MEMORYMODE = 0x20;
const uint8_t  SSD1306_COLUMNADDR = 0x21;
const uint8_t  SSD1306_PAGEADDR = 0x22;

const uint8_t  SSD1306_COMSCANINC = 0xC0;
const uint8_t  SSD1306_COMSCANDEC = 0xC8;

const uint8_t  SSD1306_SEGREMAP = 0xA0;

const uint8_t  SSD1306_CHARGEPUMP = 0x8D;

const uint8_t  SSD1306_EXTERNALVCC = 0x1;
const uint8_t  SSD1306_SWITCHCAPVCC = 0x2;

// Scrolling const uint8_t s
const uint8_t  SSD1306_ACTIVATE_SCROLL = 0x2F;
const uint8_t  SSD1306_DEACTIVATE_SCROLL = 0x2E;
const uint8_t  SSD1306_SET_VERTICAL_SCROLL_AREA = 0xA3;
const uint8_t  SSD1306_RIGHT_HORIZONTAL_SCROLL = 0x26;
const uint8_t  SSD1306_LEFT_HORIZONTAL_SCROLL = 0x27;
const uint8_t  SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29;
const uint8_t  SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A;

const uint8_t  SSD1306_BLACK = 0;
const uint8_t  SSD1306_WHITE = 1;
const uint8_t  SSD1306_LCDWIDTH = 128;
const uint8_t  SSD1306_LCDHEIGHT = 64;

/**
 * @library i2clcd
 * @sensor ssd1306
 * @comname SSD1306 OLED Display
 * @altname Adafruit SSD1306 OLED Display 0.96"
 * @type display
 * @man adafruit
 * @web https://www.adafruit.com/datasheets/SSD1306.pdf
 * @web http://www.farnell.com/datasheets/609753.pdf
 * @con i2c
 *
 * @brief API for SSD1306 I2C-controlled OLED displays
 *
 * SSD1306 is a 128x64 dot-matrix OLED/PLED segment driver with a
 * controller. This device is available from many suppliers for a
 * very low cost. This implementation was tested using a generic
 * SSD1306 device from eBay.
 *
 * @image html ssd1306.jpeg
 * @snippet ssd1306-oled.cxx Interesting
 */
class SSD1306 : public LCD
{
  public:
    /**
     * SSD1306 constructor; calls libmraa initialisation functions
     *
     * @param bus I2C bus to use
     * @param address Slave address the LCD is registered on
     */
    SSD1306(int bus, int address = 0x3C);
    /**
     * SSD1306 destructor
     */
    ~SSD1306();
    /**
     * Draws an image; see examples/python/make_oled_pic.py for an
     * explanation of how pixels are mapped to bytes
     *
     * @param data Buffer to read
     * @param bytes Number of bytes to read from the pointer
     * @return Result of the operation
     */
    mraa::Result draw(uint8_t* data, int bytes);
    /**
     * Writes a string to the LCD
     *
     * @param msg std::string to write to the display; note: only ASCII
     * characters are supported
     * @return Result of the operation
     */
    mraa::Result write(std::string msg);
    /**
     * Sets the cursor to specified coordinates
     *
     * @param row Row to set the cursor to
     * @param column Column to set the cursor to
     * @return Result of the operation
     */
    mraa::Result setCursor(int row, int column);
    /**
     * Clears the display of all characters
     *
     * @return Result of the operation
     */
    mraa::Result clear();
    /**
     * Returns to the original coordinates (0,0)
     *
     * @return Result of the operation
     */
    mraa::Result home();
    /**
     * Inverts the display
     *
     * @param i true to invert, false for normal display
     * @return Result of the operation
     */
    mraa::Result invert(bool i);
    /**
     *  Activate a scroll to the right for rows start through stop
     *  The display is 16 rows tall. To scroll the whole display, run:
     *  display.scrollright(0x00, 0x0F)
     *
     * @param start First row to scroll
     * @param stop  Last row to scroll
     * @return void
     */
    void startscrollright(uint8_t start, uint8_t stop);
    /**
     *  Activate a scroll to the left for rows start through stop
     *  The display is 16 rows tall. To scroll the whole display, run:
     *  display.startscrollright(0x00, 0x0F)
     *
     * @param start First row to scroll
     * @param stop  Last row to scroll
     * @return void
     */
    void startscrollleft(uint8_t start, uint8_t stop);
    /**
     *  Activate a scroll to the upper right for rows start through stop
     *  The display is 16 rows tall. To scroll the whole display, run:
     *  display.startscrollleft(0x00, 0x0F)
     *
     * @param start First row to scroll
     * @param stop  Last row to scroll
     * @return void
     */
    void startscrolldiagright(uint8_t start, uint8_t stop);
    /**
     *  Activate a scroll to the upper left for rows start through stop
     *  The display is 16 rows tall. To scroll the whole display, run:
     *  display.startscrolldiaagright(0x00, 0x0F)
     *
     * @param start First row to scroll
     * @param stop  Last row to scroll
     * @return void
     */
    void startscrolldiagleft(uint8_t start, uint8_t stop);
    /**
     * Stops display scrolling.
     *
     * @return void
     */
    void stopscroll(void);
    /**
     * Dims display
     *
     * @param dim True to dim display, false for max intensity
     * @return Result of last operation
     */
    void dim(bool dim);

  private:
    mraa::Result writeChar(uint8_t value);
    mraa::Result setNormalDisplay();
    mraa::Result setAddressingMode(displayAddressingMode mode);

    int m_lcd_control_address;
    mraa::I2c m_i2c_lcd_control;

    int _vccstate;
};
}
