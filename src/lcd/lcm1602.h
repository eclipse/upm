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
 * @man adafruit sparkfun
 * @web https://www.adafruit.com/datasheets/TC1602A-01T.pdf
 * @con i2c
 *
 * @brief API for LCM1602 i2c controller for HD44780 based displays
 *
 * This supports all sizes of HD44780 displays from 16x2 to 4x20, the
 * controller has no idea of the actual display hardware so will let you write
 * further than you can see. These displays with such controllers are available
 * from various manufacturers with different i2c addresses. The adafruit
 * TC1602A-01T seems to be a well documented example.
 *
 * @image html lcm1602.jpeg
 * @snippet lcm1602-lcd.cxx Interesting
 */
class Lcm1602 : public LCD
{
  public:
    /**
     * Lcm1602 Constructor, calls libmraa initialisation functions
     *
     * @param bus i2c bus to use
     * @param address the slave address the lcd is registered on
     */
    Lcm1602(int bus, int address);

    /**
     * Lcm1602 alternate constructor, used for GPIO based hd44780
     * controllers supporting RS, Enable, and 4 data pins in 4-bit
     * mode.
     *
     * @param rs register select pin
     * @param enable enable pin
     * @param d0 data 0 pin
     * @param d1 data 1 pin
     * @param d2 data 2 pin
     * @param d3 data 3 pin
     */
    Lcm1602(uint8_t rs,  uint8_t enable,
            uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

    /**
     * Lcm1602 Destructor
     */
    ~Lcm1602();
    /**
     * Write a string to LCD
     *
     * @param msg The std::string to write to display, note only ascii
     *     chars are supported
     * @return Result of operation
     */
    mraa_result_t write(std::string msg);
    /**
     * Set cursor to a coordinate
     *
     * @param row The row to set cursor to
     * @param column The column to set cursor to
     * @return Result of operation
     */
    mraa_result_t setCursor(int row, int column);
    /**
     * Clear display from characters
     *
     * @return Result of operatio
     */
    mraa_result_t clear();
    /**
     * Return to coordinate 0,0
     *
     * @return Result of operation
     */
    mraa_result_t home();

    /**
     * Create a custom character
     *
     * @param charSlot the character slot to write, only 8 are available
     * @param charData The character data (8 bytes) making up the character
     * @return Result of operation
     */
    mraa_result_t createChar(uint8_t charSlot, uint8_t charData[]);

    /**
     * Turn the display on
     *
     */
    void displayOn();

    /**
     * Turn the display off
     *
     */
    void displayOff();

    /**
     * Turn the cursor on
     *
     */
    void cursorOn();

    /**
     * Turn the cursor off
     *
     */
    void cursorOff();

    /**
     * Turn cursor blink on
     *
     */
    void cursorBlinkOn();

    /**
     * Turn cursor blink off
     *
     */
    void cursorBlinkOff();

    /**
     * Scroll the display left, without changing the character RAM
     *
     */
    void scrollDisplayLeft();

    /**
     * Scroll the display right, without changing the character RAM
     *
     */
    void scrollDisplayRight();

    /**
     * set the entry mode so that characters are added left to right
     *
     */
    void entryLeftToRight();

    /**
     * set the entry mode so that characters are added right to left
     *
     */
    void entryRightToLeft();

    /**
     * Right justify text entered from the cursor
     *
     */
    void autoscrollOn();

    /**
     * Left justify text entered from the cursor
     *
     */
    void autoscrollOff();


  protected:
    mraa_result_t send(uint8_t value, int mode);
    mraa_result_t write4bits(uint8_t value);
    mraa_result_t expandWrite(uint8_t value);
    mraa_result_t pulseEnable(uint8_t value);

  private:
    int m_lcd_control_address;
    mraa::I2c* m_i2c_lcd_control;

    // true if using i2c, false otherwise (gpio)
    bool m_isI2C;

    // gpio operation
    mraa::Gpio* m_gpioRS;
    mraa::Gpio* m_gpioEnable;
    mraa::Gpio* m_gpioD0;
    mraa::Gpio* m_gpioD1;
    mraa::Gpio* m_gpioD2;
    mraa::Gpio* m_gpioD3;

    uint8_t m_displayControl;
    uint8_t m_entryDisplayMode;

};
}
