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

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <upm.h>

#include <mraa/i2c.h>
#include <mraa/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif


    /**
     * @file lcm1602.h
     * @library lcm1602
     * @brief C API for the LCM1602 family of LCD displays
     *
     * @include lcm1602.c
     */

    /**
     * Device context
     */
    typedef struct _lcm1602_context {
        // A large number of GPIOs
        mraa_gpio_context        gpioRS;
        mraa_gpio_context        gpioEN;
        mraa_gpio_context        gpioD0;
        mraa_gpio_context        gpioD1;
        mraa_gpio_context        gpioD2;
        mraa_gpio_context        gpioD3;

        // I2C command control
        mraa_i2c_context         i2c;

        // what interface are we using?
        bool                     isI2C;

        // configuration
        unsigned int             columns;
        unsigned int             rows;

        // display command
        uint8_t                  displayControl;
        uint8_t                  entryDisplayMode;
        uint8_t                  backlight;
    } *lcm1602_context;

    /**
     * Custom character.
     */
    typedef char lcm1602_custom_char_t[8];

    /**
     * LCM1602 I2C initialization.  This is used for those devices
     * using an I2C expander, or other I2C interface.  If you are
     * using an expander, you must specify true for the is_expander
     * argument.  If not, then the caller of this function is
     * responsible for any transfers to the I2C device.  Internally,
     * only direct access to an I2C expander is supported.
     *
     * @param bus I2C bus to use.
     * @param address I2C address the LCD is configured for.
     * @param is_expander True if we are dealing with an I2C expander,
     * false otherwise.
     * @param num_columns Number of columns the display has.
     * @param num_rows Number of rows the display has.
     * @return Device Ccontext, or NULL on error.
     */
    lcm1602_context lcm1602_i2c_init(int bus, int address, bool is_expander,
                                     uint8_t num_columns, uint8_t num_rows);

    /**
     * LCM1602 GPIO (parallel) initialization, used for GPIO based
     * HD44780 controllers supporting RS, Enable, and 4 data pins in
     * 4-bit mode.
     *
     * @param rs Register select pin
     * @param enable Enable pin
     * @param d0 Data 0 pin
     * @param d1 Data 1 pin
     * @param d2 Data 2 pin
     * @param d3 Data 3 pin
     * @param num_columns Number of columns the display has. Default 16.
     * @param num_rows Number of rows the display has. Default 2.
     * @return Device Ccontext, or NULL on error.
     */
    lcm1602_context lcm1602_gpio_init(uint8_t rs, uint8_t enable,
                                      uint8_t d0, uint8_t d1, uint8_t d2,
                                      uint8_t d3, uint8_t num_columns,
                                      uint8_t num_rows);

    /**
     * LCM1602 close.
     *
     * @param dev The device context.
     */
    void lcm1602_close(lcm1602_context dev);

    /**
     * Writes a string to the LCD.
     *
     * @param dev The device context.
     * @param buffer Character buffer containing characters to write to
     * the display; note: only ASCII characters are supported
     * @param len The number of characters to write.
     * @return UPM result.
     */
    upm_result_t lcm1602_write(const lcm1602_context dev, char *buffer,
                               int len);

    /**
     * Sets the cursor to specified coordinates
     *
     * @param dev The device context.
     * @param row Row to set the cursor to.
     * @param column Column to set the cursor to.
     * @return UPM result.
     */
    upm_result_t lcm1602_set_cursor(const lcm1602_context dev,
                                    unsigned int row, unsigned int column);

    /**
     * Clears the display of all characters.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t lcm1602_clear(const lcm1602_context dev);

    /**
     * Returns to the home coordinates (0,0).
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t lcm1602_home(const lcm1602_context dev);

    /**
     * Create a custom character.
     *
     * @param dev The device context.
     * @param slot The character slot to write, only 8 are available.
     * @param data The character data (8 bytes) making up the character.
     * @return UPM result.
     */
    upm_result_t lcm1602_create_char(const lcm1602_context dev,
                                     unsigned int slot,
                                     lcm1602_custom_char_t data);

    /**
     * Turn the display on.
     *
     * @param dev The device context.
     * @param on true to turn display on, false otherwise.
     * @return UPM result.
     */
    upm_result_t lcm1602_display_on(const lcm1602_context dev, bool on);

    /**
     * Turn the cursor on.
     *
     * @param dev The device context.
     * @param on true to turn cursor on, false otherwise.
     * @return UPM result.
     */
    upm_result_t lcm1602_cursor_on(const lcm1602_context dev, bool on);

    /**
     * Turn cursor blink on.
     *
     * @param dev The device context.
     * @param on true to turn cursor blink on, false otherwise.
     * @return UPM result.
     */
    upm_result_t lcm1602_cursor_blink_on(const lcm1602_context dev, bool on);

    /**
     * Turn backlight on.
     *
     * @param dev The device context.
     * @param on true to turn backlight on, false otherwise.
     * @return UPM result.
     */
    upm_result_t lcm1602_backlight_on(const lcm1602_context dev, bool on);

    /**
     * Scroll the display left, without changing the character RAM.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t lcm1602_scroll_display_left(const lcm1602_context dev);

    /**
     * Scroll the display right, without changing the character RAM.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t lcm1602_scroll_display_right(const lcm1602_context dev);

    /**
     * Set the entry mode so that characters are added left to right.
     *
     * @param dev The device context.
     * @param on true to add characters left to right, false for right
     * to left.
     * @return UPM result.
     */
    upm_result_t lcm1602_entry_left_to_right(const lcm1602_context dev,
                                             bool on);

    /**
     * Right justify text entered from the cursor.
     *
     * @param dev The device context.
     * @param on true right justify text, false to left justify text.
     * @return UPM result.
     */
    upm_result_t lcm1602_autoscroll_on(const lcm1602_context dev, bool on);

    /**
     * Send a command byte to the controller.  This is a low level
     * command and should not be used unless you know what you are
     * doing.
     *
     * @param dev The device context.
     * @param cmd The command byte to send.
     * @return UPM result.
     */
    upm_result_t lcm1602_command(const lcm1602_context dev, uint8_t cmd);

    /**
     * Send a data byte to the controller.  This is a low level
     * command and should not be used unless you know what you are
     * doing.
     *
     * @param dev The device context.
     * @param data The data byte to send.
     * @return UPM result.
     */
    upm_result_t lcm1602_data(const lcm1602_context dev, uint8_t data);


#ifdef __cplusplus
}
#endif
