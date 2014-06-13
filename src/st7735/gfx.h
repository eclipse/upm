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
#include <maa/maa.h>

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
         * @param width 
         * @param height
         */
        GFX (int width, int height);

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
         * 
         */
        virtual void drawPixel (int16_t x, int16_t y, uint16_t color) = 0;
        
        /**
         * 
         */
        virtual void refresh () = 0;
        
        /**
         * 
         */
        maa_result_t setPixel (int x, int y, uint16_t color);
        
        /**
         * 
         */
        void fillScreen (uint16_t color);
        
        /**
         * 
         */
        void fillRect (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        
        /**
         * 
         */
        void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
        
        /**
         * 
         */
        void drawLine (int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
        
        /**
         * 
         */
        void drawTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        
        /**
         * 
         */
        void drawCircle (int16_t x0, int16_t y0, int16_t r, uint16_t color);
        
        int m_height;
        int m_width;
        
        uint8_t m_map[160 * 128 * 2];
    };
}
