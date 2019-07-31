/*
 * Author: Zion Orent <sorent@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef COLLISION_H_
#define COLLISION_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "upm.h"
#include "mraa/gpio.h"

/**
 * @file collision.h
 * @library collision
 * @brief C API for the MVS0608 Digital Collision Sensor
 *
 * @include collision.c
 */

/**
 * device context
 */
typedef struct _collision_context {
    mraa_gpio_context        gpio;
} *collision_context;

/**
 * Collision Initialization function
 *
 * @param pin number
 * @return void* pointer to the sensor struct
 */
collision_context collision_init(int pin);

/**
 * Collision Initialization function
 *
 * @param void* pointer to the sensor struct
 */
void collision_close(collision_context dev);

/**
 * This function tells you whether the sensor has
 * collided with something or not.
 *
 * @param void* pointer to the sensor struct
 * @param bool* pointer to hold the collision value
 * @return upm_result_t UPM success/error code
 */
upm_result_t collision_is_colliding(collision_context dev, bool* collision_val);

#endif /* COLLISION_H_ */
