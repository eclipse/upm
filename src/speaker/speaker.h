/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * Based on original C++ driver by:
 * Author: Zion Orent <sorent@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <unistd.h>
#include <mraa/gpio.h>
#include <mraa/pwm.h>

#include <upm.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file speaker.h
     * @library speaker
     * @brief C API for the Speaker
     *
     * @include speaker.c
     */

    /**
     * Device context
     */
    typedef struct _speaker_context {
        // gpio version
        mraa_gpio_context gpio;

        // pwm version
        mraa_pwm_context  pwm;

        // are using the original GPIO mechanism or PWM?
        bool is_pwm;
        // default frequency, or last frequency specified
        unsigned int default_freq;
    } *speaker_context;

    /**
     * Speaker GPIO init.  In this mode, only the speaker_play_all()
     * and speaker_play_sound() function will work.  The other
     * functions only support PWM mode (see speaker_init_pwm()).
     *
     * @param pin Digital pin to use
     * @return Device context
     */
    speaker_context speaker_init(int pin);

    /**
     * Speaker PWM init.  In this mode a PWM pin is used to emit tones
     * (within the capabilities of your PWM hardware).
     * speaker_play_all() and speaker_play_sound() will not operate in
     * this mode.  The default frequency is set to 1KHz.
     *
     * @param pin Digital PWM capable pin to use
     * @return Device context
     */
    speaker_context speaker_init_pwm(int pin);

    /**
     * Speaker close function
     *
     * @param dev Device context
     */
    void speaker_close(speaker_context dev);

    /**
     * Plays all alto notes (lowest notes).  This function only
     * operates in GPIO mode.
     *
     * @param dev Device context
     */
    void speaker_play_all(const speaker_context dev);

    /**
     * Plays a sound and a note whether it's sharp or not.  This
     * function only operates in GPIO mode.
     *
     * @param dev Device context
     * @param letter Character name of the note
     * ('a', 'b', 'c', 'd', 'e', 'f', or 'g')
     * @param sharp If true, plays a sharp version of the note;
     * otherwise, does not play the note
     * @param vocalWeight String to determine whether to play a low ("low"),
     * a medium ("med"), or a high ("high") note
     */
    void speaker_play_sound(const speaker_context dev, char letter, bool sharp,
                            const char *vocal_weight);

    /**
     * Emit a specific frequency for a given period of time and
     * return.  This function only operates when in PWM mode (ie: the
     * speaker context was initialized with speaker_init_pwm()).  The
     * frequency is limited to between 50-32000Hz.  In addition, the
     * allowable frequencies may be restricted further by the
     * capabilities of your PWM hardware.
     *
     * @param dev Device context
     * @param freq The frequency to emit. Must be between 50 and 32000Hz
     * inclusive.
     * @param emit_ms The number of milliseconds to emit the frequency.
     * @return UPM result
     */
    upm_result_t speaker_emit(const speaker_context dev, unsigned int freq,
                              unsigned int emit_ms);

    /**
     * Set a default frequency to be used with speaker_on() and
     * speaker_off().  This function only operates when in PWM mode
     * (ie: the speaker context was initialized with
     * speaker_init_pwm()).  The frequency is limited to between
     * 50-32000Hz.  In addition, the allowable frequencies may be
     * restricted further by the capabilities of your PWM hardware.
     *
     * @param dev Device context
     * @param freq The frequency to emit. Must be between 50 and 32000Hz
     * inclusive.
     * @return UPM result
     */
    upm_result_t speaker_set_frequency(const speaker_context dev,
                                       unsigned int freq);

    /**
     * Turn the speaker on, and emit the frequency last specified with
     * speaker_set_frequency() or speaker_emit().  This function only
     * operates when in PWM mode (ie: the speaker context was
     * initialized with speaker_init_pwm()).
     *
     * @param dev Device context
     */
    void speaker_on(const speaker_context dev);

    /**
     * Turn the speaker off.  This function only operates when in PWM
     * mode (ie: the speaker context was initialized with
     * speaker_init_pwm()).
     *
     * @param dev Device context
     */
    void speaker_off(const speaker_context dev);

#ifdef __cplusplus
}
#endif
