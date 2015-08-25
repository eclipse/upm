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

#include <string>
#include <mraa/spi.hpp>
#include <mraa/gpio.hpp>
#include "lcd.h"
#include "ssd.h"

#define EBOLED_DEFAULT_SPI_BUS 0
#define EBOLED_DEFAULT_CD      36
#define EBOLED_DEFAULT_RESET   48

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

namespace upm
{
  /**
   * @library i2clcd
   * @sensor eboled
   * @comname Sparkfun Edison Block OLED Display
   * @altname ssd1306
   * @type display
   * @man sparkfun
   * @web https://www.sparkfun.com/products/13035
   * @con spi
   *
   * @brief API for EBOLED spi controlled OLED display
   *
   * The EBOLED is an ssd1306 with some modifications to work as an
   * Edison Block.  It is a 64x48 pixel OLED display that connects
   * directly to an edison via it's 80-pin connector.  Edison Blocks
   * are stackable modules created by Sparkfun.
   *
   * This block has some buttons on it that can be accessed using
   * standard GPIO -- this driver only concerns itself with the
   * display.
   *
   * @snippet eboled.cxx Interesting
   */
   
  const uint8_t COLOR_WHITE     = 0x01;
  const uint8_t COLOR_BLACK     = 0x00;
  const uint8_t COLOR_XOR       = 0x02;
  const uint8_t OLED_WIDTH      = 0x40; // 64 pixels
  const uint8_t VERT_COLUMNS    = 0x20; // half width for hi/lo 16bit writes.
  const uint8_t OLED_HEIGHT     = 0x30; // 48 pixels
  const int     BUFFER_SIZE     = 192;

  class EBOLED : public LCD
  {
    // SSD commands
    typedef enum {
      CMD_SETLOWCOLUMN          = 0x00,
      CMD_EXTERNALVCC           = 0x01,
      CMD_SWITCHCAPVCC          = 0x02,
      CMD_SETHIGHCOLUMN         = 0x10,
      CMD_MEMORYADDRMODE        = 0x20,
      CMD_SETCOLUMNADDRESS      = 0x21,
      CMD_SETPAGEADDRESS        = 0x22,
      CMD_SETSTARTLINE          = 0x40, // 0x40 - 0x7f
      CMD_SETCONTRAST           = 0x81,
      CMD_CHARGEPUMP            = 0x8d, 
      CMD_SEGREMAP              = 0xa0,
      CMD_DISPLAYALLONRESUME    = 0xa4,
      CMD_DISPLAYALLON          = 0xa5,
      CMD_NORMALDISPLAY         = 0xa6,
      CMD_INVERTDISPLAY         = 0xa7,
      CMD_SETMULTIPLEX          = 0xa8,
      CMD_DISPLAYOFF            = 0xae,
      CMD_DISPLAYON             = 0xaf,
      CMD_SETPAGESTARTADDR      = 0xb0, // 0xb0-0xb7
      CMD_COMSCANINC            = 0xc0,
      CMD_COMSCANDEC            = 0xc8,
      CMD_SETDISPLAYOFFSET      = 0xd3,
      CMD_SETDISPLAYCLOCKDIV    = 0xd5,
      CMD_SETPRECHARGE          = 0xd9,
      CMD_SETCOMPINS            = 0xda,
      CMD_SETVCOMDESELECT       = 0xdb
    } SSD_CMDS_T;
       
  public:
    /**
     * EBOLED Constructor.  Note that you will not have any choice as
     * to the pins used, so they are all set to default values.
     *
     * @param spi spi bus to use
     * @param CD Command/Data select pin
     * @param reset reset pin
     * @param address the slave address the lcd is registered on
     */
    EBOLED(int spi=EBOLED_DEFAULT_SPI_BUS, int CD=EBOLED_DEFAULT_CD, 
           int reset=EBOLED_DEFAULT_RESET);
    
    /**
     * EBOLED Destructor
     */
    ~EBOLED();
    
    /**
     * Draw the buffer to screen, see examples/python/make_oled_pic.py for an
     * explanation on how the pixels are mapped to bytes.
     *
     * @param data the buffer to write
     * @param bytes the number of bytes to write
     * @return result of operation
     */
    mraa_result_t refresh();
    
    /**
     * Write a string to LCD
     *
     * @param msg the std::string to write to display, note only ascii 
     * chars are supported
     * @return result of operation
     */
    mraa_result_t write(std::string msg);

    /**
     * Set cursor to a coordinate
     *
     * @param y Axis on the vertical scale. This device supports 6 rows.
     * @param x Axis on the horizontal scale This device supports 64 columns
     *
     * @return result of operation
     */
    mraa_result_t setCursor (int y, int x);

    /**
      * Sets a text color for a message
      *
      * @param textColor Font color
      * @param textBGColor Background color
      */
    void setTextColor (uint8_t textColor);

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

    /**
      * Write a single character to the screen. 
      *
      * @param x Axis on the horizontal scale
      * @param y Axis on the vertical scale
      * @param data Character to write
      * @param color Character color
      * @param bg Character background color
      * @param size Size of the font
      */
    void drawChar (uint8_t x, uint8_t y, uint8_t data, uint8_t color, uint8_t size);
                                                
    /**
     * Clear display
     *
     * @return result of operation
     */
    mraa_result_t clear();
    
    void clearScreenBuffer();

    /**
     * Return to coordinate 0,0
     *
     * @return result of operation
     */
    mraa_result_t home();
    
    /**
     * Write a single pixel to the screen buffer.
     * Can do an absolutel write or toggle (xor) a pixel.
     *
     * @param x the x position of the pixel
     * @param y the y position of the pixel
     * @param color pixel is COLOR_WHITE or COLOR_BLACK
     * @param mode the draw mode DRAWMODE_NORMAL or DRAWMODE_XOR
     * @return result of operation
     */    
    void drawPixel (uint8_t x, uint8_t y, uint8_t color=COLOR_WHITE);
    
    void drawLine (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color = COLOR_WHITE); 
    
    void drawLineHorizontal (uint8_t x, uint8_t y, uint8_t width, uint8_t color = COLOR_WHITE);  
                                         
    void drawLineVertical (uint8_t x, uint8_t y, uint8_t height, uint8_t color = COLOR_WHITE); 
                          
    void drawRectangle (uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color = COLOR_WHITE);  
    
    void drawRectangleFilled (uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color = COLOR_WHITE);   
                  
    void drawTriangle (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color = COLOR_WHITE);       
                                  
    void drawCircle (int16_t x0, int16_t y0, int16_t r, uint8_t color = COLOR_WHITE);    
                        
    void fillScreen (uint8_t color=COLOR_WHITE);
    
  protected:
    mraa_result_t command(uint8_t cmd);
    mraa_result_t data(uint16_t data);
    mraa_result_t writeChar(uint8_t value);
    mraa_result_t setAddressingMode(displayAddressingMode mode);

  private:    
    mraa::Gpio m_gpioCD;        // command(0)/data(1)
    mraa::Gpio m_gpioRST;       // reset pin

    mraa::Spi m_spi;
    
    uint8_t m_cursorX;
    uint8_t m_cursorY;
    uint8_t m_textSize;
    uint8_t m_textColor;
    uint8_t m_textWrap;
  };
}
