/*
 * Author: Marc Graham <marc@m2ag.net>
 * Copyright (c) 2015 Intel Corporation
 *
 * Adapted from ssd1308 library.
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

#include <string>
#include <unistd.h>

#include "hd44780_bits.h"
#include "ssd1306.h"

using namespace upm;

SSD1306::SSD1306(int bus_in, int addr_in) : m_i2c_lcd_control(bus_in)
{
    int vccstate = SSD1306_SWITCHCAPVCC;
    _vccstate = vccstate;

    int LCD_CMD = 0x00;

    m_lcd_control_address = addr_in;
    m_name = "SSD1306";

    mraa::Result error = m_i2c_lcd_control.address(m_lcd_control_address);
    
    if (error != mraa::SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                ": mraa_i2c_address() failed");
        return;
    }
    
    error = m_i2c_lcd_control.frequency(mraa::I2C_FAST);
    
    if (error != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                ": mraa_i2c_frequency(MRAA_I2C_FAST) failed");
        return;
    }

    m_i2c_lcd_control.writeReg(LCD_CMD, DISPLAY_CMD_OFF); // display off
    usleep(4500);
    //ADD 1306 stuff
    // Init sequence for 128x64 OLED module                 // 0xAE
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    m_i2c_lcd_control.writeReg(LCD_CMD, 0x80);                                  // the suggested ratio 0x80
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_SETMULTIPLEX);                  // 0xA8
    m_i2c_lcd_control.writeReg(LCD_CMD, 0x3F);
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_SETDISPLAYOFFSET);              // 0xD3
    m_i2c_lcd_control.writeReg(LCD_CMD, 0x0);                                   // no offset
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_SETSTARTLINE | 0x0);            // line #0
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_CHARGEPUMP);                    // 0x8D
    if (vccstate == SSD1306_EXTERNALVCC) {
        m_i2c_lcd_control.writeReg(LCD_CMD, 0x10);
    } else {
        m_i2c_lcd_control.writeReg(LCD_CMD,0x14);
    }
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_MEMORYMODE);                    // 0x20
    m_i2c_lcd_control.writeReg(LCD_CMD, 0x00);                                  // 0x0 act like ks0108
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_SEGREMAP | 0x1);
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_COMSCANDEC);
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_SETCOMPINS);                    // 0xDA
    m_i2c_lcd_control.writeReg(LCD_CMD, 0x12);
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_SETCONTRAST);                   // 0x81
    if (vccstate == SSD1306_EXTERNALVCC) {
        m_i2c_lcd_control.writeReg(LCD_CMD, 0x9F);
    } else {
        m_i2c_lcd_control.writeReg(LCD_CMD, 0xCF);
    }
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_SETPRECHARGE);                  // 0xd9
    if (vccstate == SSD1306_EXTERNALVCC) {
      m_i2c_lcd_control.writeReg(LCD_CMD, 0x22);
    } else {
      m_i2c_lcd_control.writeReg(LCD_CMD,0xF1);
    }
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_SETVCOMDETECT);                 // 0xDB
    m_i2c_lcd_control.writeReg(LCD_CMD, 0x40);
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    m_i2c_lcd_control.writeReg(LCD_CMD, DISPLAY_CMD_SET_NORMAL_1306);                 // 0xA6

    //END 1306 Stuff
    m_i2c_lcd_control.writeReg(LCD_CMD, DISPLAY_CMD_ON); // display on
    usleep(4500);
    setNormalDisplay(); // set to normal display '1' is ON

    clear();
    setAddressingMode(PAGE);
}

SSD1306::~SSD1306()
{
}

mraa::Result
SSD1306::draw(uint8_t* data, int bytes)
{
    mraa::Result error = mraa::SUCCESS;

    setAddressingMode(HORIZONTAL);
    for (int idx = 0; idx < bytes; idx++) {
        m_i2c_lcd_control.writeReg(LCD_DATA, data[idx]);
    }

    return error;
}

/*
 * **************
 *  virtual area
 * **************
 */
mraa::Result
SSD1306::write(std::string msg)
{
    mraa::Result error = mraa::SUCCESS;

    setAddressingMode(PAGE);
    for (std::string::size_type i = 0; i < msg.size(); ++i) {
        writeChar(msg[i]);
    }

    return error;
}

mraa::Result
SSD1306::setCursor(int row, int column)
{
    mraa::Result error = mraa::SUCCESS;

    error = m_i2c_lcd_control.writeReg(LCD_CMD, BASE_PAGE_START_ADDR + row); // set page address
    error = m_i2c_lcd_control.writeReg(LCD_CMD,
                                       BASE_LOW_COLUMN_ADDR + (8 * column & 0x0F)); // set column
                                                                                    // lower address
    error = m_i2c_lcd_control.writeReg(LCD_CMD,
                                       BASE_HIGH_COLUMN_ADDR +
                                       ((8 * column >> 4) & 0x0F)); // set column higher address

    return error;
}

mraa::Result
SSD1306::clear()
{
    mraa::Result error = mraa::SUCCESS;
    uint8_t columnIdx, rowIdx;

    m_i2c_lcd_control.writeReg(LCD_CMD, DISPLAY_CMD_OFF); // display off
    for (rowIdx = 0; rowIdx < 8; rowIdx++) {
        setCursor(rowIdx, 0);

        // clear all columns
        for (columnIdx = 0; columnIdx < 16; columnIdx++) {
            writeChar(' ');
        }
    }
    m_i2c_lcd_control.writeReg(LCD_CMD, DISPLAY_CMD_ON); // display on
    home();

    return error;
}

mraa::Result
SSD1306::home()
{
    return setCursor(0, 0);
}

/*
 * **************
 *  private area
 * **************
 */
mraa::Result
SSD1306::writeChar(uint8_t value)
{
    mraa::Result rv;
    if (value < 0x20 || value > 0x7F) {
        value = 0x20; // space
    }

    for (uint8_t idx = 0; idx < 8; idx++) {
        rv = m_i2c_lcd_control.writeReg(LCD_DATA, BasicFont[value - 32][idx]);
    }

    return rv;
}

mraa::Result
SSD1306::setNormalDisplay()
{
    return m_i2c_lcd_control.writeReg(LCD_CMD,
                                      DISPLAY_CMD_SET_NORMAL_1306); // set to normal display '1' is
                                                                    // ON
}

mraa::Result
SSD1306::setAddressingMode(displayAddressingMode mode)
{
    mraa::Result rv;
    rv =m_i2c_lcd_control.writeReg(LCD_CMD, DISPLAY_CMD_MEM_ADDR_MODE); // set addressing mode
    rv =m_i2c_lcd_control.writeReg(LCD_CMD, mode);                      // set page addressing mode
    return rv;
}


mraa::Result
SSD1306::invert(bool i)
{
    mraa::Result rv;
    if(i){
        rv = m_i2c_lcd_control.writeReg(LCD_CMD,  DISPLAY_CMD_SET_INVERT_1306);
    } else {
        rv = m_i2c_lcd_control.writeReg(LCD_CMD,  DISPLAY_CMD_SET_NORMAL_1306);
    }
    return rv;
}


void SSD1306::startscrollright(uint8_t start, uint8_t stop){
    m_i2c_lcd_control.writeReg(LCD_CMD,SSD1306_RIGHT_HORIZONTAL_SCROLL);
    m_i2c_lcd_control.writeReg(LCD_CMD,0X00);
    m_i2c_lcd_control.writeReg(LCD_CMD,start);
    m_i2c_lcd_control.writeReg(LCD_CMD,0X00);
    m_i2c_lcd_control.writeReg(LCD_CMD,stop);
    m_i2c_lcd_control.writeReg(LCD_CMD,0X00);
    m_i2c_lcd_control.writeReg(LCD_CMD,0XFF);
    m_i2c_lcd_control.writeReg(LCD_CMD,SSD1306_ACTIVATE_SCROLL);
}


void SSD1306::startscrollleft(uint8_t start, uint8_t stop){
    m_i2c_lcd_control.writeReg(LCD_CMD,SSD1306_LEFT_HORIZONTAL_SCROLL);
    m_i2c_lcd_control.writeReg(LCD_CMD,0X00);
    m_i2c_lcd_control.writeReg(LCD_CMD,start);
    m_i2c_lcd_control.writeReg(LCD_CMD,0X00);
    m_i2c_lcd_control.writeReg(LCD_CMD,stop);
    m_i2c_lcd_control.writeReg(LCD_CMD,0X00);
    m_i2c_lcd_control.writeReg(LCD_CMD,0XFF);
    m_i2c_lcd_control.writeReg(LCD_CMD,SSD1306_ACTIVATE_SCROLL);
}

void SSD1306::startscrolldiagright(uint8_t start, uint8_t stop){
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_SET_VERTICAL_SCROLL_AREA);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0X00);
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_LCDHEIGHT);
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0X00);
    m_i2c_lcd_control.writeReg(LCD_CMD, start);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0X00);
    m_i2c_lcd_control.writeReg(LCD_CMD, stop);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0X01);
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_ACTIVATE_SCROLL);
}

void SSD1306::startscrolldiagleft(uint8_t start, uint8_t stop){
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_SET_VERTICAL_SCROLL_AREA);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0X00);
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_LCDHEIGHT);
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0X00);
    m_i2c_lcd_control.writeReg(LCD_CMD, start);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0X00);
    m_i2c_lcd_control.writeReg(LCD_CMD, stop);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0X01);
    m_i2c_lcd_control.writeReg(LCD_CMD, SSD1306_ACTIVATE_SCROLL);
}

void SSD1306::stopscroll(void){
    m_i2c_lcd_control.writeReg(LCD_CMD,SSD1306_DEACTIVATE_SCROLL);
}

// Dim the display
// dim = true: display is dimmed
// dim = false: display is normal
void SSD1306::dim(bool dim) {
    uint8_t contrast;

    if (dim) {
    contrast = 0; // Dimmed display
    } else {
    if (_vccstate == SSD1306_EXTERNALVCC) {
      contrast = 0x9F;
    } else {
      contrast = 0xCF;
    }
    }
    // the range of contrast to too small to be really useful
    // it is useful to dim the display
    m_i2c_lcd_control.writeReg(LCD_CMD,SSD1306_SETCONTRAST);
    m_i2c_lcd_control.writeReg(LCD_CMD,contrast);
}
