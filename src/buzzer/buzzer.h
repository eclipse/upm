/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * based on original C++ driver by
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
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <upm.h>

#include <mraa/pwm.h>
#include <buzzer_tones.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file buzzer.h
     * @library buzzer
     * @brief C API for the buzzer driver
     *
     * @include buzzer.c
     */

    /**
     * Device context
     */
    typedef struct _buzzer_context {
        mraa_pwm_context pwm;

        float volume;
        bool initialized;
    } *buzzer_context;

    /**
     * Initialize a buzzer context.
     *
     * @param pin Buzzer pin number (pwm capable)
     * @return buzzer device context
     */
    buzzer_context buzzer_init(int pin);

    /**
     * Buzzer object destructor.
     *
     * @param dev Device context.
     */
    void buzzer_close(buzzer_context dev);

    /**
     * Plays a tone for a certain amount of time or indefinitely. When delay
     * is not used, the sound can be stopped by calling stopSound().
     *
     * @param dev Device context.
     * @param note Note to play (DO, RE, MI, etc.) or frequency
     * @param delay Time in microseconds for which to play the sound;
     * if the value is 0, the sound is played indefinitely
     * @return UPM result
     */
    upm_result_t buzzer_play_sound(const buzzer_context dev, int note,
                                   int delay);

    /**
     * Stops the sound currently playing. Should be called when playSound()
     * does not have a delay value.
     *
     * @param dev Device context.
     * @return UPM result
     */
    upm_result_t buzzer_stop_sound(const buzzer_context dev);

    /**
     * Sets the volume for the buzzer, but may affect the sound timbre.
     * Works best with halved values; e.g., 1.0, 0.5, 0.25, etc.
     *
     * @param dev Device context.
     * @param vol Value to set the volume to, from 0.0 to 1.0
     */
    void buzzer_set_volume(const buzzer_context dev, float vol);

    /**
     * Gets the buzzer volume.
     *
     * @param dev Device context.
     * @return Value the volume was set to
     */
    float buzzer_get_volume(const buzzer_context dev);

#ifdef __cplusplus
}
#endif
