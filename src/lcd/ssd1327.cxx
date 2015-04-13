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

#include <string>
#include <unistd.h>

#include "ssd1327.h"

using namespace upm;

#define INIT_SLEEP 50000
#define CMD_SLEEP 10000

SSD1327::SSD1327(int bus_in, int addr_in) : I2CLcd(bus_in, addr_in)
{
    mraa_result_t error = MRAA_SUCCESS;
    usleep(INIT_SLEEP);
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xFD, LCD_CMD); // Unlock OLED driver IC MCU
                                                                // interface from entering command.
                                                                // i.e: Accept commands
    usleep(INIT_SLEEP);
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x12, LCD_CMD);
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xAE, LCD_CMD); // Set display off
    usleep(INIT_SLEEP);
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xA8, LCD_CMD); // set multiplex ratio
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x5F, LCD_CMD); // 96
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xA1, LCD_CMD); // set display start line
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x00, LCD_CMD); //
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xA2, LCD_CMD); // set display offset
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x60, LCD_CMD);
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xA0, LCD_CMD); // set remap
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x46, LCD_CMD);
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xAB, LCD_CMD); // set vdd internal
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x01, LCD_CMD); //
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x81, LCD_CMD); // set contrasr
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x53, LCD_CMD); // 100 nit
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xB1, LCD_CMD); // Set Phase Length
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0X51, LCD_CMD); //
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control,
                                     0xB3,
                                     LCD_CMD); // Set Display Clock Divide Ratio/Oscillator
                                               // Frequency
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x01, LCD_CMD); //
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xB9, LCD_CMD); //
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xBC, LCD_CMD); // set pre_charge
                                                                        // voltage/VCOMH
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x08, LCD_CMD); // (0x08);
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xBE, LCD_CMD); // set VCOMH
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0X07, LCD_CMD); // (0x07);
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xB6, LCD_CMD); // Set second pre-charge
                                                                        // period
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x01, LCD_CMD); //
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control,
                                     0xD5,
                                     LCD_CMD); // enable second precharge and enternal vsl
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0X62, LCD_CMD); // (0x62);
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xA4, LCD_CMD); // Set Normal Display Mode
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x2E, LCD_CMD); // Deactivate Scroll
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xAF, LCD_CMD); // Switch on display
    usleep(INIT_SLEEP);

    // Row Address
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x75, LCD_CMD); // Set Row Address
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x00, LCD_CMD); // Start 0
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x5f, LCD_CMD); // End 95
    usleep(INIT_SLEEP);

    // Column Address
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x15, LCD_CMD); // Set Column Address
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x08, LCD_CMD); // Start from 8th Column of
                                                                        // driver IC. This is 0th
                                                                        // Column for OLED
    usleep(INIT_SLEEP);
    error = mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x37, LCD_CMD); // End at  (8 + 47)th
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

mraa_result_t
SSD1327::draw(uint8_t* data, int bytes)
{
    mraa_result_t error = MRAA_SUCCESS;

    setHorizontalMode();
    for (int row = 0; row < bytes; row++) {
        for (uint8_t col = 0; col < 8; col += 2) {
            uint8_t value = 0x0;

            uint8_t bitOne = (data[row] << col) & 0x80;
            uint8_t bitTwo = (data[row] << (col + 1)) & 0x80;

            value |= (bitOne) ? grayHigh : 0x00;
            value |= (bitTwo) ? grayLow : 0x00;

            mraa_i2c_write_byte_data(m_i2c_lcd_control, value, LCD_DATA);
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
mraa_result_t
SSD1327::write(std::string msg)
{
    mraa_result_t error = MRAA_SUCCESS;

    setVerticalMode();
    for (std::string::size_type i = 0; i < msg.size(); ++i) {
        writeChar(m_i2c_lcd_control, msg[i]);
    }

    return error;
}

mraa_result_t
SSD1327::setCursor(int row, int column)
{
    mraa_result_t error = MRAA_SUCCESS;

    // Column Address
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x15, LCD_CMD); /* Set Column Address */
    usleep(CMD_SLEEP);
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x08 + (column * 4), LCD_CMD); /* Start Column:
                                                                                  Start from 8 */
    usleep(CMD_SLEEP);
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x37, LCD_CMD); /* End Column */
    usleep(CMD_SLEEP);
    // Row Address
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x75, LCD_CMD); /* Set Row Address */
    usleep(CMD_SLEEP);
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x00 + (row * 8), LCD_CMD); /* Start Row*/
    usleep(CMD_SLEEP);
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x07 + (row * 8), LCD_CMD); /* End Row*/
    usleep(CMD_SLEEP);

    return error;
}

mraa_result_t
SSD1327::clear()
{
    mraa_result_t error = MRAA_SUCCESS;
    uint8_t columnIdx, rowIdx;

    for (rowIdx = 0; rowIdx < 12; rowIdx++) {
        // clear all columns
        for (columnIdx = 0; columnIdx < 12; columnIdx++) {
            writeChar(m_i2c_lcd_control, ' ');
        }
    }

    return MRAA_SUCCESS;
}

mraa_result_t
SSD1327::home()
{
    return setCursor(0, 0);
}

mraa_result_t
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
mraa_result_t
SSD1327::writeChar(mraa_i2c_context ctx, uint8_t value)
{
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

            mraa_i2c_write_byte_data(m_i2c_lcd_control, data, LCD_DATA);
            usleep(CMD_SLEEP - 2000);
        }
    }
}

mraa_result_t
SSD1327::setNormalDisplay()
{
    return mraa_i2c_write_byte_data(m_i2c_lcd_control,
                                    DISPLAY_CMD_SET_NORMAL,
                                    LCD_CMD); // set to normal display '1' is ON
}

mraa_result_t
SSD1327::setHorizontalMode()
{
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xA0, LCD_CMD); // remap to
    usleep(CMD_SLEEP);
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x42, LCD_CMD); // horizontal mode
    usleep(CMD_SLEEP);

    // Row Address
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x75, LCD_CMD); // Set Row Address
    usleep(CMD_SLEEP);
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x00, LCD_CMD); // Start 0
    usleep(CMD_SLEEP);
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x5f, LCD_CMD); // End 95
    usleep(CMD_SLEEP);

    // Column Address
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x15, LCD_CMD); // Set Column Address
    usleep(CMD_SLEEP);
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x08, LCD_CMD); // Start from 8th Column of driver
                                                                // IC. This is 0th Column for OLED
    usleep(CMD_SLEEP);
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x37, LCD_CMD); // End at  (8 + 47)th column. Each
                                                                // Column has 2 pixels(or segments)
    usleep(CMD_SLEEP);
}

mraa_result_t
SSD1327::setVerticalMode()
{
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0xA0, LCD_CMD); // remap to
    usleep(CMD_SLEEP);
    mraa_i2c_write_byte_data(m_i2c_lcd_control, 0x46, LCD_CMD); // Vertical mode
    usleep(CMD_SLEEP);
}
