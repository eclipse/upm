/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

// This file contains bit definitions for the HD44780 and compatible
// LCD controllers.  They are used by most drivers in this (LCD)
// library, though the SSD controllers only seem to make use of the
// LCD_DATA and LCD_CMD values.
//
// Those values (DATA, CMD) are specific to the implementation of the
// i2C expander in use, so may not be appropriate for inclusion into
// this file.  But for now, we will leave them here.

#include <stdint.h>

namespace upm
{
// commands
  const uint8_t LCD_CLEARDISPLAY = 0x01;
  const uint8_t LCD_RETURNHOME = 0x02;
  const uint8_t LCD_ENTRYMODESET = 0x04;
  const uint8_t LCD_DISPLAYCONTROL = 0x08;
  const uint8_t LCD_CURSORSHIFT = 0x10;
  const uint8_t LCD_FUNCTIONSET = 0x20;

  const uint8_t LCD_BACKLIGHT = 0x08;
  const uint8_t LCD_NOBACKLIGHT = 0x00;
  
  // flags for display entry mode
  const uint8_t LCD_ENTRYRIGHT = 0x00;
  const uint8_t LCD_ENTRYLEFT = 0x02;
  const uint8_t LCD_ENTRYSHIFTINCREMENT = 0x01;
  const uint8_t LCD_ENTRYSHIFTDECREMENT = 0x00;
  
  // flags for display on/off control
  const uint8_t LCD_DISPLAYON = 0x04;
  const uint8_t LCD_DISPLAYOFF = 0x00;
  const uint8_t LCD_CURSORON = 0x02;
  const uint8_t LCD_CURSOROFF = 0x00;
  const uint8_t LCD_BLINKON = 0x01;
  const uint8_t LCD_BLINKOFF = 0x00;
  
  // flags for display/cursor shift
  const uint8_t LCD_DISPLAYMOVE = 0x08;
  const uint8_t LCD_MOVERIGHT = 0x04;
  const uint8_t LCD_MOVELEFT = 0x00;
  
  // flags for function set
  const uint8_t LCD_8BITMODE = 0x10;
  const uint8_t LCD_4BITMODE = 0x00;
  const uint8_t LCD_2LINE = 0x08;
  const uint8_t LCD_1LINE = 0x00;
  const uint8_t LCD_5x10DOTS = 0x04;
  const uint8_t LCD_5x8DOTS = 0x00;
  
  // flags for CGRAM
  const uint8_t LCD_SETCGRAMADDR = 0x40;
  
  // may be implementation specific
  const uint8_t LCD_EN = 0x04; // Enable bit
  const uint8_t LCD_RW = 0x02; // Read/Write bit
  const uint8_t LCD_RS = 0x01; // Register select bit
  const uint8_t LCD_DATA = 0x40;
  const uint8_t LCD_CMD = 0x80;
}
