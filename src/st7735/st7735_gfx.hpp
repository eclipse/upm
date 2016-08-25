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
#pragma once

#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include <mraa.hpp>

#define swap(a, b) { int16_t t = a; a = b; b = t; }

namespace upm {

/**
 * @brief GFX helper class
 *
 * This file is used by the screen.
 */
class GFX {
    public:
        /**
         * Instantiates a GFX object
         *
         * @param width Screen width
         * @param height Screen height
         * @param screenBuffer Pointer to the screen buffer
         * @param font Pointer to the font map
         */
        GFX (int width, int height, uint8_t * screenBuffer, const unsigned char * font);

        /**
         * GFX object destructor
         */
        ~GFX ();

        /**
         * Sets the window address
         *
         * @param x0 First coordinate
         * @param y0 First coordinate
         * @param x1 Second coordinate
         * @param y1 Second coordinate
         */
        virtual void setAddrWindow (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) = 0;

        /**
         * Sends a pixel color (RGB) to the chip. Must be implemented by the
         * inherited class.
         *
         * @param x Axis on the horizontal scale
         * @param y Axis on the vertical scale
         * @param color RGB value
         */
        virtual void drawPixel (int16_t x, int16_t y, uint16_t color) = 0;

        /**
         * Copies the buffer to the chip via the SPI.
         */
        virtual void refresh () = 0;

        /**
         *
         *
         * @param x Axis on the horizontal scale
         * @param y Axis on the vertical scale
         * @param data Character to write
         * @param color Character color
         * @param bg Character background color
         * @param size Size of the font
         */
        void drawChar (int16_t x, int16_t y, uint8_t data, uint16_t color, uint16_t bg, uint8_t size);

        /**
         * Prints a message on the screen
         *
         * @param msg Message to print
         */
        void print (std::string msg);

        /**
         * Prints a message on the screen
         *
         * @param x Axis on the horizontal scale
         * @param y Axis on the vertical scale
         * @param color Pixel color
         */
        mraa::Result setPixel (int x, int y, uint16_t color);

        /**
         * Fills the screen with a selected color
         *
         * @param color Selected color
         */
        void fillScreen (uint16_t color);

        /**
         * Fills a rectangle with a selected color
         *
         * @param x Axis on the horizontal scale (top-left corner)
         * @param y Axis on the vertical scale (top-left corner)
         * @param w Distanse from x
         * @param h Distanse from y
         * @param color Selected color
         */
        void fillRect (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

        /**
         * Draws a line on the vertical scale
         *
         * @param x Axis on the horizontal scale
         * @param y Axis on the vertical scale
         * @param h Distanse from y
         * @param color Selected color
         */
        void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

        /**
         * Draws a line from coordinate C0 to coordinate C1
         *
         * @param x0 First coordinate
         * @param y0 First coordinate
         * @param x1 Second coordinate
         * @param y1 Second coordinate
         * @param color selected color
         */
        void drawLine (int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

        /**
         * Draws a triangle
         *
         * @param x0 First coordinate
         * @param y0 First coordinate
         * @param x1 Second coordinate
         * @param y1 Second coordinate
         * @param x2 Third coordinate
         * @param y2 Third coordinate
         * @param color Selected color
         */
        void drawTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

        /**
         * Draws a circle
         *
         * @param x Center of the circle on the horizontal scale
         * @param y Center of the circle on the vertical scale
         * @param r Radius of the circle
         * @param color Color of the circle
         */
        void drawCircle (int16_t x, int16_t y, int16_t r, uint16_t color);

        /**
         * Sets the cursor for a text message
         *
         * @param x Axis on the horizontal scale
         * @param y Axis on the vertical scale
         */
        void setCursor (int16_t x, int16_t y);

        /**
         * Sets a text color for a message
         *
         * @param textColor Font color
         * @param textBGColor Background color
         */
        void setTextColor (uint16_t textColor, uint16_t textBGColor);

        /**
         * Sets the size of the font
         *
         * @param size Font size
         */
        void setTextSize (uint8_t size);

        /**
         * Wraps a printed message
         *
         * @param wrap True (0x1) or false (0x0)
         */
        void setTextWrap (uint8_t wrap);

        int m_height; /**< Screen height */
        int m_width; /**< Screen width */
        int m_textSize; /**< Printed text size */
        int m_textColor; /**< Printed text color */
        int m_textBGColor; /**< Printed text background color */
        int m_cursorX; /**< Cursor X coordinate */
        int m_cursorY; /**< Cursor Y coordinate */
        int m_wrap; /**< Wrapper flag (true or false) */

        uint8_t * m_map; /**< Screens buffer */

    protected:
        const int16_t   WIDTH, HEIGHT;
        const unsigned char * m_font;
    };
}
