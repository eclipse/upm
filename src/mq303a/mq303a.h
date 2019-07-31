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
