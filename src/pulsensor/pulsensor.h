/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Credits to Adafruit.
 * Based on Adafruit BMP085 library.
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
#pragma once

#include <string>
#include <math.h>
#include <mraa/pwm.h>
#include <mraa/aio.h>
#include <mraa/gpio.h>
#include <pthread.h>

#define HIGH               1
#define LOW                0

#define TRUE               HIGH
#define FALSE              LOW

struct clbk_data {
    int is_heart_beat;
};

typedef void (* callback_handler) (clbk_data);

struct pulsensor_context {
    pthread_t        sample_thread;
    uint32_t         sample_counter;
    uint32_t         last_beat_time;
    int              threshold;
    int              ibi_rate[10];
    int              ibi;
    int              trough;
    int              peak;
    int              bpm;
    int              apmlitude;
    uint8_t          qs;
    uint8_t          is_pulse;
    uint8_t          first_beat;
    uint8_t          second_beat;
    uint8_t          pin;
    uint8_t          ret;
    mraa_aio_context  pin_ctx;

    callback_handler callback;
};

static volatile uint16_t ctx_counter = 0;

void init_pulsensor (pulsensor_context * ctx, callback_handler handler);
void start_sampler (pulsensor_context * ctx);
void stop_sampler (pulsensor_context * ctx);

void * do_sample (void * arg);
