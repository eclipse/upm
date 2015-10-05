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

#include <stdexcept>
#include <string>
#include <unistd.h>

#include "hd44780_bits.h"
#include "ssd1327.h"

using namespace upm;

#define INIT_SLEEP 50000
#define CMD_SLEEP 10000

SSD1327::SSD1327(int bus_in, int addr_in) : m_i2c_lcd_control(bus_in)
{
    mraa::Result error = mraa::SUCCESS;

    m_lcd_control_address = addr_in;
    m_name = "SSD1327";

    error = m_i2c_lcd_control.address(m_lcd_control_address);
    if (error != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": I2c.address() failed");
        return;
    }

    usleep(INIT_SLEEP);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0xFD); // Unlock OLED driver IC MCU
                                               // interface from entering command.
                                               // i.e: Accept commands
    usleep(INIT_SLEEP);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0x12);
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0xAE); // Set display off
    usleep(INIT_SLEEP);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0xA8); // set multiplex ratio
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x5F); // 96
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0xA1); // set display start line
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x00); //
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0xA2); // set display offset
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x60);
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0xA0); // set remap
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x46);
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0xAB); // set vdd internal
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x01); //
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x81); // set contrasr
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x53); // 100 nit
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0xB1); // Set Phase Length
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0X51); //
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0xB3); // Set Display Clock Divide Ratio/Oscillator
                                                       // Frequency
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x01); //
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0xB9); //
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0xBC); // set pre_charge
                                                       // voltage/VCOMH
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x08); // (0x08);
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0xBE); // set VCOMH
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0X07); // (0x07);
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0xB6); // Set second pre-charge
                                                       // period
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x01); //
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0xD5); // enable second precharge and enternal vsl
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0X62); // (0x62);
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0xA4); // Set Normal Display Mode
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x2E); // Deactivate Scroll
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0xAF); // Switch on display
    usleep(INIT_SLEEP);

    // Row Address
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x75); // Set Row Address
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x00); // Start 0
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x5f); // End 95
    usleep(INIT_SLEEP);

    // Column Address
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x15); // Set Column Address
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x08); // Start from 8th Column of
                                                       // driver IC. This is 0th
                                                       // Column for OLED
    usleep(INIT_SLEEP);
    error = m_i2c_lcd_control.writeReg(LCD_CMD, 0x37); // End at  (8 + 47)th
                                                       // column. Each Column has 2
                                                       // pixels(segments)
    usleep(INIT_SLEEP);

    clear();
    setNormalDisplay();
    setVerticalMode();
}

SSD1327::~SSD1327()
{
}

mraa::Result
SSD1327::draw(uint8_t* data, int bytes)
{
    mraa::Result error = mraa::SUCCESS;

    setHorizontalMode();
    for (int row = 0; row < bytes; row++) {
        for (uint8_t col = 0; col < 8; col += 2) {
            uint8_t value = 0x0;

            uint8_t bitOne = (data[row] << col) & 0x80;
            uint8_t bitTwo = (data[row] << (col + 1)) & 0x80;

            value |= (bitOne) ? grayHigh : 0x00;
            value |= (bitTwo) ? grayLow : 0x00;

            m_i2c_lcd_control.writeReg(LCD_DATA, value);
            usleep(CMD_SLEEP - 2000);
        }
    }

    return error;
}

/*
 * **************
 *  virtual area
 * **************
 */
mraa::Result
SSD1327::write(std::string msg)
{
    mraa::Result error = mraa::SUCCESS;

    setVerticalMode();
    for (std::string::size_type i = 0; i < msg.size(); ++i) {
        writeChar(msg[i]);
    }

    return error;
}

mraa::Result
SSD1327::setCursor(int row, int column)
{
    mraa::Result error = mraa::SUCCESS;

    // Column Address
    m_i2c_lcd_control.writeReg(LCD_CMD, 0x15); /* Set Column Address */
    usleep(CMD_SLEEP);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0x08 + (column * 4)); /* Start Column:
                                                                                  Start from 8 */
    usleep(CMD_SLEEP);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0x37); /* End Column */
    usleep(CMD_SLEEP);
    // Row Address
    m_i2c_lcd_control.writeReg(LCD_CMD, 0x75); /* Set Row Address */
    usleep(CMD_SLEEP);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0x00 + (row * 8)); /* Start Row*/
    usleep(CMD_SLEEP);
    m_i2c_lcd_control.writeReg(LCD_CMD, 0x07 + (row * 8)); /* End Row*/
    usleep(CMD_SLEEP);

    return error;
}

mraa::Result
SSD1327::clear()
{
    mraa::Result error = mraa::SUCCESS;
    uint8_t columnIdx, rowIdx;

    for (rowIdx = 0; rowIdx < 12; rowIdx++) {
        // clear all columns
        for (columnIdx = 0; columnIdx < 12; columnIdx++) {
            writeChar(' ');
        }
    }

    return mraa::SUCCESS;
}

mraa::Result
SSD1327::home()
{
    return setCursor(0, 0);
}

void
SSD1327::setGrayLevel(uint8_t level)
{
    grayHigh = (level << 4) & 0xF0;
    grayLow = level & 0x0F;
}

/*
 * **************
 *  private area
 * **************
 */
mraa::Result
SSD1327::writeChar(uint8_t value)
{
    mraa::Result rv = mraa::SUCCESS;
    if (value < 0x20 || value > 0x7F) {
        value = 0x20; // space
    }

    for (uint8_t row = 0; row < 8; row = row + 2) {
        for (uint8_t col = 0; col < 8; col++) {
            uint8_t data = 0x0;

            uint8_t bitOne = ((BasicFont[value - 32][row]) >> col) & 0x1;
            uint8_t bitTwo = ((BasicFont[value - 32][row + 1]) >> col) & 0x1;

            data |= (bitOne) ? grayHigh : 0x00;
            data |= (bitTwo) ? grayLow : 0x00;

            rv = m_i2c_lcd_control.writeReg(LCD_DATA, data);
            usleep(CMD_SLEEP - 2000);
        }
    }
    return rv;
}

mraa::Result
SSD1327::setNormalDisplay()
{
    return m_i2c_lcd_control.writeReg(LCD_CMD,
                                      DISPLAY_CMD_SET_NORMAL); // set to normal display '1' is ON
}

mraa::Result
SSD1327::setHorizontalMode()
{
    mraa::Result rv = mraa::SUCCESS;
    rv = m_i2c_lcd_control.writeReg(LCD_CMD, 0xA0); // remap to
    usleep(CMD_SLEEP);
    rv = m_i2c_lcd_control.writeReg(LCD_CMD, 0x42); // horizontal mode
    usleep(CMD_SLEEP);

    // Row Address
    rv = m_i2c_lcd_control.writeReg(LCD_CMD, 0x75); // Set Row Address
    usleep(CMD_SLEEP);
    rv = m_i2c_lcd_control.writeReg(LCD_CMD, 0x00); // Start 0
    usleep(CMD_SLEEP);
    rv = m_i2c_lcd_control.writeReg(LCD_CMD, 0x5f); // End 95
    usleep(CMD_SLEEP);

    // Column Address
    rv = m_i2c_lcd_control.writeReg(LCD_CMD, 0x15); // Set Column Address
    usleep(CMD_SLEEP);
    rv = m_i2c_lcd_control.writeReg(LCD_CMD, 0x08); // Start from 8th Column of driver
                                               // IC. This is 0th Column for OLED
    usleep(CMD_SLEEP);
    rv = m_i2c_lcd_control.writeReg(LCD_CMD, 0x37); // End at  (8 + 47)th column. Each
                                               // Column has 2 pixels(or segments)
    usleep(CMD_SLEEP);
    return rv;
}

mraa::Result
SSD1327::setVerticalMode()
{
    mraa::Result rv = mraa::SUCCESS;
    rv = m_i2c_lcd_control.writeReg(LCD_CMD, 0xA0); // remap to
    usleep(CMD_SLEEP);
    rv = m_i2c_lcd_control.writeReg(LCD_CMD, 0x46); // Vertical mode
    usleep(CMD_SLEEP);
    return rv;
}
