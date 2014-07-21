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
#include <mraa.h>

#define swap(a, b) { int16_t t = a; a = b; b = t; }

namespace upm {

/**
 * @brief GFX helper class
 *
 * This file used by the screen.
 */
class GFX {
    public:
        /**
         * Instanciates a GFX object
         *
         * @param width screen width
         * @param height screen height
         * @param screenBuffer pointer to screen buffer
         * @param font pointer to font map
         */
        GFX (int width, int height, uint8_t * screenBuffer, const unsigned char * font);

        /**
         * GFX object destructor
         */
        ~GFX ();

        /**
         * Set the window address
         *
         * @param x0 first coordinate
         * @param y0 first coordinate
         * @param x1 second coordinate
         * @param y1 second coordinate
         */
        virtual void setAddrWindow (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) = 0;

        /**
         * Send pixel collor (RGB) to the chip. Must be implemented by
         * inherited class.
         *
         * @param x axis on horizontal scale
         * @param y axis on vertical scale
         * @param color rgb value
         */
        virtual void drawPixel (int16_t x, int16_t y, uint16_t color) = 0;

        /**
         * Copy the buffer to the chip via SPI interface.
         */
        virtual void refresh () = 0;

        /**
         *
         *
         * @param x axis on horizontal scale
         * @param y axis on vertical scale
         * @param data character to write
         * @param color character's color
         * @param bg character's background color
         * @param size size of the font
         */
        void drawChar (int16_t x, int16_t y, uint8_t data, uint16_t color, uint16_t bg, uint8_t size);

        /**
         * Print the message to the screen
         *
         * @param msg message which will be printed
         */
        void print (std::string msg);

        /**
         * Print the message to the screen
         *
         * @param x axis on horizontal scale
         * @param y axis on vertical scale
         * @param color pixel's color
         */
        mraa_result_t setPixel (int x, int y, uint16_t color);

        /**
         * Fill screen with selected color
         *
         * @param color selected's color
         */
        void fillScreen (uint16_t color);

        /**
         * Fill rectangle with selected color
         *
         * @param x axis on horizontal scale (top left corner)
         * @param y axis on vertical scale (top left corner)
         * @param w distanse from x
         * @param h distanse from y
         * @param color selected color
         */
        void fillRect (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

        /**
         * Draw line in vertical scale.
         *
         * @param x axis on horizontal scale
         * @param y axis on vertical scale
         * @param h distanse from y
         * @param color selected color
         */
        void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

        /**
         * Draw line from coordinate C0 to coordinate C1
         *
         * @param x0 first coordinate
         * @param y0 first coordinate
         * @param x1 second coordinate
         * @param y1 second coordinate
         * @param color selected color
         */
        void drawLine (int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

        /**
         * Draw a triangle
         *
         * @param x0 first coordinate
         * @param y0 first coordinate
         * @param x1 second coordinate
         * @param y1 second coordinate
         * @param x2 third coordinate
         * @param y2 third coordinate
         * @param color selected color
         */
        void drawTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

        /**
         * Draw a circle
         *
         * @param x center of circle on X scale
         * @param y center of circle on Y scale
         * @param r radius of circle
         * @param color color of circle
         */
        void drawCircle (int16_t x, int16_t y, int16_t r, uint16_t color);

        /**
         * Set cursor for text message
         *
         * @param x axis on horizontal scale
         * @param y axis on vertical scale
         */
        void setCursor (int16_t x, int16_t y);

        /**
         * Set text color for the message
         *
         * @param textColor font color
         * @param textBGColor background color
         */
        void setTextColor (uint16_t textColor, uint16_t textBGColor);

        /**
         * Set the size of the font
         *
         * @param size font size
         */
        void setTextSize (uint8_t size);

        /**
         * Wrap printed message.
         *
         * @param wrap true (0x1) or false (0x0)
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
