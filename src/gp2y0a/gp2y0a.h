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
    uint8_t                     pin;
    int                         a_res;
} *gp2y0a_context;

/**
 * GP2Y0A sensor init function
 *
 * @param pin Analog pin to use
 * @param a_ref reference voltage
 */
gp2y0a_context gp2y0a_init(uint8_t pin, float a_ref);

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
