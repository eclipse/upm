/*
 * Author: Andrei Vasiliu <andrei.vasiliu@intel.com>
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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

#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>

#include "pulsensor.h"

#if defined(JAVACALLBACK)
Pulsensor::Pulsensor (Callback *obj_call) : pin_ctx(0)
{
    obj_callback = obj_call;

    sample_counter = 0;
    last_beat_time = 0;
    threshold      = 512;
    ibi            = 600;
    trough         = 512;
    peak           = 512;
    is_pulse       = FALSE;
    ret            = FALSE;
    bpm            = 0;
    qs             = FALSE;
    apmlitude      = 100;
}
#else
Pulsensor::Pulsensor (callback_handler handler) : pin_ctx(0)
{
    callback = handler;

    sample_counter = 0;
    last_beat_time = 0;
    threshold      = 512;
    ibi            = 600;
    trough         = 512;
    peak           = 512;
    is_pulse       = FALSE;
    ret            = FALSE;
    bpm            = 0;
    qs             = FALSE;
    apmlitude      = 100;
}
#endif

void Pulsensor::start_sampler ()
{
    int error;
    ctx_counter++;
    usleep (100000);
    error = pthread_create (&(sample_thread), NULL, &Pulsensor::do_sample, this);
    if (error != 0) {
        printf ("ERROR : Cannot created sampler thread.\n");
    }
}

void Pulsensor::stop_sampler () {
    ctx_counter--;
}

void *Pulsensor::do_sample (void *arg) {
    int data_from_sensor;
    clbk_data callback_data;

    Pulsensor *pulsensor = static_cast<Pulsensor *>(arg);

    while (pulsensor->ctx_counter) {
        data_from_sensor = pulsensor->pin_ctx.read ();
        pulsensor->ret = FALSE;

        pulsensor->sample_counter += 2;
        int N = pulsensor->sample_counter - pulsensor->last_beat_time;

        if (data_from_sensor < pulsensor->threshold &&
            N > ( pulsensor->ibi / 5)* 3) {
            if (data_from_sensor < pulsensor->trough) {
                pulsensor->trough = data_from_sensor;
            }
        }

        if (data_from_sensor > pulsensor->threshold &&
            data_from_sensor > pulsensor->peak) {
            pulsensor->peak = data_from_sensor;
        }

        if (N > 250) {
            // printf ("(NO_GDB) DEBUG\n");
            if ( (data_from_sensor > pulsensor->threshold) &&
                    (pulsensor->is_pulse == FALSE) &&
                    (N > (pulsensor->ibi / 5)* 3) ) {
                pulsensor->is_pulse = callback_data.is_heart_beat = TRUE;
#if defined(JAVACALLBACK)
                pulsensor->obj_callback->run(callback_data);
#else
                pulsensor->callback(callback_data);
#endif

                pulsensor->ibi = pulsensor->sample_counter - pulsensor->last_beat_time;
                pulsensor->last_beat_time = pulsensor->sample_counter;

                // second beat
                if (pulsensor->second_beat) {
                    pulsensor->second_beat = FALSE;
                    for (int i = 0; i <= 9; i++) {
                        pulsensor->ibi_rate[i] = pulsensor->ibi;
                    }
                }

                // first beat
                if (pulsensor->first_beat) {
                    pulsensor->first_beat  = FALSE;
                    pulsensor->second_beat = TRUE;
                    pulsensor->ret = TRUE;
                } else {
                    uint32_t running_total = 0;
                    for(int i = 0; i <= 8; i++){
                        pulsensor->ibi_rate[i] = pulsensor->ibi_rate[i+1];
                        running_total += pulsensor->ibi_rate[i];
                    }

                    pulsensor->ibi_rate[9] = pulsensor->ibi;
                    running_total += pulsensor->ibi_rate[9];
                    running_total /= 10;
                    pulsensor->bpm = 60000 / running_total;
                    pulsensor->qs = TRUE;
                }
            }
        }

        if (pulsensor->ret == FALSE) {
            if (data_from_sensor < pulsensor->threshold &&
                pulsensor->is_pulse == TRUE) {
                pulsensor->is_pulse = callback_data.is_heart_beat = FALSE;
#if defined(JAVACALLBACK)
                pulsensor->obj_callback->run(callback_data);
#else
                pulsensor->callback(callback_data);
#endif
                pulsensor->is_pulse   = FALSE;
                pulsensor->apmlitude  = pulsensor->peak - pulsensor->trough;
                pulsensor->threshold  = pulsensor->apmlitude / 2 + pulsensor->trough;
                pulsensor->peak       = pulsensor->threshold;
                pulsensor->trough     = pulsensor->threshold;
            }

            if (N > 2500) {
                pulsensor->threshold      = 512;
                pulsensor->peak           = 512;
                pulsensor->trough         = 512;
                pulsensor->last_beat_time = pulsensor->sample_counter;
                pulsensor->first_beat     = TRUE;
                pulsensor->second_beat    = FALSE;
            }
        }

        usleep (2000);
    }
    return NULL;
}
