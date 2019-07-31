/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
    int                    gpio_pin;
    bool                   isr_installed;
} *a110x_context;

/**
 * A110X Initialization function
 *
 * @param pin GPIO pin to use
 * @return device context pointer
 */
a110x_context a110x_init(int pin);

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
