/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Author: Tyler Gibson <tgibson@microsoft.com>
 * Copyright (c) 2015 Microsoft Corporation.
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

mraa::Result EBOLED::refresh()
{
  mraa::Result error = mraa::SUCCESS;;

  m_gpioCD.write(1);            // data mode
  for(int i=0; i<BUFFER_SIZE; i++)
  {
    error = data(screenBuffer[i]);
    if(error != mraa::SUCCESS)
      return error;
  }

  return error;
}

mraa::Result EBOLED::write (std::string msg)
{
  int len = msg.length();
  uint8_t temp_cursorX = m_cursorX;
  for (int idx = 0; idx < len; idx++)
  {
    if (msg[idx] == '\n')
    {
      m_cursorY += m_textSize * 9;
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

      //textColor used to avoid wrapping if COLOR_BLACK is set.
      if (m_textWrap && (m_textColor > OLED_WIDTH - temp_cursorX - 6))
      {
        m_cursorY += m_textSize * 9;
        temp_cursorX = m_cursorX;
      }
    }
  }
  return mraa::SUCCESS;;
}

mraa::Result EBOLED::setCursor (int row, int column) {
  m_cursorX = column;
  m_cursorY = row;
  return mraa::SUCCESS;;
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
      //32 offset to align standard ASCII range to index
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

mraa::Result EBOLED::clear()
{
  mraa::Result error = mraa::SUCCESS;;

  m_gpioCD.write(1);            // data mode
  for(int i=0; i<BUFFER_SIZE; i++)
  {
    error = data(0x0000);
    if(error != mraa::SUCCESS)
      return error;
  }

  return mraa::SUCCESS;;
}

mraa::Result EBOLED::home()
{
  return setCursor(0, 0);
}

void EBOLED::drawPixel(int8_t x, int8_t y, uint8_t color)
{
  if(x<0 || x>=OLED_WIDTH || y<0 || y>=OLED_HEIGHT)
    return;

  /* Screenbuffer is uint16 array, but pages are 8bit high so each buffer
   * index is two columns.  This means the index is based on x/2 and
   * OLED_WIDTH/2 = VERT_COLUMNS.
   *
   * Then to set the appropriate bit, we need to shift based on the y
   * offset in relation to the page and then adjust for high/low based
   * on the x position.
  */

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

void EBOLED::drawLine(int8_t x0, int8_t y0, int8_t x1, int8_t y1, uint8_t color)
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

void EBOLED::drawLineHorizontal(int8_t x, int8_t y, uint8_t width, uint8_t color)
{
  drawLine(x, y, x+width-1, y, color);
}

void EBOLED::drawLineVertical(int8_t x, int8_t y, uint8_t height, uint8_t color)
{
  drawLine(x, y, x, y+height-1, color);
}

void EBOLED::drawRectangle(int8_t x, int8_t y, uint8_t width, uint8_t height, uint8_t color)
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

void EBOLED::drawRoundedRectangle(int8_t x, int8_t y, int8_t width, int8_t height, int16_t radius, uint8_t color) {
  // smarter version
  drawLineHorizontal(x+radius  , y         , width-2*radius,  color); // Top
  drawLineHorizontal(x+radius  , y+height-1, width-2*radius,  color); // Bottom
  drawLineVertical(  x         , y+radius  , height-2*radius, color); // Left
  drawLineVertical(  x+width-1 , y+radius  , height-2*radius, color); // Right
  // draw four corners
  drawRoundCorners(x+radius        , y+radius         , radius, 1, color);
  drawRoundCorners(x+width-radius-1, y+radius         , radius, 2, color);
  drawRoundCorners(x+width-radius-1, y+height-radius-1, radius, 4, color);
  drawRoundCorners(x+radius        , y+height-radius-1, radius, 8, color);
}

void EBOLED::drawRectangleFilled(int8_t x, int8_t y, uint8_t width, uint8_t height, uint8_t color)
{
  for (uint8_t i=x; i<x+width; i++) {
    drawLineVertical(i, y, height, color);
  }
}

void EBOLED::drawTriangle(int8_t x0, int8_t y0, int8_t x1, int8_t y1, int8_t x2, int8_t y2, uint8_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

void EBOLED::drawTriangleFilled ( int8_t x0, int8_t y0, int8_t x1, int8_t y1, int8_t x2, int8_t y2, uint8_t color) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    drawLineHorizontal(a, y0, b-a+1, color);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
  int32_t
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    drawLineHorizontal(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    drawLineHorizontal(a, y, b-a+1, color);
  }
}

void EBOLED::drawCircle(int16_t x0, int16_t y0, int16_t radius, uint8_t color)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t x = 0;
  int16_t y = radius;

  drawPixel(x0  , y0+radius, color);
  drawPixel(x0  , y0-radius, color);
  drawPixel(x0+radius, y0  , color);
  drawPixel(x0-radius, y0  , color);

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

void EBOLED::drawRoundCorners( int8_t x0, int8_t y0, int16_t radius, uint8_t cornername, uint8_t color) {
  int16_t f     = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t x     = 0;
  int16_t y     = radius;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      drawPixel(x0 + x, y0 + y, color);
      drawPixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      drawPixel(x0 + x, y0 - y, color);
      drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      drawPixel(x0 - y, y0 + x, color);
      drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      drawPixel(x0 - y, y0 - x, color);
      drawPixel(x0 - x, y0 - y, color);
    }
  }
}

void EBOLED::drawCircleFilled(int8_t x0, int8_t y0, int16_t radius, uint8_t color) {
  drawLineVertical(x0, y0-radius, 2*radius+1, color);
  drawRoundedCornersFilled(x0, y0, radius, 3, 0, color);
}

void EBOLED::drawRoundedCornersFilled(int8_t x0, int8_t y0, int16_t radius, uint8_t cornername, int16_t delta, uint8_t color) {

  int16_t f     = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t x     = 0;
  int16_t y     = radius;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      drawLineVertical(x0+x, y0-y, 2*y+1+delta, color);
      drawLineVertical(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      drawLineVertical(x0-x, y0-y, 2*y+1+delta, color);
      drawLineVertical(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

void EBOLED::fillScreen(uint8_t color)
{
  drawRectangleFilled(0, 0, OLED_WIDTH-1, OLED_HEIGHT-1, color);
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

mraa::Result EBOLED::data(uint16_t data)
{
  m_spi.write_word(data);
  return mraa::SUCCESS;
}

void EBOLED::clearScreenBuffer()
{
  for(int i=0; i<BUFFER_SIZE;i++)
    screenBuffer[i] = 0x0000;
}
