/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2016 Intel Corporation
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
#include "mraa/spi.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file apa102.h
 * @library apa102
 * @brief C API for controlling APA102/DotStar RGB LED Strips
 *
 * @include apa102.c
 */

/**
 * Device context
 */
typedef struct _apa102_context {
    mraa_spi_context        spi;

    // optional chip select
    mraa_gpio_context       cs;

    uint8_t*    buffer;
    int         leds;
    int         framelength;
} *apa102_context;


/**
 * Instantiates a new APA102 LED Strip
 *
 * @param ledcount  Number of LEDs on the strip
 * @param bus       SPI bus to use
 * @param cs        Pin to use for chip select. -1 if not used.
 * @return an initialized apa102 context on success, NULL on error.
 */
apa102_context apa102_init(int ledcount, int bus, int cs);

/**
 * APA102 close function
 *
 * @param dev The apa102_context to close
 */
void apa102_close(apa102_context dev);

/**
 * Sets the color and brightness for one LED in the buffer
 *
 * @param dev           The apa102_context to use
 * @param index         Index of the LED (0 based)
 * @param brightness    Brightness value (0-31)
 * @param r             Red component (0-255)
 * @param g             Green component (0-255)
 * @param b             Blue component (0-255)
 * @return upm_result_t UPM success/error code
 */
upm_result_t apa102_set_led(apa102_context dev, uint16_t index, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b);

/**
 * Sets the brightness for one LED in the buffer
 *
 * @param dev           The apa102_context to use
 * @param index         Index of the LED (0 based)
 * @param brightness    Brightness value (0-31)
 * @return upm_result_t UPM success/error code
 */
upm_result_t apa102_set_led_brightness(apa102_context dev, uint16_t index, uint8_t brightness);

/**
 * Sets the color and brightness for multiple LEDs in the buffer
 *
 * @param dev           The apa102_context to use
 * @param s_index       The start Index of the LED range (0 based)
 * @param e_index       The end Index of the LED range (0 based)
 * @param brightness    Brightness value (0-31)
 * @param r             Red component (0-255)
 * @param g             Green component (0-255)
 * @param b             Blue component (0-255)
 * @return upm_result_t UPM success/error code
 */
upm_result_t apa102_set_leds(apa102_context dev, uint16_t s_index, uint16_t e_index, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b);

/**
 * Sets the brightness for multiple LEDs in the buffer
 *
 * @param dev           The apa102_context to use
 * @param s_index       The start Index of the LED range (0 based)
 * @param e_index       The end Index of the LED range (0 based)
 * @param brightness    Brightness value (0-31)
 * @return upm_result_t UPM success/error code
 */
upm_result_t apa102_set_leds_brightness(apa102_context dev, uint16_t s_index, uint16_t e_index, uint8_t brightness);

/**
 * Writes the buffer to the SPI bus thus updating the LED Strip
 *
 * @param dev The apa102_context to use
 * @return upm_result_t UPM success/error code
 */
upm_result_t apa102_refresh(apa102_context dev);

#ifdef __cplusplus
}
#endif
