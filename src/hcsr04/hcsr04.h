/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Author: Rafael Neri <rafael.neri@gmail.com>
 * Author: Jun Kato <i@junkato.jp>
 * Contributions by: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014-2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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