/*
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

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "gfx.h"

using namespace upm;

GFX::GFX (int width, int height, uint8_t * screenBuffer, const unsigned char * font) : WIDTH(width), HEIGHT(height) {
    m_height = height;
    m_width  = width;
    m_font   = font;
    m_map    = screenBuffer;
}

GFX::~GFX () {
}

mraa::Result
GFX::setPixel (int x, int y, uint16_t color) {
    if((x < 0) ||(x >= m_width) || (y < 0) || (y >= m_height)) {
        return mraa::ERROR_UNSPECIFIED;
    }

    int index = ((y * m_width) + x) * sizeof(uint16_t);
    m_map[index] = (uint8_t) (color >> 8);
    m_map[++index] = (uint8_t)(color);

    return mraa::SUCCESS;
}

void
GFX::fillScreen (uint16_t color) {
    fillRect(0, 0, m_width, m_height, color);
}

void
GFX::fillRect (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    for (int16_t i=x; i<x+w; i++) {
        drawFastVLine(i, y, h, color);
    }
}

void
GFX::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    drawLine(x, y, x, y+h-1, color);
}

void
GFX::drawLine (int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
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
            setPixel(y0, x0, color);
        } else {
            setPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void
GFX::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void
GFX::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    setPixel(x0  , y0+r, color);
    setPixel(x0  , y0-r, color);
    setPixel(x0+r, y0  , color);
    setPixel(x0-r, y0  , color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;

        ddF_x += 2;
        f += ddF_x;

        setPixel(x0 + x, y0 + y, color);
        setPixel(x0 - x, y0 + y, color);
        setPixel(x0 + x, y0 - y, color);
        setPixel(x0 - x, y0 - y, color);
        setPixel(x0 + y, y0 + x, color);
        setPixel(x0 - y, y0 + x, color);
        setPixel(x0 + y, y0 - x, color);
        setPixel(x0 - y, y0 - x, color);
    }
}

void
GFX::setCursor (int16_t x, int16_t y) {
    m_cursorX = x;
    m_cursorY = y;
}

void
GFX::setTextColor (uint16_t textColor, uint16_t textBGColor) {
    m_textColor   = textColor;
    m_textBGColor = textBGColor;
}

void
GFX::setTextSize (uint8_t size) {
    m_textSize = (size > 0) ? size : 1;
}

void
GFX::setTextWrap (uint8_t wrap) {
    m_wrap = wrap;
}

void
GFX::drawChar (int16_t x, int16_t y, uint8_t data, uint16_t color, uint16_t bg, uint8_t size) {
    if( (x >= m_width)            || // Clip right
        (y >= m_height)           || // Clip bottom
        ((x + 6 * size - 1) < 0)  || // Clip left
        ((y + 8 * size - 1) < 0))    // Clip top
    return;

    for (int8_t i=0; i<6; i++ ) {
        uint8_t line;
        if (i == 5) {
            line = 0x0;
        } else {
            line = *(m_font+(data * 5)+i);
            for (int8_t j = 0; j<8; j++) {
                if (line & 0x1) {
                    if (size == 1) // default size
                        setPixel (x+i, y+j, color);
                    else {  // big size
                        fillRect (x+(i*size), y+(j*size), size, size, color);
                    }
                } else if (bg != color) {
                    if (size == 1) // default size
                        setPixel (x+i, y+j, bg);
                    else {  // big size
                        fillRect (x+i*size, y+j*size, size, size, bg);
                    }
                }
                line >>= 1;
            }
        }
    }
}

void
GFX::print (std::string msg) {
    int len = msg.length();

    for (int idx = 0; idx < len; idx++) {
        if (msg[idx] == '\n') {
            m_cursorY += m_textSize * 8;
            m_cursorX  = 0;
        } else if (msg[idx] == '\r') {
            // skip em
        } else {
            drawChar(m_cursorX, m_cursorY, msg[idx], m_textColor, m_textBGColor, m_textSize);
            m_cursorX += m_textSize * 6;
            if (m_wrap && (m_textColor > (m_width - m_textSize * 6))) {
                m_cursorY += m_textSize * 8;
                m_cursorX = 0;
            }
        }
    }
}
