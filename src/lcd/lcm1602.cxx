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
 * Permission is hereby granted, free of charge, to any person
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

#include <string>
#include <stdexcept>
#include <unistd.h>

#include "hd44780_bits.h"
#include "lcm1602.h"

using namespace upm;

Lcm1602::Lcm1602(int bus_in, int addr_in, bool isExpander,
                 uint8_t numColumns, uint8_t numRows) :
  m_i2c_lcd_control(new mraa::I2c(bus_in)),
  m_gpioRS(0), m_gpioEnable(0), m_gpioD0(0),
  m_gpioD1(0), m_gpioD2(0), m_gpioD3(0),
  m_numColumns(numColumns), m_numRows(numRows)
{
    mraa::Result error = mraa::SUCCESS;
    m_name = "Lcm1602 (I2C)";
    m_isI2C = true;

    m_lcd_control_address = addr_in;

    error = m_i2c_lcd_control->address(m_lcd_control_address);
    if (error != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": I2c.address() failed");
        return;
    }

    // default display control
    m_displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;

    // if we are not dealing with an expander (say via a derived class
    // like Jhd1313m1), then we do not want to execute the rest of the
    // code below.  Rather, the derived class's constructor should
    // follow up with any setup required -- we will only initialize
    // the I2C context and bail.

    if (!isExpander)
      return;

    usleep(50000);
    expandWrite(LCD_BACKLIGHT);
    usleep(100000);

    write4bits(0x03 << 4);
    usleep(4500);
    write4bits(0x30);
    usleep(4500);
    write4bits(0x30);
    usleep(150);

    // Put into 4 bit mode
    write4bits(0x20);

    m_displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    // Set numeber of lines
    command(LCD_FUNCTIONSET | 0x0f);
    command(LCD_DISPLAYCONTROL | m_displayControl);
    clear();

    // Set entry mode.
    m_entryDisplayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    command(LCD_ENTRYMODESET | m_entryDisplayMode);

    home();
}

Lcm1602::Lcm1602(uint8_t rs,  uint8_t enable, uint8_t d0, 
                 uint8_t d1, uint8_t d2, uint8_t d3,
                 uint8_t numColumns, uint8_t numRows) :
  m_i2c_lcd_control(0),  
  m_gpioRS(new mraa::Gpio(rs)), m_gpioEnable(new mraa::Gpio(enable)), 
  m_gpioD0(new mraa::Gpio(d0)), m_gpioD1(new mraa::Gpio(d1)),
  m_gpioD2(new mraa::Gpio(d2)), m_gpioD3(new mraa::Gpio(d3)),
  m_numColumns(numColumns), m_numRows(numRows)
{
    mraa::Result error = mraa::SUCCESS;
    m_name = "Lcm1602 (4-bit GPIO)";
    m_isI2C = false;

    // setup our gpios

    m_gpioRS->dir(mraa::DIR_OUT);
    m_gpioEnable->dir(mraa::DIR_OUT);

    m_gpioD0->dir(mraa::DIR_OUT);
    m_gpioD1->dir(mraa::DIR_OUT);
    m_gpioD2->dir(mraa::DIR_OUT);
    m_gpioD3->dir(mraa::DIR_OUT);


    // set RS and Enable low to begin issuing commands
    m_gpioRS->write(0);
    m_gpioEnable->write(0);

    // wait to stabilize
    usleep(100000);

    // set 4bit mode

    // These steps are adapted from the HD44780 datasheet, figure 24

    // try 1
    write4bits(0x03);
    usleep(4500);

    // try 2
    write4bits(0x03);
    usleep(4500);

    // try 3
    write4bits(0x03);
    usleep(150);

    // Finally, put into 4 bit mode
    write4bits(0x02);

    // Set number of lines
    command(LCD_FUNCTIONSET | LCD_2LINE | LCD_4BITMODE | LCD_5x8DOTS);
    m_displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    command(LCD_DISPLAYCONTROL | m_displayControl);
    usleep(2000);
    clear();

    // Set entry mode.
    m_entryDisplayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    command(LCD_ENTRYMODESET | m_entryDisplayMode);

    home();
}

Lcm1602::~Lcm1602()
{
  // clean up after ourselves
  if (m_isI2C)
    {
      delete m_i2c_lcd_control;
    }
  else
    {
      delete m_gpioRS;
      delete m_gpioEnable;

      delete m_gpioD0;
      delete m_gpioD1;
      delete m_gpioD2;
      delete m_gpioD3;
    }
}

/*
 * **************
 *  virtual area
 * **************
 */
mraa::Result
Lcm1602::write(std::string msg)
{
    mraa::Result error = mraa::SUCCESS;
    for (std::string::size_type i = 0; i < msg.size(); ++i) {
        error = data(msg[i]);
    }
    return error;
}

mraa::Result
Lcm1602::setCursor(int row, int column)
{
    mraa::Result error = mraa::SUCCESS;
    column = column % m_numColumns;
    uint8_t offset = column;

    switch (m_numRows)
    {
        case 1:
            // Single row displays with more than 8 columns usually have their
            // DDRAM split in two halves. The first half starts at address 00.
            // The second half starts at address 40. E.g. 16x2 DDRAM mapping:
            // 00 01 02 03 04 05 06 07 40 41 42 43 44 45 46 47
            if (m_numColumns > 8)
            {
                offset = (column % (m_numColumns / 2)) +
                         (column / (m_numColumns / 2)) * 0x40;
            }
            break;
        case 2:
            // this should work for any display with two rows
            // DDRAM mapping:
            // 00 .. 27
            // 40 .. 67
            offset += row * 0x40;
            break;
        case 4:
            if (m_numColumns == 16)
            {
                 // 16x4 display
                 // DDRAM mapping:
                 // 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
                 // 40 41 42 43 43 45 46 47 48 49 4A 4B 4C 4D 4E 4F
                 // 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
                 // 50 51 52 53 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F
                 int row_addr[] = { 0x00, 0x40, 0x10, 0x50 };
                 offset += row_addr[row];
             }
             else
             {
                 // 20x4 display
                 // DDRAM mapping:
                 // 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13
                 // 40 41 42 43 43 45 46 47 48 49 4A 4B 4C 4D 4E 4F 50 51 52 53
                 // 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23 24 25 26 27
                 // 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F 60 61 62 63 64 65 66 67
		 int row_addr[] = { 0x00, 0x40, 0x14, 0x54 };
                 offset += row_addr[row];
             }
             break;
    }

    return command(LCD_CMD | offset);
}

mraa::Result
Lcm1602::clear()
{
    mraa::Result ret;
    ret = command(LCD_CLEARDISPLAY);
    usleep(2000); // this command takes awhile
    return ret;
}

mraa::Result
Lcm1602::home()
{
    mraa::Result ret;
    ret = command(LCD_RETURNHOME);
    usleep(2000); // this command takes awhile
    return ret;
}

mraa::Result
Lcm1602::createChar(uint8_t charSlot, uint8_t charData[])
{
    mraa::Result error = mraa::SUCCESS;
    charSlot &= 0x07; // only have 8 positions we can set
    error = command(LCD_SETCGRAMADDR | (charSlot << 3));
    if (error == mraa::SUCCESS) {
        for (int i = 0; i < 8; i++) {
          error = data(charData[i]);
        }
    }

    return error;
}

mraa::Result Lcm1602::displayOn()
{
  m_displayControl |= LCD_DISPLAYON;
  return command(LCD_DISPLAYCONTROL | m_displayControl);
}

mraa::Result Lcm1602::displayOff()
{
  m_displayControl &= ~LCD_DISPLAYON;
  return command(LCD_DISPLAYCONTROL | m_displayControl);
}

mraa::Result Lcm1602::cursorOn()
{
  m_displayControl |= LCD_CURSORON;
  return command(LCD_DISPLAYCONTROL | m_displayControl);
}

mraa::Result Lcm1602::cursorOff()
{
  m_displayControl &= ~LCD_CURSORON;
  return command(LCD_DISPLAYCONTROL | m_displayControl);
}

mraa::Result Lcm1602::cursorBlinkOn()
{
  m_displayControl |= LCD_BLINKON;
  return command(LCD_DISPLAYCONTROL | m_displayControl);
}

mraa::Result Lcm1602::cursorBlinkOff()
{
  m_displayControl &= ~LCD_BLINKON;
  return command(LCD_DISPLAYCONTROL | m_displayControl);
}

mraa::Result Lcm1602::scrollDisplayLeft()
{
  return command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

mraa::Result Lcm1602::scrollDisplayRight()
{
  return command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

mraa::Result Lcm1602::entryLeftToRight()
{
  m_entryDisplayMode |= LCD_ENTRYLEFT;
  return command(LCD_ENTRYMODESET | m_entryDisplayMode);
}

mraa::Result Lcm1602::entryRightToLeft()
{
  m_entryDisplayMode &= ~LCD_ENTRYLEFT;
  return command(LCD_ENTRYMODESET | m_entryDisplayMode);
}

mraa::Result Lcm1602::autoscrollOn()
{
  m_entryDisplayMode |= LCD_ENTRYSHIFTINCREMENT;
  return command(LCD_ENTRYMODESET | m_entryDisplayMode);
}

mraa::Result Lcm1602::autoscrollOff()
{
  m_entryDisplayMode &= ~LCD_ENTRYSHIFTINCREMENT;
  return command(LCD_ENTRYMODESET | m_entryDisplayMode);
}

mraa::Result Lcm1602::command(uint8_t cmd)
{
  return send(cmd, 0);
}

mraa::Result Lcm1602::data(uint8_t cmd)
{
  return send(cmd, LCD_RS); // 1
}


/*
 * **************
 *  private area
 * **************
 */
mraa::Result
Lcm1602::send(uint8_t value, int mode)
{
    mraa::Result ret = mraa::SUCCESS;
    uint8_t h;
    uint8_t l;

    if (m_isI2C)
      {
        h = value & 0xf0;
        l = (value << 4) & 0xf0;
        ret = write4bits(h | mode);
        ret = write4bits(l | mode);
        return ret;
      }

    // else, gpio (4 bit)

    // register select
    m_gpioRS->write(mode);
    
    h = value >> 4;
    l = value & 0x0f;

    ret = write4bits(h);
    ret = write4bits(l);
    return ret;
}

mraa::Result
Lcm1602::write4bits(uint8_t value)
{
    mraa::Result ret = mraa::SUCCESS;

    if (m_isI2C)
      {
        ret = expandWrite(value);
        ret = pulseEnable(value);
        return ret;
      }

    // else gpio
    ret = m_gpioD0->write( ((value >> 0) & 0x01) );
    ret = m_gpioD1->write( ((value >> 1) & 0x01) );
    ret = m_gpioD2->write( ((value >> 2) & 0x01) );
    ret = m_gpioD3->write( ((value >> 3) & 0x01) );

    ret = pulseEnable(value); // value is ignored here for gpio

    return ret;
}

mraa::Result
Lcm1602::expandWrite(uint8_t value)
{
    // invalid for gpio
    if (!m_isI2C)
        return mraa::ERROR_INVALID_RESOURCE;

    uint8_t buffer = value | LCD_BACKLIGHT;
    return m_i2c_lcd_control->writeByte(buffer);
}

mraa::Result
Lcm1602::pulseEnable(uint8_t value)
{
    mraa::Result ret = mraa::SUCCESS;

    if (m_isI2C)
      {
        ret = expandWrite(value | LCD_EN);
        usleep(1);
        ret = expandWrite(value & ~LCD_EN);
        usleep(50);
        return ret;
      }

    // else gpio

    ret = m_gpioEnable->write(0);
    usleep(1);
    ret = m_gpioEnable->write(1);
    usleep(1); // must be > 450ns
    ret = m_gpioEnable->write(0);
    usleep(100); // must be >37us

    return ret;
}
