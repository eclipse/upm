/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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

#ifndef TTP223_H_
#define TTP223_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "upm.h"
#include "mraa/gpio.h"

/**
 * @file ttp223.h
 * @library ttp223
 * @brief C API for the TTP223 Touch Detector Sensor
 *
 * @include ttp223.c
 */

/**
 * device context
 */
typedef struct _ttp223_context {
    mraa_gpio_context       gpio;
    uint8_t                 gpio_pin;
    bool                    isr_installed;
} *ttp223_context;

/**
 * Sensor Init function
 *
 * @param pin The pin number the sensor is attached to
 * @return void* pointer to the sensor struct
 */
ttp223_context ttp223_init(int pin);

/**
 * Sensor Module close function
 *
 * @param dev pointer to the sensor struct
 */
void ttp223_close(ttp223_context dev);

/**
 * Function to tell if the sensor is pressed
 *
 * @param dev pointer to the sensor struct
 * @param value pointer to store whether the sensor is pressed or not
 * @result upm_result_t UPM success/error code
 */
upm_result_t ttp223_is_pressed(ttp223_context dev, bool* value);

/**
 *
 */
upm_result_t ttp223_install_isr(ttp223_context dev,
                                mraa_gpio_edge_t edge_level,
                                void (*isr)(void *), void *arg);

/**
 *
 */
upm_result_t ttp223_uninstall_isr(ttp223_context);

#endif /* TTP223_H_ */
