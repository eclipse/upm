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

#ifndef GP2Y0A_GP2Y0A_H_
#define GP2Y0A_GP2Y0A_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#include "upm.h"
#include "mraa/aio.h"

/**
 * @file gp2y0a.h
 * @library gp2y0a
 * @brief C API for GP2Y0A IR Proximity Sensors
 *
 * @include gp2y0a.c
 */

/**
 * device context
 */
typedef struct _gp2y0a_context {
    mraa_aio_context            aio;
    int                         pin;
    int                         a_res;
} *gp2y0a_context;

/**
 * GP2Y0A sensor init function
 *
 * @param pin Analog pin to use
 * @param a_ref reference voltage
 */
gp2y0a_context gp2y0a_init(int pin, float a_ref);

/**
 * GP2Y0A close function
 *
 * @param dev sensor struct
 */
void gp2y0a_close(gp2y0a_context dev);

/**
 * Gets an average voltage value from the sensor
 *
 * @param dev sensor struct
 * @param aref Reference voltage in use (usually 5.0V or 3.3V)
 * @param samples Number of samples to average over
 * @return Average voltage reading
 */
upm_result_t gp2y0a_get_value(gp2y0a_context dev, float a_ref, uint8_t samples,
                              float* val);

#endif /* GP2Y0A_GP2Y0A_H_ */
