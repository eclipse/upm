/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *                Sarah Knepper <sarah.knepper@intel.com>
 *                Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef LED_LED_H_
#define LED_LED_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "upm.h"
#include "mraa/gpio.h"
#include "mraa/led.h"

#ifdef __cplusplus
extern "C" {
#endif

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
    mraa_led_context         gpioled;
    int                      led_pin;
    const char*              name;
    int                      max_brightness;
} *led_context;

/**
 * LED Initialization function
 *
 * @param pin GPIO pin to use
 * @return The sensor context
 */
led_context led_init(int pin);

/**
 * LED Initialization function
 *
 * @param name Linux gpioled device to use
 * @return The sensor context
 */
led_context led_init_str(const char* name);

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

#ifdef __cplusplus
}
#endif

#endif /* LED_LED_H_ */
