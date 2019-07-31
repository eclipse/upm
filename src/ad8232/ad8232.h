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

#ifndef AD8232_H_
#define AD8232_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "upm.h"
#include "mraa/aio.h"
#include "mraa/gpio.h"

#define AD8232_DEFAULT_AREF  3.3

/**
 * @file ad8232.h
 * @library ad8232
 * @brief C API for the AD8232 Heart Rate Monitor
 *
 * @include ad8232.c
 */

/**
 * device context
 */
typedef struct _ad8232_context {
    mraa_aio_context        aio;
    mraa_gpio_context       gpio_lo_plus;
    mraa_gpio_context       gpio_lo_minus;
    int                     a_res;
} *ad8232_context;

/**
 * Sensor init function
 *
 * @param lo_plus Digital pin to use for LO+
 * @param lo_minus Digital pin to use for LO-
 * @param output Analog pin to read the data
 * @param a_ref Analog voltage reference
 */
ad8232_context ad8232_init(int lo_plus, int lo_minus, int output, float a_ref);

/**
 * Sensor destructor
 */
void ad8232_close(ad8232_context dev);

    /**
     * Returns the current ADC value for the device output pin.  If an
     * LO (leads off) event is detected, 0 is returned.
     *
     * @param dev sensor context pointer
     * @param value ADC value
     * @return result status code
     */
upm_result_t ad8232_get_value(ad8232_context dev, int* value);

#endif /* AD8232_H_ */