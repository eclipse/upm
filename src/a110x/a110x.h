/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
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
#ifndef A110X_A110X_H_
#define A110X_A110X_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "upm.h"
#include "mraa/gpio.h"

/**
 * @file a110x.h
 * @library a110x
 * @brief C API for the A110X Hall Effect sensors
 *
 * @include a110x.c
 */

/**
 * device context
 */
typedef struct _a110x_context {
    mraa_gpio_context      gpio;
    uint8_t                gpio_pin;
    bool                   isr_installed;
} *a110x_context;

/**
 * A110X Initialization function
 *
 * @param pin GPIO pin to use
 * @return device context pointer
 */
a110x_context a110x_init(uint8_t pin);

/**
 * A110X Initialization function
 *
 * @param dev a110x_context pointer
 */
void a110x_close(a110x_context dev);

/**
 * Determines whether a magnetic field of south polarity has been detected
 *
 * @param dev a110x_context pointer
 * @param bool* to note the response
 * @return upm_result_t UPM success/error code
 */
upm_result_t a110x_magnet_detected(a110x_context dev, bool* res);

/**
 * Installs an interrupt service routine (ISR) to be called when
 * the appropriate magnetic field is detected
 *
 * @param dev a110x_context pointer
 * @param mraa_gpio_edge_t edge trigger level
 * @param isr ISR callback function
 * @return upm_result_t UPM success/error code
 */
upm_result_t a110x_install_isr(a110x_context dev,
                               mraa_gpio_edge_t edge_level,
                               void (*isr)(void *), void *arg);

/**
 * Uninstalls the previously installed ISR
 *
 * @param dev a110x_context pointer
 * @return upm_result_t UPM success/error code
 */
upm_result_t a110x_uninstall_isr(a110x_context dev);

#endif /* A110X_A110X_H_ */
