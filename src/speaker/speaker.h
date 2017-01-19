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

#include <unistd.h>
#include <mraa/gpio.h>
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
        mraa_gpio_context gpio;

    } *speaker_context;

    /**
     * Speaker GPIO init
     *
     * @param pin Digital pin to use
     */
    speaker_context speaker_init(int pin);

    /**
     * Speaker close function
     *
     * @param dev Device context
     */
    void speaker_close(speaker_context dev);

    /**
     * Plays all alto notes (lowest notes)
     *
     * @param dev Device context
     */
    void speaker_play_all(const speaker_context dev);

    /**
     * Plays a sound and a note whether it's sharp or not
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

#ifdef __cplusplus
}
#endif
