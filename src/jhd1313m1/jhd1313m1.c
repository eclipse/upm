/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Based on UPM C++ drivers originally developed by:
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

#include <unistd.h>
#include <assert.h>
#include <string.h>

#include <upm_utilities.h>

#include "jhd1313m1.h"
#include "hd44780_bits.h"

jhd1313m1_context jhd1313m1_init(int bus, int lcd_addr, int rgb_addr)
{
    jhd1313m1_context dev =
        (jhd1313m1_context)malloc(sizeof(struct _jhd1313m1_context));

    if (!dev)
        return NULL;

    memset((void *)dev, 0, sizeof(struct _jhd1313m1_context));

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        jhd1313m1_close(dev);
        return NULL;
    }

    // initialize the MRAA contexts

    if (!(dev->i2cLCD = mraa_i2c_init(bus)))
    {
        printf("%s: mraa_i2c_init(LCD) failed.\n", __FUNCTION__);
        jhd1313m1_close(dev);

        return NULL;
    }

    // now check the address...
    if (mraa_i2c_address(dev->i2cLCD, lcd_addr) != MRAA_SUCCESS)
    {
        printf("%s: mraa_i2c_address(LCD) failed.\n", __FUNCTION__);

        jhd1313m1_close(dev);

        return NULL;
    }

    if (!(dev->i2cRGB = mraa_i2c_init(bus)))
    {
        printf("%s: mraa_i2c_init(RGB) failed.\n", __FUNCTION__);
        jhd1313m1_close(dev);

        return NULL;
    }

    // now check the address...
    if (mraa_i2c_address(dev->i2cRGB, rgb_addr) != MRAA_SUCCESS)
    {
        printf("%s: mraa_i2c_address(RGB) failed.\n", __FUNCTION__);

        jhd1313m1_close(dev);

        return NULL;
    }

    /* HD44780 requires writing three times to initialize or reset
       according to the hardware errata on page 45 figure 23 of
       the Hitachi HD44780 datasheet */

    /* First try */
    upm_delay_us(50000);
    jhd1313m1_command(dev, HD44780_FUNCTIONSET | HD44780_8BITMODE);

    /* Second try */
    upm_delay_us(4500);
    jhd1313m1_command(dev, HD44780_FUNCTIONSET | HD44780_8BITMODE);

    /* Third try */
    upm_delay_us(150);
    jhd1313m1_command(dev, HD44780_FUNCTIONSET | HD44780_8BITMODE);

    /* Set 2 row mode and font size */
    jhd1313m1_command(dev, HD44780_FUNCTIONSET | HD44780_8BITMODE
                      | HD44780_2LINE | HD44780_5x10DOTS);
    upm_delay_us(100);

    jhd1313m1_display_on(dev, true);
    upm_delay_us(100);

    jhd1313m1_clear(dev);
    upm_delay_us(2000);

    jhd1313m1_command(dev, HD44780_ENTRYMODESET | HD44780_ENTRYLEFT
                      | HD44780_ENTRYSHIFTDECREMENT);

    jhd1313m1_backlight_on(dev, true);
    // full white
    jhd1313m1_set_color(dev, 0xff, 0xff, 0xff);

    return dev;
}

void jhd1313m1_close(jhd1313m1_context dev)
{
    assert(dev != NULL);

    if (dev->i2cLCD)
        mraa_i2c_stop(dev->i2cLCD);
    if (dev->i2cRGB)
        mraa_i2c_stop(dev->i2cRGB);

    free(dev);
}

upm_result_t jhd1313m1_set_color(jhd1313m1_context dev, uint8_t r, uint8_t g,
                                 uint8_t b)
{
    assert(dev != NULL);

    mraa_result_t rv = MRAA_SUCCESS;
    rv = mraa_i2c_write_byte_data(dev->i2cRGB, 0, 0);
    rv = mraa_i2c_write_byte_data(dev->i2cRGB, 0, 1);

    rv = mraa_i2c_write_byte_data(dev->i2cRGB, r, 0x04);
    rv = mraa_i2c_write_byte_data(dev->i2cRGB, g, 0x03);
    rv = mraa_i2c_write_byte_data(dev->i2cRGB, b, 0x02);

    if (rv)
    {
        printf("%s: mraa_i2c_write_byte_data() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t jhd1313m1_write(const jhd1313m1_context dev, char *buffer,
                             int len)
{
    assert(dev != NULL);

    upm_result_t error = UPM_SUCCESS;

    int i;
    for (i=0; i<len; ++i)
        error = jhd1313m1_data(dev, buffer[i]);

    return error;
}

upm_result_t jhd1313m1_set_cursor(const jhd1313m1_context dev, unsigned int row,
                                  unsigned int column)
{
    assert(dev != NULL);

    column = column % 16;
    uint8_t offset = column;

    // this should work for any display with two rows
    // DDRAM mapping:
    // 00 .. 27
    // 40 .. 67
    offset += row * 0x40;

    return jhd1313m1_command(dev, HD44780_CMD | offset);
}

upm_result_t jhd1313m1_clear(const jhd1313m1_context dev)
{
    assert(dev != NULL);

    upm_result_t ret;
    ret = jhd1313m1_command(dev, HD44780_CLEARDISPLAY);
    upm_delay_us(2000); // this command takes awhile
    return ret;
}

upm_result_t jhd1313m1_home(const jhd1313m1_context dev)
{
    assert(dev != NULL);

    upm_result_t ret;
    ret = jhd1313m1_command(dev, HD44780_RETURNHOME);
    upm_delay_us(2000); // this command takes awhile
    return ret;
}

upm_result_t jhd1313m1_create_char(const jhd1313m1_context dev,
                                 unsigned int slot,
                                 jhd1313m1_custom_char_t data)
{
    assert(dev != NULL);

    upm_result_t error = UPM_SUCCESS;

    slot &= 0x07; // only have 8 positions we can set

    error = jhd1313m1_command(dev, HD44780_SETCGRAMADDR | (slot << 3));

    if (error == UPM_SUCCESS)
    {
        int i;
        for (i = 0; i < 8; i++) {
            error = jhd1313m1_data(dev, data[i]);
        }
    }

    return error;
}

upm_result_t jhd1313m1_display_on(const jhd1313m1_context dev, bool on)
{
    assert(dev != NULL);

    if (on)
        dev->displayControl |= HD44780_DISPLAYON;
    else
        dev->displayControl &= ~HD44780_DISPLAYON;

    return jhd1313m1_command(dev, HD44780_DISPLAYCONTROL | dev->displayControl);
}

upm_result_t jhd1313m1_cursor_on(const jhd1313m1_context dev, bool on)
{
    assert(dev != NULL);

    if (on)
        dev->displayControl |= HD44780_CURSORON;
    else
        dev->displayControl &= ~HD44780_CURSORON;

    return jhd1313m1_command(dev, HD44780_DISPLAYCONTROL | dev->displayControl);
}

upm_result_t jhd1313m1_cursor_blink_on(const jhd1313m1_context dev, bool on)
{
    assert(dev != NULL);

    if (on)
        dev->displayControl |= HD44780_BLINKON;
    else
        dev->displayControl &= ~HD44780_BLINKON;

    return jhd1313m1_command(dev, HD44780_DISPLAYCONTROL | dev->displayControl);
}

upm_result_t jhd1313m1_backlight_on(const jhd1313m1_context dev, bool on)
{
    assert(dev != NULL);

    mraa_result_t rv = MRAA_SUCCESS;
    if (on)
        rv = mraa_i2c_write_byte_data(dev->i2cRGB, 0xaa, 0x08);
    else
        rv = mraa_i2c_write_byte_data(dev->i2cRGB, 0x00, 0x08);

    if (rv)
    {
        printf("%s: mraa_i2c_write_byte_data() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t jhd1313m1_scroll(const jhd1313m1_context dev, bool direction)
{
    if (direction)
        return jhd1313m1_scroll_display_left(dev);
    else
        return jhd1313m1_scroll_display_right(dev);
}

upm_result_t jhd1313m1_scroll_display_left(const jhd1313m1_context dev)
{
    assert(dev != NULL);

    return jhd1313m1_command(dev, HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE
                             | HD44780_MOVELEFT);
}

upm_result_t jhd1313m1_scroll_display_right(const jhd1313m1_context dev)
{
    assert(dev != NULL);

    return jhd1313m1_command(dev, HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE
                             | HD44780_MOVERIGHT);
}

upm_result_t jhd1313m1_entry_left_to_right(const jhd1313m1_context dev, bool on)
{
    assert(dev != NULL);

    if (on)
        dev->entryDisplayMode |= HD44780_ENTRYLEFT;
    else
        dev->entryDisplayMode &= ~HD44780_ENTRYLEFT;

    return jhd1313m1_command(dev, HD44780_ENTRYMODESET | dev->entryDisplayMode);
}

upm_result_t jhd1313m1_autoscroll_on(const jhd1313m1_context dev, bool on)
{
    assert(dev != NULL);

    if (on)
        dev->entryDisplayMode |= HD44780_ENTRYSHIFTINCREMENT;
    else
        dev->entryDisplayMode &= ~HD44780_ENTRYSHIFTINCREMENT;

    return jhd1313m1_command(dev, HD44780_ENTRYMODESET | dev->entryDisplayMode);
}

upm_result_t jhd1313m1_command(const jhd1313m1_context dev, uint8_t cmd)
{
    assert(dev != NULL);

    if (mraa_i2c_write_byte_data(dev->i2cLCD, cmd, HD44780_CMD))
    {
        printf("%s: mraa_i2c_write_byte_data() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t jhd1313m1_data(const jhd1313m1_context dev, uint8_t cmd)
{
    assert(dev != NULL);

    if (mraa_i2c_write_byte_data(dev->i2cLCD, cmd, HD44780_DATA))
    {
        printf("%s: mraa_i2c_write_byte_data() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}
