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

static uint16_t screenBuffer[BUFFER_SIZE];

EBOLED::EBOLED(int spi, int CD, int reset) : 
  m_spi(spi), m_gpioCD(CD), m_gpioRST(reset)
{
  m_name = "EBOLED";
  m_textColor = COLOR_WHITE;
  m_textWrap = 0;
  m_textSize = 1;
  m_cursorX = 0;
  m_cursorY = 0;

  m_gpioCD.dir(mraa::DIR_OUT);
  m_gpioRST.dir(mraa::DIR_OUT);

  //1000000 is standard.
  m_spi.frequency(10000000);
  
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
  
  setAddressingMode(HORIZONTAL);
    
  //Set Page Address range, required for horizontal addressing mode.
  command(CMD_SETPAGEADDRESS); // triple-byte cmd
  command(0x00); //Initial page address
  command(0x05); //Final page address
    
  //Set Column Address range, required for horizontal addressing mode.
  command(CMD_SETCOLUMNADDRESS); // triple-byte cmd
  command(0x20); // this display has a horizontal offset of 20 columns
  command(0x5f); // 64 columns wide - 0 based 63 offset
}

EBOLED::~EBOLED()
{
  clear();
}

mraa_result_t EBOLED::refresh()
{
  mraa_result_t error = MRAA_SUCCESS;
    
  m_gpioCD.write(1);            // data mode
  for(int i=0; i<BUFFER_SIZE; i++) 
  {
    error = data(screenBuffer[i]);
    if(error != MRAA_SUCCESS)
      return error;    
  }
    
  return error;
}

mraa_result_t EBOLED::write (std::string msg) {
  int len = msg.length();
  uint8_t temp_cursorX = m_cursorX;
  for (int idx = 0; idx < len; idx++) 
  {
    if (msg[idx] == '\n') 
    {
      m_cursorY += m_textSize * 7;
      temp_cursorX = m_cursorX;
    } 
    else if (msg[idx] == '\r') 
    {
      // skip em
    } 
    else 
    {
      drawChar(temp_cursorX, m_cursorY, msg[idx], m_textColor, m_textSize);
      temp_cursorX += m_textSize * 6;
      if (m_textWrap && (m_textColor > OLED_WIDTH - temp_cursorX - 6)) 
      {
        m_cursorY += m_textSize * 7;
        temp_cursorX = m_cursorX;
      }
    }
  }
  return MRAA_SUCCESS;
}

mraa_result_t EBOLED::setCursor (int y, int x) {
  m_cursorX = x;
  m_cursorY = y;
  return MRAA_SUCCESS;
}

void EBOLED::setTextColor (uint8_t textColor) {
  m_textColor   = textColor;
}

void EBOLED::setTextSize (uint8_t size) {
  m_textSize = (size > 0) ? size : 1;
}

void EBOLED::setTextWrap (uint8_t wrap) {
  m_textWrap = wrap;
}

void EBOLED::drawChar (uint8_t x, uint8_t y, uint8_t data, uint8_t color, uint8_t size) {
  if( (x >= OLED_WIDTH)            || // Clip right
      (y >= OLED_HEIGHT)           || // Clip bottom
      ((x + 6 * size - 1) < 0)  || // Clip left
      ((y + 8 * size - 1) < 0))    // Clip top
  return;
  
  if (data < 0x20 || data > 0x7F) {
    data = 0x20; // space
  }

  for (int8_t i=0; i<6; i++ ) {
    uint8_t line;
    if (i == 6) 
      line = 0x0;
    else 
    {
      //line = *(font+(data * 5)+i);
      line = BasicFont[data - 32][i+1];
      for (int8_t j = 0; j<8; j++) 
      {
        if (line & 0x1) 
        {
          if (size == 1) // default size
            drawPixel(x+i, y+j, color);
          else 
            drawRectangleFilled(x+(i*size), y+(j*size), size, size, color); // big size
        } 
        line >>= 1;
      }
    }
  }
}

mraa_result_t EBOLED::clear()
{
  mraa_result_t error = MRAA_SUCCESS;
  
  clearScreenBuffer();
  error = refresh();
 
  return MRAA_SUCCESS;
}

mraa_result_t EBOLED::home()
{
  return setCursor(0, 0);
}

void EBOLED::drawPixel(uint8_t x, uint8_t y, uint8_t color) 
{    
  if(x<0 || x>=OLED_WIDTH || y<0 || y>=OLED_HEIGHT)
    return;
  
  switch(color)
  {
    case COLOR_XOR:  
      screenBuffer[(x/2) + ((y/8) * VERT_COLUMNS)] ^= (1<<(y%8+(x%2 * 8)));
      return;
    case COLOR_WHITE:
      screenBuffer[(x/2) + ((y/8) * VERT_COLUMNS)] |= (1<<(y%8+(x%2 * 8)));
      return;
    case COLOR_BLACK:
      screenBuffer[(x/2) + ((y/8) * VERT_COLUMNS)] &= ~(1<<(y%8+(x%2 * 8)));
      return;
  }
}

void EBOLED::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{  
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
      
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs (y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void EBOLED::drawLineHorizontal(uint8_t x, uint8_t y, uint8_t width, uint8_t color)
{
  drawLine(x, y, x+width-1, y, color);
}

void EBOLED::drawLineVertical(uint8_t x, uint8_t y, uint8_t height, uint8_t color)
{
  drawLine(x, y, x, y+height-1, color);
}

void EBOLED::drawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
  drawLineHorizontal(x, y, width, color);
  drawLineHorizontal(x, y+height-1, color);
    
  uint8_t innerHeight = height - 2;
  if(innerHeight > 0) 
  {
    drawLineVertical(x, y+1, innerHeight, color);
    drawLineVertical(x+width-1, y+1, innerHeight, color);
  }
}

void EBOLED::drawRectangleFilled(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{    
  for (uint8_t i=x; i<x+width; i++) {
    drawLineVertical(i, y, height, color);
  }
}

void EBOLED::drawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color) 
{    
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

void EBOLED::drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) 
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0  , y0+r, color);
  drawPixel(x0  , y0-r, color);
  drawPixel(x0+r, y0  , color);
  drawPixel(x0-r, y0  , color);

  while (x<y) 
  {
    if (f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;

    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}

void EBOLED::fillScreen(uint8_t color) 
{
  drawRectangleFilled(0, 0, OLED_WIDTH-1, OLED_HEIGHT-1, color);
}

mraa_result_t EBOLED::setAddressingMode(displayAddressingMode mode)
{
  mraa_result_t rv;

  rv = command(CMD_MEMORYADDRMODE);
  rv = command(mode);

  return rv;
}

mraa_result_t EBOLED::command(uint8_t cmd)
{
  m_gpioCD.write(0);            // command mode
  m_spi.writeByte(cmd);
  return MRAA_SUCCESS;
}

mraa_result_t EBOLED::data(uint16_t data)
{
  m_spi.write_word(data);
  return MRAA_SUCCESS;
}

void EBOLED::clearScreenBuffer() 
{
  for(int i=0; i<BUFFER_SIZE;i++)
    screenBuffer[i] = 0x0000; 
}
