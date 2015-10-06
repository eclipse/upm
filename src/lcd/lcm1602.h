/*
 * The MIT License (MIT)
 *
 * Author: Daniel Mosquera
 * Copyright (c) 2013 Daniel Mosquera
 *
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Contributions: Jon Trulson <jtrulson@ics.com>
 *                Sergey Kiselev <sergey.kiselev@intel.com> 
 *
 * Permission is hereby granted, free of uint8_tge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <string>
#include <mraa/i2c.hpp>

#include <mraa/gpio.hpp>

#include "lcd.h"

namespace upm
{
/**
 * @library i2clcd
 * @sensor lcm1602
 * @comname LCM1602 Display
 * @type display
 * @man adafruit sparkfun seeed
 * @web https://www.adafruit.com/datasheets/TC1602A-01T.pdf
 * @con i2c gpio
 *
 * @brief API for the LCM1602 I2C controller for HD44780-based displays
 *
 * This supports all sizes of HD44780 displays, from 16x2 to 4x20. The
 * controller has no idea of the actual display hardware, so it lets you write
 * farther than you can see. These displays with such controllers are available
 * from various manufacturers with different I2C addresses. Adafruit*
 * TC1602A-01T seems to be a well-documented example. The driver also supports
 * parallel GPIO connections directly to the HD44780 in case you are not using
 * an I2C expander/backpack.
 *
 * @image html lcm1602.jpeg
 * @snippet lcm1602-lcd.cxx Interesting
 */
class Lcm1602 : public LCD
{
  public:
    /**
     * Lcm1602 constructor; calls libmraa initialisation functions
     *
     * @param bus I2C bus to use
     * @param address Slave address the LCD is registered on
     * @param isExpander True if we are dealing with an I2C expander,
     * false otherwise. Default is true.
     */
  Lcm1602(int bus, int address, bool isExpander=true,
          uint8_t numColumns = 16, uint8_t numRows = 4);

    /**
     * Lcm1602 alternate constructor, used for GPIO based HD44780
     * controllers supporting RS, Enable, and 4 data pins in 4-bit
     * mode.
     *
     * @param rs Register select pin
     * @param enable Enable pin
     * @param d0 Data 0 pin
     * @param d1 Data 1 pin
     * @param d2 Data 2 pin
     * @param d3 Data 3 pin
     */
    Lcm1602(uint8_t rs,  uint8_t enable,
            uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
            uint8_t numColumns = 16, uint8_t numRows = 4);

    /**
     * Lcm1602 destructor
     */
    ~Lcm1602();
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
     * Create a custom character
     *
     * @param charSlot the character slot to write, only 8 are available
     * @param charData The character data (8 bytes) making up the character
     * @return Result of operation
     */
    mraa::Result createChar(uint8_t charSlot, uint8_t charData[]);

    /**
     * Turn the display on
     *
     * @return Result of operation
     */
    mraa::Result displayOn();

    /**
     * Turn the display off
     *
     * @return Result of operation
     */
    mraa::Result displayOff();

    /**
     * Turn the cursor on
     *
     * @return Result of operation
     */
    mraa::Result cursorOn();

    /**
     * Turn the cursor off
     *
     * @return Result of operation
     */
    mraa::Result cursorOff();

    /**
     * Turn cursor blink on
     *
     * @return Result of operation
     */
    mraa::Result cursorBlinkOn();

    /**
     * Turn cursor blink off
     *
     * @return Result of operation
     */
    mraa::Result cursorBlinkOff();

    /**
     * Scroll the display left, without changing the character RAM
     *
     * @return Result of operation
     */
    mraa::Result scrollDisplayLeft();

    /**
     * Scroll the display right, without changing the character RAM
     *
     * @return Result of operation
     */
    mraa::Result scrollDisplayRight();

    /**
     * set the entry mode so that characters are added left to right
     *
     * @return Result of operation
     */
    mraa::Result entryLeftToRight();

    /**
     * set the entry mode so that characters are added right to left
     *
     * @return Result of operation
     */
    mraa::Result entryRightToLeft();

    /**
     * Right justify text entered from the cursor
     *
     * @return Result of operation
     */
    mraa::Result autoscrollOn();

    /**
     * Left justify text entered from the cursor
     *
     * @return Result of operation
     */
    mraa::Result autoscrollOff();


  protected:
    mraa::Result send(uint8_t value, int mode);
    mraa::Result write4bits(uint8_t value);
    mraa::Result expandWrite(uint8_t value);
    mraa::Result pulseEnable(uint8_t value);

    uint8_t m_displayControl;
    uint8_t m_entryDisplayMode;

    // Display size
    uint8_t m_numColumns;
    uint8_t m_numRows;

    // Add a command() and data() virtual member functions, with a
    // default implementation in lcm1602.  This is expected to be
    // implemented by derived classes with different needs (Jhd1313m1,
    // for example).
    virtual mraa::Result command(uint8_t cmd);
    virtual mraa::Result data(uint8_t data);

    int m_lcd_control_address;
    mraa::I2c* m_i2c_lcd_control;

  private:

    // true if using i2c, false otherwise (gpio)
    bool m_isI2C;

    // gpio operation
    mraa::Gpio* m_gpioRS;
    mraa::Gpio* m_gpioEnable;
    mraa::Gpio* m_gpioD0;
    mraa::Gpio* m_gpioD1;
    mraa::Gpio* m_gpioD2;
    mraa::Gpio* m_gpioD3;
};
}
