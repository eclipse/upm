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
#include <unistd.h>
#include <iostream>

#include "eboled.h"

using namespace upm;
using namespace std;

EBOLED::EBOLED(int spi, int CD, int reset) : 
  m_spi(spi), m_gpioCD(CD), m_gpioRST(reset)
{
  m_name = "EBOLED";
  
  m_gpioCD.dir(mraa::DIR_OUT);
  m_gpioRST.dir(mraa::DIR_OUT);

  m_spi.frequency(1000000);

  // reset the device
  m_gpioRST.write(1);
  usleep(5000);
  m_gpioRST.write(0);
  usleep(10000);
  m_gpioRST.write(1);

  command(CMD_DISPLAYOFF);

  command(CMD_SETDISPLAYCLOCKDIV);
  command(0x80);

  command(CMD_SETMULTIPLEX);
  command(0x2f);
  
  command(CMD_SETDISPLAYOFFSET);
  command(0x0);                 // no offset
  
  command(CMD_SETSTARTLINE | 0x0); // line #0
  
  command(CMD_CHARGEPUMP);      // enable charge pump
  command(0x14);
  
  command(CMD_NORMALDISPLAY);
  command(CMD_DISPLAYALLONRESUME);
  
  command(CMD_SEGREMAP | 0x1);  // reverse mapping (SEG0==COL127)
  command(CMD_COMSCANDEC);
  
  command(CMD_SETCOMPINS);      // custom COM PIN mapping
  command(0x12);
  
  command(CMD_SETCONTRAST);
  command(0x8f);
  
  command(CMD_SETPRECHARGE);
  command(0xf1);
  
  command(CMD_SETVCOMDESELECT);
  command(0x40);
  
  command(CMD_DISPLAYON);
  

  usleep(4500);
  
  clear();
  setAddressingMode(PAGE);
}

EBOLED::~EBOLED()
{
  clear();
}

mraa::Result EBOLED::draw(uint8_t* bdata, int bytes)
{
  mraa::Result error = mraa::SUCCESS;

  setAddressingMode(HORIZONTAL);

  command(CMD_SETCOLUMNADDRESS); // triple-byte cmd
  command(0x20); // this display has a horizontal offset of 20 columns
  command(0x20 + 0x40); // 64 columns wide

  for (int idx = 0; idx < bytes; idx++)
    error = data(bdata[idx]);

  // reset addressing mode
  setAddressingMode(PAGE);

  return error;
}

mraa::Result EBOLED::write(std::string msg)
{
  mraa::Result error = mraa::SUCCESS;

  setAddressingMode(PAGE);
  for (std::string::size_type i = 0; i < msg.size(); ++i)
    error = writeChar(msg[i]);

  return error;
}

mraa::Result EBOLED::setCursor(int row, int column)
{
  mraa::Result error = mraa::SUCCESS;

  // set page address
  error = command(CMD_SETPAGESTARTADDR | (row & 0x07));

  // set column address
  error = command(CMD_SETLOWCOLUMN | (column & 0x0f));
  // ediblock oled starts at col 20 apparently...
  error = command(CMD_SETHIGHCOLUMN | ((column >> 4) & 0x0f) + 0x02);

  return error;
}

mraa::Result EBOLED::clear()
{
  mraa::Result error = mraa::SUCCESS;
  uint8_t columnIdx, rowIdx;
  
  for (rowIdx = 0; rowIdx < 8; rowIdx++)
    {
      setCursor(rowIdx, 0);
      
      // clear all columns (8 * 8 pixels per char)
      for (columnIdx = 0; columnIdx < 8; columnIdx++)
        error = writeChar(' ');
    }
  
  home();
  
  return mraa::SUCCESS;
}

mraa::Result EBOLED::home()
{
  return setCursor(0, 0);
}

mraa::Result EBOLED::writeChar(uint8_t value)
{
  mraa::Result rv;

  if (value < 0x20 || value > 0x7F) {
    value = 0x20; // space
  }

  for (uint8_t idx = 0; idx < 8; idx++) {
    rv = data(BasicFont[value - 32][idx]);
  }

  return rv;
}

mraa::Result EBOLED::setAddressingMode(displayAddressingMode mode)
{
  mraa::Result rv;

  rv = command(CMD_MEMORYADDRMODE);
  rv = command(mode);

  return rv;
}

mraa::Result EBOLED::command(uint8_t cmd)
{
  m_gpioCD.write(0);            // command mode
  m_spi.writeByte(cmd);
  return mraa::SUCCESS;
}

mraa::Result EBOLED::data(uint8_t data)
{
  m_gpioCD.write(1);            // data mode
  m_spi.writeByte(data);
  return mraa::SUCCESS;
}
