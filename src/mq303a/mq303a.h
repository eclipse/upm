/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
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
#ifndef MQ303A_MQ303A_H_
#define MQ303A_MQ303A_H_

#include "upm.h"
#include "mraa/aio.h"
#include "mraa/gpio.h"

/**
 * @file mq303a.h
 * @library mq303
 * @brief C API for the MQ303A Alcohol Sensor
 *
 * @include mq303.c
 */

/**
 * device context
 */
typedef struct _mq303a_context {
    mraa_gpio_context       gpio;
    mraa_aio_context        aio;
    uint8_t                 gpio_pin;
    uint8_t                 aio_pin;
} *mq303a_context;

/**
 * MQ303A Initialization function
 *
 * @param pin Analog pin to use
 * @param heaterPin Digital pin mapped to the analog pin to use
 * @return void* pointer to the sensor struct
 */
mq303a_context mq303a_init(int pin, int heater_pin);

/**
 * MQ303A Initialization function
 *
 * @param void* pointer to the sensor struct
 */
void mq303a_close(mq303a_context dev);

/**
 * This function gets the value of the alcohol content
 *
 * @param void* pointer to the sensor struct
 * @param *val pointer to store the reading
 * @return upm_result_t UPM success/error code
 */
upm_result_t mq303a_get_value(mq303a_context dev, int* val);

/**
 * This function enables/disables the heater
 * on board the sensor.
 *
 * @param void* pointer to the sensor struct
 * @param bool to turn on/off heater
 * @return upm_result_t UPM success/error code
 */
upm_result_t mq303a_heater_enable(mq303a_context dev, bool enable);

#endif /* MQ303A_MQ303A_H_ */
