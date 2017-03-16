/*
 * Author: Sergey Kiselev <sergey.kiselev@intel.com>
 * Copyright (c) 2017 Sergey Kiselev
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

#include <stdint.h>
#include "upm.h"
#include "mraa/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file p9813.h
 * @library p9813
 * @brief C API for controlling P9813/Chainable RGN LEDs
 *
 * @include p9813.c
 */

/**
 * Device context
 */
typedef struct _p9813_context {
    // clock and data GPIO pins
    mraa_gpio_context       clk, data;

    uint8_t*    buffer;
    int         leds;
} *p9813_context;


/**
 * Instantiates a new P9813 Chainable RGB LEDs
 *
 * @param ledcount  Number of LEDs in the chain
 * @param clk       Clock pin
 * @param data      Data pin
 * @return an initialized p9813 context on success, NULL on error.
 */
p9813_context p9813_init(int ledcount, int clk, int data);

/**
 * P9813 close function
 *
 * @param dev The p9813_context to close
 */
void p9813_close(p9813_context dev);

/**
 * Sets the color for one LED in the buffer
 *
 * @param dev           The p9813_context to use
 * @param index         Index of the LED (0 based)
 * @param r             Red component (0-255)
 * @param g             Green component (0-255)
 * @param b             Blue component (0-255)
 * @return upm_result_t UPM success/error code
 */
upm_result_t p9813_set_led(p9813_context dev, uint16_t index, uint8_t r, uint8_t g, uint8_t b);

/**
 * Sets the color for multiple LEDs in the buffer
 *
 * @param dev           The p9813_context to use
 * @param s_index       The start Index of the LED range (0 based)
 * @param e_index       The end Index of the LED range (0 based)
 * @param r             Red component (0-255)
 * @param g             Green component (0-255)
 * @param b             Blue component (0-255)
 * @return upm_result_t UPM success/error code
 */
upm_result_t p9813_set_leds(p9813_context dev, uint16_t s_index, uint16_t e_index, uint8_t r, uint8_t g, uint8_t b);

/**
 * Writes the buffer to the LED controllers thus updating the LEDs
 *
 * @param dev The p9813_context to use
 * @return upm_result_t UPM success/error code
 */
upm_result_t p9813_refresh(p9813_context dev);

#ifdef __cplusplus
}
#endif
