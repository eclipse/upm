/*
 * Authors: Brendan Le Foll <brendan.le.foll@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *          Sarah Knepper <sarah.knepper@intel.com>
 *          Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014 - 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "upm.h"
#include "mraa/gpio.h"

/**
 * @brief Button library
 * @defgroup buttonlib libupm-button
 * @ingroup seeed gpio button gsk
 */

/**
 * @library buttonlib
 * @sensor button
 * @comname Button
 * @altname Grove Button
 * @type button
 * @man seeed
 * @con gpio
 * @kit gsk
 *
 * @brief API for the Button
 *
 * Basic UPM module for the button sensor
 *
 * @image html button.jpg
 * @snippet button.cxx Interesting
 */

typedef struct _button_context {
    mraa_gpio_context    gpio;
    bool                 isr_installed;
} *button_context;

/**
 * button init function
 *
 * @param pin Pin to use
 */
button_context button_init(int pin);

/**
 * button destructor
 */
void button_close(button_context dev);
/**
 * Gets the value from the GPIO pin
 *
 * @return Value from the GPIO pin
 */
upm_result_t button_get_value(button_context dev, bool* val);
/**
 * Installs an interrupt service routine (ISR) to be called when
 * the button is activated or deactivated.
 *
 * @param fptr Pointer to a function to be called on interrupt
 * @param arg Pointer to an object to be supplied as an
 * argument to the ISR.
 */
upm_result_t button_install_isr(button_context dev, mraa_gpio_edge_t edge_level, void (*isr)(void *), void *arg);
/**
 * Uninstalls the previously installed ISR
 *
 */
upm_result_t button_uninstall_isr(button_context);
