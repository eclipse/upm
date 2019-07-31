/*
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 *
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef RELAY_H_
#define RELAY_H_

#pragma once
#include <stdlib.h>
#include <unistd.h>

#include "upm.h"
#include <mraa/gpio.h>

/**
 * @file relay.h
 * @library relay
 * @brief C API for Relays
 *
 * @include relay.c
 */

/**
 * device context
 */
typedef struct _relay_context{
    mraa_gpio_context gpio;
} *relay_context;

/**
 * relay init function
 *
 * @param pin Pin to use
 */
relay_context relay_init(int pin);

/**
 * relay destructor
 */
void relay_close(relay_context dev);

/**
 * Sets the relay switch to on (closed circuit). This allows current
 * to flow and lights up the indicator LED.
 *
 * @return result of the operation
 */
upm_result_t relay_on(relay_context dev);

/**
 * Sets the relay switch to off (open circuit). This stops current
 * from flowing and the indicator LED is not lit.
 *
 * @return result of the operation
 */
upm_result_t relay_off(relay_context dev);

/**
 * Checks whether the relay switch is closed.
 *
 * @return True if the switch is on (closed), false otherwise
 */
bool relay_is_on(relay_context dev);

/**
 * Checks whether the relay switch is open.
 *
 * @return True if the switch is off (open), false otherwise
 */
bool relay_is_off(relay_context dev);

#endif /* RELAY_H_ */
