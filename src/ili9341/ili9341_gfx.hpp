/**
 * Author: Shawn Hymel
 * Copyright (c) 2016 SparkFun Electronics
 *
 * Based on GFX interface by Yevgeniy Kiveisha and Adafruit Industries.
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

#include <mraa.hpp>

#define adagfxswap(a, b) { int16_t t = a; a = b; b = t; }

namespace upm 
{
    /**
     * @brief GFX helper class
     */
    class GFX {
        public:
        
            /**
             * Creates a GFX object
             *
             * @param w Screen width
             * @param h Screen height
             */
            GFX(int16_t w, int16_t h);

            /**
             * Empyt virtual destructor
             */
            virtual ~GFX() {};
             
            /**
             * Sends a pixel color (RGB) to the driver chip. This must be
             * defined by the subclass (pure virtual function).
             *
             * @param x Axis on the horizontal scale
             * @param y Axis on the vertical scale
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;
            
            /**
             * Draw a line.
             *
             * @param x0 Start of line x coordinate
             * @param y0 Start of line y coordinate
             * @param x1 End of line x coordinate
             * @param y1 End of line y coordinate
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */            
            virtual void drawLine(int16_t x0, 
                                  int16_t y0, 
                                  int16_t x1, 
                                  int16_t y1, 
                                  uint16_t color);
            
            /**
             * Draws a vertical line using minimal SPI writes.
             *
             * @param x Axis on the horizontal scale to begin line
             * @param y Axis on the vertical scale to begin line
             * @param h Height of line in pixels
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            virtual void drawFastVLine(int16_t x, 
                                       int16_t y, 
                                       int16_t h, 
                                       uint16_t color);
            
            /**
             * Draws a horizontal line using  minimal SPI writes.
             *
             * @param x Axis on the horizontal scale to begin line
             * @param y Axis on the vertical scale to begin line
             * @param w Width of line in pixels
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            virtual void drawFastHLine(int16_t x, 
                                       int16_t y, 
                                       int16_t w, 
                                       uint16_t color);
    
            /**
             * Draws a rectangle (not filled).
             *
             * @param x Position of upper left corner on horizontal axis
             * @param y Position of upper left corner on vertical axis
             * @param w Width of rectangle
             * @param h Height of rectangle
             * @color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            virtual void drawRect(int16_t x, 
                                  int16_t y, 
                                  int16_t w, 
                                  int16_t h, 
                                  uint16_t color);
                                 
            /**
             * Draw a filled rectangle.
             *
             * @param x Axis on the horizontal scale of upper-left corner
             * @param y Axis on the vertical scale of upper-left corner
             * @param w Width of rectangle in pixels
             * @param h Height of rectangle in pixels
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */ 
            virtual void fillRect(int16_t x, 
                                  int16_t y, 
                                  int16_t w, 
                                  int16_t h, 
                                  uint16_t color);
    
            /**
             * Fill the screen with a single color.
             *
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            virtual void fillScreen(uint16_t color);
    
            /**
             * Invert colors on the display.
             *
             * @param i True or false to invert colors
             */
            virtual void invertDisplay(bool i);
            
            /**
             * Draw a circle outline.
             *
             * @param x0 Center point of circle on x-axis
             * @param y0 Center point of circle on y-axis
             * @param r Radius of circle
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
            
            /**
             * Used to draw rounded corners.
             *
             * @param x0 Center point of circle on x-axis
             * @param y0 Center point of circle on y-axis
             * @param r Radius of circle
             * @param cornername Mask of corner number (1, 2, 4, 8)
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            void drawCircleHelper(int16_t x0, 
                                  int16_t y0, 
                                  int16_t r, 
                                  uint8_t cornername,
                                  uint16_t color);

            /**
             * Draws a filled circle.
             *
             * @param x0 Center point of circle on x-axis
             * @param y0 Center point of circle on y-axis
             * @param r Radius of circle
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */    
            void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    
            /**
             * Used to draw a filled circle and rounded rectangles.
             *
             * @param x0 Center point of circle on x-axis
             * @param y0 Center point of circle on y-axis
             * @param r Radius of circle
             * @param cornername Mask of corner number (1, 2, 4, 8)
             * @param delta Line offset
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            void fillCircleHelper(int16_t x0, 
                                  int16_t y0, 
                                  int16_t r, 
                                  uint8_t cornername,
                                  int16_t delta,
                                  uint16_t color);
                                  
            /**
             * Draw a triangle.
             *
             * @param x0 First point coordinate on x-axis
             * @param y0 First point coordinate on y-axis
             * @param x1 Second point coordinate on x-axis
             * @param y1 Second point coordinate on y-axis
             * @param x2 Third point coordinate on x-axis
             * @param y2 Third point coordinate on y-axis
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            void drawTriangle(int16_t x0, 
                              int16_t y0, 
                              int16_t x1, 
                              int16_t y1,
                              int16_t x2, 
                              int16_t y2, 
                              uint16_t color);
                              
            /**
             * Draw a filled triangle.
             *
             * @param x0 First point coordinate on x-axis
             * @param y0 First point coordinate on y-axis
             * @param x1 Second point coordinate on x-axis
             * @param y1 Second point coordinate on y-axis
             * @param x2 Third point coordinate on x-axis
             * @param y2 Third point coordinate on y-axis
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            void fillTriangle(int16_t x0,
                              int16_t y0,
                              int16_t x1,
                              int16_t y1,
                              int16_t x2,
                              int16_t y2,
                              uint16_t color);

            /**
             * Draw a rectangle with rounded corners
             *
             * @param x0 X-axis coordinate of top-left corner
             * @param y0 Y-axis coordinate of top-left corner
             * @param w Width of rectangle
             * @param h height of rectangle
             * @param radius Radius of rounded corners
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            void drawRoundRect(int16_t x0, 
                               int16_t y0, 
                               int16_t w, 
                               int16_t h,
                               int16_t radius, 
                               uint16_t color);
                               
            /**
             * Draw a filled rectangle with rounded corners
             *
             * @param x0 X-axis coordinate of top-left corner
             * @param y0 Y-axis coordinate of top-left corner
             * @param w Width of rectangle
             * @param h height of rectangle
             * @param radius Radius of rounded corners
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            void fillRoundRect(int16_t x0, 
                               int16_t y0, 
                               int16_t w, 
                               int16_t h,
                               int16_t radius, 
                               uint16_t color);
                               
            /**
             * Draw a character at the specified point.
             *
             * @param x X-axis coordinate of the top-left corner
             * @param y Y-axis coordinate of the top-left corner
             * @param c Character to draw
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             * @param bg Background color (16-bit RGB)
             * @param size Font size
             */
            void drawChar(int16_t x, 
                          int16_t y, 
                          unsigned char c, 
                          uint16_t color,
                          uint16_t bg, 
                          uint8_t size);
            
            /**
             * Get the x-axis coordinate of the upper-left corner of the cursor.
             *
             * @return X-axis coordinate of the cursor
             */
            int16_t getCursorX(void) const;
            
            /**
             * Get the y-axis coordinate of the upper-left corner of the cursor.
             *
             * @return Y-axis coordinate of the cursor
             */
            int16_t getCursorY(void) const;
             
            /**
             * Set the cursor for writing text.
             *
             * @param x X-axis coordinate of the top-left corner of the cursor
             * @param y Y-axis coordinate of the top-left corner of the cursor
             */
            void setCursor(int16_t x, int16_t y);
             
            /**
             * Set the color for text.
             *
             * @param c RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            void setTextColor(uint16_t c);
            
            /**
             * Set the color for text and text background (highlight).
             *
             * @param c Text color (RGB, 16-bit)
             * @param bg Background text color (RGB, 16-bit)
             */
            void setTextColor(uint16_t c, uint16_t bg);
            
            /**
             * Set the size of the text.
             *
             * @param s Font size (multiples of 8 pixel text height)
             */ 
            void setTextSize(uint8_t s);
            
            /**
             * Enable or disable text wrapping.
             *
             * @param w True to wrap text. False to truncate.
             */
            void setTextWrap(bool w);
            
            /**
             * Get the current rotation configuration of the screen.
             *
             * @return current rotation 0-3
             */
            uint8_t getRotation(void) const;
            
            /**
             * Sets the rotation of the screen. Can be overridden with another
             * screen-specific definition.
             *
             * @param r Rotation 0-3
             */
            void setRotation(uint8_t r);
                        
            /**
             * Enable (or disable) Code Page 437-compatible charset.
             *
             * @param x True to enable CP437 charset. False to disable.
             */
            void cp437(bool x);
            
            /**
             * Write a character at the current cursor position. Definition
             * can be overridden with board-specific code.
             *
             * @param c Character to draw
             */
            virtual void write(uint8_t c);
            
            /**
             * Prints a string to the screen.
             *
             * @param s Message to print
             */
            void print(std::string msg);
            
            /**
             * Get the current width of the screen.
             *
             * @return the width in pixels
             */
            int16_t width(void) const;
            
            /**
             * Get the current height of the screen.
             *
             * @return the height in pixels
             */
            int16_t height(void) const;
            
        protected:
        
            const int16_t WIDTH;
            const int16_t HEIGHT;
            
            int16_t _width;
            int16_t _height;
            
            uint8_t rotation;
            
            uint16_t textcolor;
            uint16_t textbgcolor;
            int16_t cursor_x;
            int16_t cursor_y;
            uint8_t textsize;
            bool wrap;
            bool _cp437;
            static const unsigned char font[];
    };
}     
