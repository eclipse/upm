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
     * Draw an image, see examples/python/make_oled_pic.py for an
     * explanation on how the pixels are mapped to bytes
     *
     * @param data the buffer to write
     * @param bytes the number of bytes to write
     * @return result of operation
     */
    mraa::Result draw(uint8_t* data, int bytes);

    /**
     * Write a string to LCD
     *
     * @param msg the std::string to write to display, note only ascii 
     * chars are supported
     * @return result of operation
     */
    mraa::Result write(std::string msg);

    /**
     * Set cursor to a coordinate
     *
     * @param row the row to set cursor to.  This device supports 6 rows.
     * @param column the column to set cursor to. This device support
     * 64 columns
     * @return result of operation
     */
    mraa::Result setCursor(int row, int column);

    /**
     * Clear display
     *
     * @return result of operation
     */
    mraa::Result clear();

    /**
     * Return to coordinate 0,0
     *
     * @return result of operation
     */
    mraa::Result home();

  protected:
    mraa::Result command(uint8_t cmd);
    mraa::Result data(uint8_t data);
    mraa::Result writeChar(uint8_t value);
    mraa::Result setAddressingMode(displayAddressingMode mode);

  private:
    mraa::Gpio m_gpioCD;        // command(0)/data(1)
    mraa::Gpio m_gpioRST;       // reset pin

    mraa::Spi m_spi;
  };
}
