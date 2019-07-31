/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * Based on UPM C++ drivers originally developed by:
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <mraa/gpio.h>
#include <mraa/aio.h>

#include <upm.h>
#include "lcm1602.h"

#ifdef __cplusplus
extern "C" {
#endif


    /**
     * @file lcdks.h
     * @library lcdks
     * @brief C API for the LCDKS (LCD Keypad Shield) display family
     *
     * @include lcdks.c
     */

    /**
     * Device context
     */
    typedef struct _lcdks_context {
        // LCM1602 context - does most of the work
        lcm1602_context lcm1602;
        // aio context for keypad
        mraa_aio_context aio;
        // optional GPIO context for backlight pin
        mraa_gpio_context gpio;
    } *lcdks_context;

    /**
     * LCDKS initialization
     *
     * As this is a shield, you will not likely have any choice over
     * the pins that are used.  For this reason, we provide defaults
     * for all of them -- of course they can be changed if your device
     * is different.
     *
     * @param rs Register select pin.
     * @param enable Enable pin.
     * @param d0 Data 0 pin.
     * @param d1 Data 1 pin.
     * @param d2 Data 2 pin.
     * @param d3 Data 3 pin.
     * @param keypad Analog pin of the keypad.
     * @param backlight Optional GPIO backlight pin.  Specify -1 if
     * not in use or not supported on your device.
     * @return LCDKS context, or NULL if an error occurs.
     */
    lcdks_context lcdks_init(int rs, int enable,
                             int d0, int d1, int d2, int d3,
                             int keypad, int backlight);

    /**
     * LCDKS close.
     *
     * @param dev The device context.
     */
    void lcdks_close(lcdks_context dev);

    /**
     * Writes a string to the LCD.
     *
     * @param dev The device context.
     * @param buffer Character buffer containing characters to write to
     * the display; note: only ASCII characters are supported
     * @param len The number of characters to write.
     * @return UPM result.
     */
    upm_result_t lcdks_write(const lcdks_context dev, char *buffer,
                                 int len);

    /**
     * Sets the cursor to specified coordinates
     *
     * @param dev The device context.
     * @param row Row to set the cursor to.
     * @param column Column to set the cursor to.
     * @return UPM result.
     */
    upm_result_t lcdks_set_cursor(const lcdks_context dev,
                                      unsigned int row, unsigned int column);

    /**
     * Clears the display of all characters.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t lcdks_clear(const lcdks_context dev);

    /**
     * Returns to the home coordinates (0,0).
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t lcdks_home(const lcdks_context dev);

    /**
     * Create a custom character.
     *
     * @param dev The device context.
     * @param slot The character slot to write, only 8 are available.
     * @param data The character data (8 bytes) making up the character.
     * @return UPM result.
     */
    upm_result_t lcdks_create_char(const lcdks_context dev,
                                       unsigned int slot,
                                       char *data);

    /**
     * Turn the display on.
     *
     * @param dev The device context.
     * @param on true to turn display on, false otherwise.
     * @return UPM result.
     */
    upm_result_t lcdks_display_on(const lcdks_context dev, bool on);

    /**
     * Turn the cursor on.
     *
     * @param dev The device context.
     * @param on true to turn cursor on, false otherwise.
     * @return UPM result.
     */
    upm_result_t lcdks_cursor_on(const lcdks_context dev, bool on);

    /**
     * Turn cursor blink on.
     *
     * @param dev The device context.
     * @param on true to turn cursor blink on, false otherwise.
     * @return UPM result.
     */
    upm_result_t lcdks_cursor_blink_on(const lcdks_context dev,
                                           bool on);

    /**
     * Turn backlight on.
     *
     * @param dev The device context.
     * @param on true to turn backlight on, false otherwise.
     * @return UPM result.
     */
    upm_result_t lcdks_backlight_on(const lcdks_context dev, bool on);

    /**
     * Scroll the display left, without changing the character RAM.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t lcdks_scroll_display_left(const lcdks_context dev);

    /**
     * Scroll the display right, without changing the character RAM.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t lcdks_scroll_display_right(const lcdks_context dev);

    /**
     * Set the entry mode so that characters are added left to right.
     *
     * @param dev The device context.
     * @param on true to add characters left to right, false for right
     * to left.
     * @return UPM result.
     */
    upm_result_t lcdks_entry_left_to_right(const lcdks_context dev,
                                               bool on);

    /**
     * Right justify text entered from the cursor.
     *
     * @param dev The device context.
     * @param on true right justify text, false to left justify text.
     * @return UPM result.
     */
    upm_result_t lcdks_autoscroll_on(const lcdks_context dev, bool on);

    /**
     * Returns the floating point representation of the key that is
     * being pushed.  Each key produces a different value between 0.0
     * and 1.0, and only one key can be read at a time.
     *
     * @param dev The device context.
     * @return the floating point value representing a key
     */
    float lcdks_get_key_value(const lcdks_context dev);


#ifdef __cplusplus
}
#endif
