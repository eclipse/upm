/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Based on UPM C++ drivers originally developed by:
 * Author: Daniel Mosquera
 * Copyright (c) 2013 Daniel Mosquera
 *
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Contributions: Sergey Kiselev <sergey.kiselev@intel.com>
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

#include "lcm1602.h"
#include "hd44780_bits.h"

// forward declarations
static upm_result_t send(const lcm1602_context dev, uint8_t value, int mode);
static upm_result_t write4bits(const lcm1602_context dev, uint8_t value);
static upm_result_t expandWrite(const lcm1602_context dev, uint8_t value);
static upm_result_t pulseEnable(const lcm1602_context dev, uint8_t value);

lcm1602_context lcm1602_i2c_init(int bus, int address, bool is_expander,
                                 uint8_t num_columns, uint8_t num_rows)
{
    lcm1602_context dev =
        (lcm1602_context)malloc(sizeof(struct _lcm1602_context));

    if (!dev)
        return NULL;

    memset((void *)dev, 0, sizeof(struct _lcm1602_context));

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        lcm1602_close(dev);
        return NULL;
    }

    // initialize the MRAA context

    if (!(dev->i2c = mraa_i2c_init(bus)))
    {
        printf("%s: mraa_i2c_init failed.\n", __FUNCTION__);
        lcm1602_close(dev);

        return NULL;
    }

    // now check the address...
    if (mraa_i2c_address(dev->i2c, address) != MRAA_SUCCESS)
    {
        printf("%s: mraa_i2c_address failed.\n", __FUNCTION__);

        lcm1602_close(dev);

        return NULL;
    }

    dev->isI2C = true;
    dev->backlight = HD44780_BACKLIGHT;
    dev->columns = num_columns;
    dev->rows = num_rows;

    // if we are not dealing with an expander we will only initialize
    // the I2C context and bail, leaving it up to the caller to handle
    // further communications (like JHD1313M1)

    if (!is_expander)
        return dev;

    upm_delay_us(50000);
    lcm1602_backlight_on(dev, true);
    upm_delay_us(100000);

    // try to put us into 4 bit mode
    write4bits(dev, 0x03 << 4);
    upm_delay_us(4500);

    write4bits(dev, 0x30);
    upm_delay_us(4500);

    write4bits(dev,0x30);
    upm_delay_us(150);

    // Put us into 4 bit mode, for realz yo.
    write4bits(dev, 0x20);

    // Set number of lines
    lcm1602_command(dev, HD44780_FUNCTIONSET | 0x0f);

    // default display control
    dev->displayControl = HD44780_DISPLAYON | HD44780_CURSOROFF
        | HD44780_BLINKOFF;

    lcm1602_command(dev, HD44780_DISPLAYCONTROL | dev->displayControl);
    upm_delay_us(2000);
    lcm1602_clear(dev);

    // Set entry mode.
    dev->entryDisplayMode = HD44780_ENTRYLEFT | HD44780_ENTRYSHIFTDECREMENT;
    lcm1602_command(dev, HD44780_ENTRYMODESET | dev->entryDisplayMode);

    lcm1602_home(dev);

    return dev;
}

lcm1602_context lcm1602_gpio_init(uint8_t rs, uint8_t enable,
                                  uint8_t d0, uint8_t d1, uint8_t d2,
                                  uint8_t d3, uint8_t num_columns,
                                  uint8_t num_rows)
{
    lcm1602_context dev =
        (lcm1602_context)malloc(sizeof(struct _lcm1602_context));

    if (!dev)
        return NULL;

    memset((void *)dev, 0, sizeof(struct _lcm1602_context));

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        lcm1602_close(dev);
        return NULL;
    }

    // initialize the MRAA contexts

    if (!(dev->gpioRS = mraa_gpio_init(rs)))
    {
        printf("%s: mraa_gpio_init(rs) failed.\n", __FUNCTION__);
        lcm1602_close(dev);
        return NULL;
    }
    mraa_gpio_dir(dev->gpioRS, MRAA_GPIO_OUT);

    if (!(dev->gpioEN = mraa_gpio_init(enable)))
    {
        printf("%s: mraa_gpio_init(enable) failed.\n", __FUNCTION__);
        lcm1602_close(dev);
        return NULL;
    }
    mraa_gpio_dir(dev->gpioEN, MRAA_GPIO_OUT);

    if (!(dev->gpioD0 = mraa_gpio_init(d0)))
    {
        printf("%s: mraa_gpio_init(d0) failed.\n", __FUNCTION__);
        lcm1602_close(dev);
        return NULL;
    }
    mraa_gpio_dir(dev->gpioD0, MRAA_GPIO_OUT);

    if (!(dev->gpioD1 = mraa_gpio_init(d1)))
    {
        printf("%s: mraa_gpio_init(d1) failed.\n", __FUNCTION__);
        lcm1602_close(dev);
        return NULL;
    }
    mraa_gpio_dir(dev->gpioD1, MRAA_GPIO_OUT);

    if (!(dev->gpioD2 = mraa_gpio_init(d2)))
    {
        printf("%s: mraa_gpio_init(d2) failed.\n", __FUNCTION__);
        lcm1602_close(dev);
        return NULL;
    }
    mraa_gpio_dir(dev->gpioD2, MRAA_GPIO_OUT);

    if (!(dev->gpioD3 = mraa_gpio_init(d3)))
    {
        printf("%s: mraa_gpio_init(d3) failed.\n", __FUNCTION__);
        lcm1602_close(dev);
        return NULL;
    }
    mraa_gpio_dir(dev->gpioD3, MRAA_GPIO_OUT);

    dev->isI2C = false;
    // no backlight for GPIO
    dev->backlight = 0;
    dev->columns = num_columns;
    dev->rows = num_rows;

    // set RS and Enable low to begin issuing commands
    mraa_gpio_write(dev->gpioRS, 0);
    mraa_gpio_write(dev->gpioEN, 0);

    // wait to stabilize
    upm_delay_us(100000);

    // set 4bit mode

    // These steps are adapted from the HD44780 datasheet, figure 24

    // try 1
    write4bits(dev, 0x03);
    upm_delay_us(4500);

    // try 2
    write4bits(dev, 0x03);
    upm_delay_us(4500);

    // try 3
    write4bits(dev, 0x03);
    upm_delay_us(150);

    // Finally, put into 4 bit mode
    write4bits(dev, 0x02);

    // Set number of lines
    lcm1602_command(dev, HD44780_FUNCTIONSET | HD44780_2LINE | HD44780_4BITMODE
                    | HD44780_5x8DOTS);


    dev->displayControl = HD44780_DISPLAYON | HD44780_CURSOROFF
        | HD44780_BLINKOFF;
    lcm1602_command(dev, HD44780_DISPLAYCONTROL | dev->displayControl);
    upm_delay_us(2000);
    lcm1602_clear(dev);

    // Set entry mode.
    dev->entryDisplayMode = HD44780_ENTRYLEFT | HD44780_ENTRYSHIFTDECREMENT;
    lcm1602_command(dev, HD44780_ENTRYMODESET | dev->entryDisplayMode);

    lcm1602_home(dev);

    return dev;
}

void lcm1602_close(lcm1602_context dev)
{
    assert(dev != NULL);

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);

    if (dev->gpioRS)
        mraa_gpio_close(dev->gpioRS);
    if (dev->gpioEN)
        mraa_gpio_close(dev->gpioEN);
    if (dev->gpioD0)
        mraa_gpio_close(dev->gpioD0);
    if (dev->gpioD1)
        mraa_gpio_close(dev->gpioD1);
    if (dev->gpioD2)
        mraa_gpio_close(dev->gpioD2);
    if (dev->gpioD3)
        mraa_gpio_close(dev->gpioD3);

    free(dev);
}

upm_result_t lcm1602_write(const lcm1602_context dev, char *buffer,
                           int len)
{
    assert(dev != NULL);

    upm_result_t error = UPM_SUCCESS;

    int i;
    for (i=0; i<len; ++i)
        error = lcm1602_data(dev, buffer[i]);

    return error;
}

upm_result_t lcm1602_set_cursor(const lcm1602_context dev, unsigned int row,
                                unsigned int column)
{
    assert(dev != NULL);

    column = column % dev->columns;
    uint8_t offset = column;

    switch (dev->rows)
    {
    case 1:
        // Single row displays with more than 8 columns usually have their
        // DDRAM split in two halves. The first half starts at address 00.
        // The second half starts at address 40. E.g. 16x2 DDRAM mapping:
        // 00 01 02 03 04 05 06 07 40 41 42 43 44 45 46 47
        if (dev->columns > 8)
        {
            offset = (column % (dev->columns / 2)) +
                (column / (dev->columns / 2)) * 0x40;
        }
        break;

    case 2:
        // this should work for any display with two rows
        // DDRAM mapping:
        // 00 .. 27
        // 40 .. 67
        offset += row * 0x40;
        break;

    case 4:
        if (dev->columns == 16)
        {
            // 16x4 display
            // DDRAM mapping:
            // 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
            // 40 41 42 43 43 45 46 47 48 49 4A 4B 4C 4D 4E 4F
            // 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
            // 50 51 52 53 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F
            int row_addr[] = { 0x00, 0x40, 0x10, 0x50 };
            offset += row_addr[row];
        }
        else
        {
            // 20x4 display
            // DDRAM mapping:
            // 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13
            // 40 41 42 43 43 45 46 47 48 49 4A 4B 4C 4D 4E 4F 50 51 52 53
            // 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23 24 25 26 27
            // 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F 60 61 62 63 64 65 66 67
            int row_addr[] = { 0x00, 0x40, 0x14, 0x54 };
            offset += row_addr[row];
        }
        break;
    }

    return lcm1602_command(dev, HD44780_CMD | offset);
}

upm_result_t lcm1602_clear(const lcm1602_context dev)
{
    assert(dev != NULL);

    upm_result_t ret;
    ret = lcm1602_command(dev, HD44780_CLEARDISPLAY);
    upm_delay_us(2000); // this command takes awhile
    return ret;
}

upm_result_t lcm1602_home(const lcm1602_context dev)
{
    assert(dev != NULL);

    upm_result_t ret;
    ret = lcm1602_command(dev, HD44780_RETURNHOME);
    upm_delay_us(2000); // this command takes awhile
    return ret;
}

upm_result_t lcm1602_create_char(const lcm1602_context dev,
                                 unsigned int slot,
                                 lcm1602_custom_char_t data)
{
    assert(dev != NULL);

    upm_result_t error = UPM_SUCCESS;

    slot &= 0x07; // only have 8 positions we can set

    error = lcm1602_command(dev, HD44780_SETCGRAMADDR | (slot << 3));

    if (error == UPM_SUCCESS)
    {
        int i;
        for (i = 0; i < 8; i++) {
            error = lcm1602_data(dev, data[i]);
        }
    }

    return error;
}

upm_result_t lcm1602_display_on(const lcm1602_context dev, bool on)
{
    assert(dev != NULL);

    if (on)
        dev->displayControl |= HD44780_DISPLAYON;
    else
        dev->displayControl &= ~HD44780_DISPLAYON;

    return lcm1602_command(dev, HD44780_DISPLAYCONTROL | dev->displayControl);
}

upm_result_t lcm1602_cursor_on(const lcm1602_context dev, bool on)
{
    assert(dev != NULL);

    if (on)
        dev->displayControl |= HD44780_CURSORON;
    else
        dev->displayControl &= ~HD44780_CURSORON;

    return lcm1602_command(dev, HD44780_DISPLAYCONTROL | dev->displayControl);
}

upm_result_t lcm1602_cursor_blink_on(const lcm1602_context dev, bool on)
{
    assert(dev != NULL);

    if (on)
        dev->displayControl |= HD44780_BLINKON;
    else
        dev->displayControl &= ~HD44780_BLINKON;

    return lcm1602_command(dev, HD44780_DISPLAYCONTROL | dev->displayControl);
}

upm_result_t lcm1602_backlight_on(const lcm1602_context dev, bool on)
{
    assert(dev != NULL);

    if (on)
        dev->backlight = HD44780_BACKLIGHT;
    else
        dev->backlight = HD44780_NOBACKLIGHT;

    return expandWrite(dev, dev->backlight);
}

upm_result_t lcm1602_scroll_display_left(const lcm1602_context dev)
{
    assert(dev != NULL);

    return lcm1602_command(dev, HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE
                           | HD44780_MOVELEFT);
}

upm_result_t lcm1602_scroll_display_right(const lcm1602_context dev)
{
    assert(dev != NULL);

    return lcm1602_command(dev, HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE
                           | HD44780_MOVERIGHT);
}

upm_result_t lcm1602_entry_left_to_right(const lcm1602_context dev, bool on)
{
    assert(dev != NULL);

    if (on)
        dev->entryDisplayMode |= HD44780_ENTRYLEFT;
    else
        dev->entryDisplayMode &= ~HD44780_ENTRYLEFT;

    return lcm1602_command(dev, HD44780_ENTRYMODESET | dev->entryDisplayMode);
}

upm_result_t lcm1602_autoscroll_on(const lcm1602_context dev, bool on)
{
    assert(dev != NULL);

    if (on)
        dev->entryDisplayMode |= HD44780_ENTRYSHIFTINCREMENT;
    else
        dev->entryDisplayMode &= ~HD44780_ENTRYSHIFTINCREMENT;

    return lcm1602_command(dev, HD44780_ENTRYMODESET | dev->entryDisplayMode);
}

upm_result_t lcm1602_command(const lcm1602_context dev, uint8_t cmd)
{
    assert(dev != NULL);

    return send(dev, cmd, 0);
}

upm_result_t lcm1602_data(const lcm1602_context dev, uint8_t cmd)
{
    assert(dev != NULL);
    return send(dev, cmd, HD44780_RS); // 1
}


// static declarations
static upm_result_t send(const lcm1602_context dev, uint8_t value,
                         int mode)
{
    assert(dev != NULL);

    uint8_t h;
    uint8_t l;

    upm_result_t rv = UPM_SUCCESS;

    if (dev->isI2C)
    {
        h = value & 0xf0;
        l = (value << 4) & 0xf0;
        if (write4bits(dev, h | mode))
            rv = UPM_ERROR_OPERATION_FAILED;
        if (write4bits(dev, l | mode))
            rv = UPM_ERROR_OPERATION_FAILED;

        return rv;
    }

    // else, gpio (4 bit)

    // register select
    if (mraa_gpio_write(dev->gpioRS, mode))
    {
        printf("%s: mraa_gpio_write() failed\n", __FUNCTION__);
        rv = UPM_ERROR_OPERATION_FAILED;
    }

    h = value >> 4;
    l = value & 0x0f;

    if (write4bits(dev, h))
        rv = UPM_ERROR_OPERATION_FAILED;
    if (write4bits(dev, l))
        rv = UPM_ERROR_OPERATION_FAILED;

    return rv;
}

static upm_result_t write4bits(const lcm1602_context dev,
                               uint8_t value)
{
    assert(dev != NULL);

    upm_result_t rv = UPM_SUCCESS;

    if (dev->isI2C)
    {
        if (expandWrite(dev, value))
            rv = UPM_ERROR_OPERATION_FAILED;
        if (pulseEnable(dev, value))
            rv = UPM_ERROR_OPERATION_FAILED;

        return rv;
    }

    // else gpio
    mraa_result_t mrv = MRAA_SUCCESS;
    mrv = mraa_gpio_write(dev->gpioD0, ((value >> 0) & 0x01) );
    mrv = mraa_gpio_write(dev->gpioD1, ((value >> 1) & 0x01) );
    mrv = mraa_gpio_write(dev->gpioD2, ((value >> 2) & 0x01) );
    mrv = mraa_gpio_write(dev->gpioD3, ((value >> 3) & 0x01) );

    if (mrv)
    {
        printf("%s: mraa_gpio_write() failed\n", __FUNCTION__);
        rv = UPM_ERROR_OPERATION_FAILED;
    }

    if (pulseEnable(dev, value)) // value is ignored here for gpio
    {
        printf("%s: pulseEnable() failed\n", __FUNCTION__);
        rv = UPM_ERROR_OPERATION_FAILED;
    }

    return rv;
}

static upm_result_t expandWrite(const lcm1602_context dev,
                               uint8_t value)
{
    assert(dev != NULL);

    // invalid for gpio
    if (!dev->isI2C)
        return UPM_ERROR_NO_RESOURCES;

    uint8_t buffer = value | dev->backlight;

    if (mraa_i2c_write_byte(dev->i2c, buffer))
    {
        printf("%s: mraa_i2c_write_byte() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

static upm_result_t pulseEnable(const lcm1602_context dev,
                                uint8_t value)
{
    assert(dev != NULL);

    upm_result_t rv = UPM_SUCCESS;

    if (dev->isI2C)
    {
        if (expandWrite(dev, value | HD44780_EN))
            rv = UPM_ERROR_OPERATION_FAILED;
        upm_delay_us(1);

        if (expandWrite(dev, value & ~HD44780_EN))
            rv = UPM_ERROR_OPERATION_FAILED;

        upm_delay_us(50);
        return rv;
    }

    // else gpio

    mraa_result_t mrv = MRAA_SUCCESS;
    mrv = mraa_gpio_write(dev->gpioEN, 1);
    upm_delay_us(1); // must be > 450ns
    mrv = mraa_gpio_write(dev->gpioEN, 0);
    upm_delay_us(100); // must be >37us

    if (mrv)
    {
        printf("%s: mraa_gpio_write() failed\n", __FUNCTION__);
        rv = UPM_ERROR_OPERATION_FAILED;
    }

    return rv;
}
