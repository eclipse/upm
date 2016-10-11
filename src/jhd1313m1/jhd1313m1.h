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
     * @file jhd1313m1.h
     * @library jhd1313m1
     * @brief C API for the JHD1313M1 family of LCD displays, like the
     * Grove RGB LCD
     *
     * @include jhd1313m1.c
     */

    /**
     * Device context
     */
    typedef struct _jhd1313m1_context {
        // I2C LCD command (lcm1602-like)
        mraa_i2c_context         i2cLCD;
        // I2C RGB backlight control
        mraa_i2c_context         i2cRGB;

        // display command
        uint8_t                  displayControl;
        uint8_t                  entryDisplayMode;
    } *jhd1313m1_context;

    /**
     * Custom character.
     */
    typedef char jhd1313m1_custom_char_t[8];

    /**
     * JHD1313M1 I2C initialization.
     *
     * @param bus I2C bus to use.
     * @param lcd_addr I2C address the LCD controller is on.
     * @param rgb_addr I2C address the RGB backlight controller is on.
     * @return Device Ccontext, or NULL on error.
     */
    jhd1313m1_context jhd1313m1_init(int bus, int lcd_addr, int rgb_addr);

    /**
     * JHD1313M1 close.
     *
     * @param dev The device context.
     */
    void jhd1313m1_close(jhd1313m1_context dev);

    /**
     * Writes a string to the LCD.
     *
     * @param dev The device context.
     * @param buffer Character buffer containing characters to write to
     * the display; note: only ASCII characters are supported
     * @param len The number of characters to write.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_write(const jhd1313m1_context dev, char *buffer,
                                 int len);

    /**
     * Sets the cursor to specified coordinates
     *
     * @param dev The device context.
     * @param row Row to set the cursor to.
     * @param column Column to set the cursor to.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_set_cursor(const jhd1313m1_context dev,
                                      unsigned int row, unsigned int column);

    /**
     * Clears the display of all characters.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_clear(const jhd1313m1_context dev);

    /**
     * Returns to the home coordinates (0,0).
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_home(const jhd1313m1_context dev);

    /**
     * Sets the color of the backlight
     * Can be used to change the color even when the backlight is off
     *
     * @param dev The device context.
     * @param r 0-255 value for red
     * @param g 0-255 value for green
     * @param b 0-255 value for blue
     * @return UPM result.
     */
    upm_result_t jhd1313m1_set_color(const jhd1313m1_context dev,
                                     uint8_t r, uint8_t g, uint8_t b);

    /**
     * Create a custom character.
     *
     * @param dev The device context.
     * @param slot The character slot to write, only 8 are available.
     * @param data The character data (8 bytes) making up the character.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_create_char(const jhd1313m1_context dev,
                                       unsigned int slot,
                                       jhd1313m1_custom_char_t data);

    /**
     * Turn the display on.
     *
     * @param dev The device context.
     * @param on true to turn display on, false otherwise.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_display_on(const jhd1313m1_context dev, bool on);

    /**
     * Turn the cursor on.
     *
     * @param dev The device context.
     * @param on true to turn cursor on, false otherwise.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_cursor_on(const jhd1313m1_context dev, bool on);

    /**
     * Turn cursor blink on.
     *
     * @param dev The device context.
     * @param on true to turn cursor blink on, false otherwise.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_cursor_blink_on(const jhd1313m1_context dev,
                                           bool on);

    /**
     * Turn backlight on.
     *
     * @param dev The device context.
     * @param on true to turn backlight on, false otherwise.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_backlight_on(const jhd1313m1_context dev, bool on);

    /**
     * Makes the LCD scroll text
     *
     * @param direction True if scrolling to the right, false otherwise
     * @return Result of the operation
     */
    upm_result_t jhd1313m1_scroll(const jhd1313m1_context dev, bool direction);

    /**
     * Scroll the display left, without changing the character RAM.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_scroll_display_left(const jhd1313m1_context dev);

    /**
     * Scroll the display right, without changing the character RAM.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_scroll_display_right(const jhd1313m1_context dev);

    /**
     * Set the entry mode so that characters are added left to right.
     *
     * @param dev The device context.
     * @param on true to add characters left to right, false for right
     * to left.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_entry_left_to_right(const jhd1313m1_context dev,
                                               bool on);

    /**
     * Right justify text entered from the cursor.
     *
     * @param dev The device context.
     * @param on true right justify text, false to left justify text.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_autoscroll_on(const jhd1313m1_context dev, bool on);

    /**
     * Send a command byte to the LCD controller.  This is a low level
     * function that should not be used directly unless you know what
     * you are doing.
     *
     * @param dev The device context.
     * @param cmd The command byte to send.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_command(const jhd1313m1_context dev, uint8_t cmd);

    /**
     * Send a data byte the LCD controller.  This is a low level
     * function that should not be used directly unless you know what
     * you are doing.
     *
     * @param dev The device context.
     * @param cmd The command byte to send.
     * @return UPM result.
     */
    upm_result_t jhd1313m1_data(const jhd1313m1_context dev, uint8_t data);


#ifdef __cplusplus
}
#endif
