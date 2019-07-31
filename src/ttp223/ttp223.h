/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
 * Installs an interrupt service routine (ISR) to be called when
 * the button is activated or deactivated.
 *
 * @param dev pointer to the sensor struct
 * @param edge_level one of mraa_gpio_edge_t values
 * @param isr pointer to a function to be called on interrupt
 * @param arg pointer to an object to be supplied as an
 * argument to the ISR.
 * @result upm_result_t UPM success/error code
 */
upm_result_t ttp223_install_isr(ttp223_context dev,
                                mraa_gpio_edge_t edge_level,
                                void (*isr)(void *), void *arg);

/**
 * Uninstall a previously installed interrupt handler
 *
 * @param dev pointer to the sensor struct
 * @result upm_result_t UPM success/error code
 */
upm_result_t ttp223_uninstall_isr(ttp223_context dev);

#endif /* TTP223_H_ */
