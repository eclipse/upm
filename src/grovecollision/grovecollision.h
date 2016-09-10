/*
 * Author: Zion Orent <sorent@ics.com>
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
#ifndef MVS0608_H_
#define MVS0608_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "upm.h"
#include "mraa/gpio.h"

/**
 * @brief MVS0608 - Grove Collision Sensor library
 * @ingroup gpio accelerometer
 */

/**
 * @library mvs0608
 * @sensor mvs0608
 * @comname Grove Collision Sensor
 * @type accelerometer
 * @con gpio
 *
 * @brief API for the mvs0608
 *
 * MVS0608 can detect whether any
 * collision movement or vibration happens.
 * It outputs a low pulse signal when vibration is detected.
 *
 * @image html grovecollision.jpg
 * @snippet grovecollision.cxx Interesting
 */

/**
 * device context
 */
typedef struct _mvs0608_context {
    mraa_gpio_context        gpio;
    uint8_t                  gpio_pin;
} *mvs0608_context;

/**
 * MVS0608 Initialization function
 *
 * @param pin number
 * @return void* pointer to the sensor struct
 */
mvs0608_context mvs0608_init(int pin);

/**
 * MVS0608 Initialization function
 *
 * @param void* pointer to the sensor struct
 */
void mvs0608_close(mvs0608_context dev);

/**
 * This function tells you whether the sensor has
 * collided with something or not.
 *
 * @param void* pointer to the sensor struct
 * @param bool* pointer to hold the collision value
 * @return upm_result_t UPM success/error code
 */
upm_result_t mvs0608_is_colliding(mvs0608_context dev, bool* collision_val);

#endif /* MVS0608_H_ */
