/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *                Sarah Knepper <sarah.knepper@intel.com>
 *                Abhishek Malik <abhishek.malik@intel.com>
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
#ifndef LED_LED_H_
#define LED_LED_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "upm.h"
#include "mraa/gpio.h"

/**
 * @file led.h
 * @library led
 * @brief C API for LEDs
 *
 * @include led.c
 */

/**
 * device context
 */
typedef struct _led_context {
    mraa_gpio_context        gpio;
    uint8_t                  led_pin;
} *led_context;

/**
 * LED Initialization function
 *
 * @param pin GPIO pin to use
 * @return The sensor context
 */
led_context led_init(uint8_t pin);

/**
 * LED Initialization function
 *
 * @param The sensor context
 */
void led_close(led_context dev);

/**
 * Function to turn LED on
 *
 * @param The sensor context
 * @return upm_result_t UPM success/error code
 */
upm_result_t led_on(led_context dev);

/**
 * Function to turn LED off
 *
 * @param The sensor context
 * @return upm_result_t UPM success/error code
 */
upm_result_t led_off(led_context dev);

#endif /* LED_LED_H_ */
