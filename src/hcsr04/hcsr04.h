/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Author: Rafael Neri <rafael.neri@gmail.com>
 * Author: Jun Kato <i@junkato.jp>
 * Contributions by: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014-2015 Intel Corporation.
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

#include <stdio.h>
#include <string.h>
#include <mraa/gpio.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HCSR04_CM = 0,
    HCSR04_INCH } HCSR04_U;

/**
 * @file hcsr04.h
 * @library hcsr04
 * @brief C API for the HCSR04 Ultrasonic Ranger sensor
 *
 * @include hcsr04.c
 */

typedef struct _hcsr04_context {
    mraa_gpio_context        trigPin;
    mraa_gpio_context        echoPin;
    int                      interruptCounter;
    long                     startTime;
    long                     endTime;
} *hcsr04_context;

/**
 * HCSR04 Initialization function
 *
 * @param triggerPin GPIO pin for trigger
 * @param echoPin GPIO pin used for output from sensor
 * @return hcsr04_context
 */
hcsr04_context hcsr04_init(int triggerPin, int echoPin);

/**
 * HCSR04 Close function
 *
 * @param dev hcsr04_context pointer
 */
void hcsr04_close(hcsr04_context dev);

/**
 * Function to get the distance from the HCSR04 sensor
 *
 * @param unit cm/inches
 * @return distance in specified unit
 */
double hcsr04_get_distance(hcsr04_context dev, HCSR04_U unit);

#ifdef __cplusplus
}
#endif