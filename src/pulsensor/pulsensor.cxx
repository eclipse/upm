/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include <unistd.h>
#include <stdlib.h>
#include "pulsensor.h"

void init_pulsensor (pulsensor_context * ctx, callback_handler handler) {
    ctx->callback = handler;

    ctx->pin_ctx = mraa_aio_init(0);

    ctx->sample_counter = 0;
    ctx->last_beat_time = 0;
    ctx->threshold      = 512;
    ctx->ibi            = 600;
    ctx->trough         = 512;
    ctx->peak           = 512;
    ctx->is_pulse       = FALSE;
    ctx->ret            = FALSE;
    ctx->bpm            = 0;
    ctx->qs             = FALSE;
    ctx->apmlitude      = 100;
}

void start_sampler (pulsensor_context * ctx) {
    int error;
    ctx_counter++;
    usleep (100000);
    error = pthread_create (&(ctx->sample_thread), NULL, do_sample, (void *) ctx);
    if (error != 0) {
        printf ("ERROR : Cannot created sampler thread.\n");
    }
}

void stop_sampler (pulsensor_context * ctx) {
    ctx_counter--;
}

void * do_sample (void * arg) {
    int data_from_sensor;
    clbk_data callback_data;
    while (ctx_counter) {
        pulsensor_context * ctx = (pulsensor_context *) arg;
        mraa_aio_context pin = ctx->pin_ctx;
        data_from_sensor = mraa_aio_read (pin);
        ctx->ret = FALSE;

        ctx->sample_counter += 2;
        int N = ctx->sample_counter - ctx->last_beat_time;

        if (data_from_sensor < ctx->threshold && N > ( ctx->ibi / 5)* 3) {
            if (data_from_sensor < ctx->trough) {
                ctx->trough = data_from_sensor;
            }
        }

        if (data_from_sensor > ctx->threshold && data_from_sensor > ctx->peak) {
            ctx->peak = data_from_sensor;
        }

        if (N > 250) {
            // printf ("(NO_GDB) DEBUG\n");
            if ( (data_from_sensor > ctx->threshold) &&
                    (ctx->is_pulse == FALSE) &&
                    (N > (ctx->ibi / 5)* 3) ) {
                ctx->is_pulse = callback_data.is_heart_beat = TRUE;
                ((pulsensor_context *) arg)->callback(callback_data);

                ctx->ibi = ctx->sample_counter - ctx->last_beat_time;
                ctx->last_beat_time = ctx->sample_counter;

                // second beat
                if (ctx->second_beat) {
                    ctx->second_beat = FALSE;
                    for (int i = 0; i <= 9; i++) {
                        ctx->ibi_rate[i] = ctx->ibi;
                    }
                }

                // first beat
                if (ctx->first_beat) {
                    ctx->first_beat  = FALSE;
                    ctx->second_beat = TRUE;
                    ctx->ret = TRUE;
                } else {
                    uint32_t running_total = 0;
                    for(int i = 0; i <= 8; i++){
                        ctx->ibi_rate[i] = ctx->ibi_rate[i+1];
                        running_total += ctx->ibi_rate[i];
                    }

                    ctx->ibi_rate[9] = ctx->ibi;
                    running_total += ctx->ibi_rate[9];
                    running_total /= 10;
                    ctx->bpm = 60000 / running_total;
                    ctx->qs = TRUE;
                }
            }
        }

        if (ctx->ret == FALSE) {
            if (data_from_sensor < ctx->threshold && ctx->is_pulse == TRUE) {
                ctx->is_pulse = callback_data.is_heart_beat = FALSE;
                ((pulsensor_context *) arg)->callback(callback_data);

                ctx->is_pulse   = FALSE;
                ctx->apmlitude  = ctx->peak - ctx->trough;
                ctx->threshold  = ctx->apmlitude / 2 + ctx->trough;
                ctx->peak       = ctx->threshold;
                ctx->trough     = ctx->threshold;
            }

            if (N > 2500) {
                ctx->threshold      = 512;
                ctx->peak           = 512;
                ctx->trough         = 512;
                ctx->last_beat_time = ctx->sample_counter;
                ctx->first_beat     = TRUE;
                ctx->second_beat    = FALSE;
            }
        }

        usleep (2000);
    }
}
