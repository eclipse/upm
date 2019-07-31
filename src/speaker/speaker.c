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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <upm_utilities.h>
#include <upm_platform.h>

#include "speaker.h"

typedef struct
{
    int delayTimeLow;
    int delayTimeLowSharp;
    int delayTimeMed;
    int delayTimeMedSharp;
    int delayTimeHigh;
    int delayTimeHighSharp;
} noteData_t;

// keep this synchronized with the switch statement in play_sound()
static noteData_t note_list[7] = {       // index, note
    { 1136, 1073, 568, 536, 284, 268 }, // 0, a
    { 1012, 0, 506, 0, 253, 0 },        // 1, b
    { 1911, 1804, 956, 902, 478, 451 }, // 2, c
    { 1703, 1607, 851, 804, 426, 402 }, // 3, d
    { 1517, 0, 758, 0, 379, 0 },        // 4, e
    { 1432, 1351, 716, 676, 358, 338 }, // 5, f
    { 1276, 1204, 638, 602, 319, 301 }  // 6, g
};

// forward decl
static void speaker_sound(const speaker_context dev, int note_delay);

static speaker_context _common_init()
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    speaker_context dev =
        (speaker_context)malloc(sizeof(struct _speaker_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _speaker_context));

    return dev;
}

// initialization in GPIO mode
speaker_context speaker_init(int pin)
{
    speaker_context dev = NULL;
    if (!(dev = _common_init()))
        return NULL;

    dev->is_pwm = false;

    if (!(dev->gpio = mraa_gpio_init(pin)))
    {
        printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
        speaker_close(dev);
        return NULL;
    }

    mraa_gpio_dir(dev->gpio, MRAA_GPIO_OUT);

    return dev;
}

// initialization in PWM mode
speaker_context speaker_init_pwm(int pin)
{
    speaker_context dev = NULL;
    if (!(dev = _common_init()))
        return NULL;

    dev->is_pwm = true;

    if (!(dev->pwm = mraa_pwm_init(pin)))
    {
        printf("%s: mraa_pwm_init() failed.\n", __FUNCTION__);
        speaker_close(dev);
        return NULL;
    }

    // set the default frequency to 1Khz
    dev->default_freq = 1000;

    return dev;
}

void speaker_close(speaker_context dev)
{
    assert(dev != NULL);

    if (dev->gpio)
        mraa_gpio_close(dev->gpio);

    if (dev->pwm)
    {
        speaker_off(dev);
        mraa_pwm_close(dev->pwm);
    }

    free(dev);
}

upm_result_t speaker_set_frequency(const speaker_context dev,
                                   unsigned int freq)
{
    assert(dev != NULL);

    if (!dev->is_pwm)
        return UPM_ERROR_NO_RESOURCES;

    if (freq < 50 || freq > 32000)
    {
        printf("%s: freq must be between 50 and 32000.\n", __FUNCTION__);

        return UPM_ERROR_OPERATION_FAILED;
    }

    float period = 1.0 / (float)freq;

    // printf("PERIOD: %f (freq %d)\n", period, freq);

    if (period >= 0.001) // ms range
    {
        if (mraa_pwm_period(dev->pwm, period))
        {
            printf("%s: mraa_pwm_period() failed.\n", __FUNCTION__);

            return UPM_ERROR_OPERATION_FAILED;
        }
    }
    else // us range.  With a max of 32KHz, no less than 3.125 us.
    {
        if (mraa_pwm_period_us(dev->pwm, (int)(period * 1000000)))
        {
            printf("%s: mraa_pwm_period_us() failed.\n", __FUNCTION__);

            return UPM_ERROR_OPERATION_FAILED;
        }
    }

    // save it for later if needed
    dev->default_freq = freq;

    // A 10% duty cycle enables better results at high frequencies
    mraa_pwm_write(dev->pwm, 0.1);

    return UPM_SUCCESS;
}

upm_result_t speaker_emit(const speaker_context dev, unsigned int freq,
                          unsigned int emit_ms)
{
    assert(dev != NULL);

    if (!dev->is_pwm)
        return UPM_ERROR_NO_RESOURCES;

    if (speaker_set_frequency(dev, freq))
        return UPM_ERROR_OPERATION_FAILED;

    upm_clock_t clock = upm_clock_init();

    mraa_pwm_enable(dev->pwm, 1);
    while (upm_elapsed_ms(&clock) < emit_ms)
        ; // loop until finished

    mraa_pwm_enable(dev->pwm, 0);

    return UPM_SUCCESS;
}

void speaker_on(const speaker_context dev)
{
    assert(dev != NULL);

    if (!dev->is_pwm)
        return;

    speaker_set_frequency(dev, dev->default_freq);

    mraa_pwm_enable(dev->pwm, 1);
}

void speaker_off(const speaker_context dev)
{
    assert(dev != NULL);

    if (!dev->is_pwm)
        return;

    mraa_pwm_enable(dev->pwm, 0);
}

void speaker_play_all(const speaker_context dev)
{
    assert(dev != NULL);

    if (dev->is_pwm)
        return;

    speaker_play_sound(dev, 'c', false, "low");
    upm_delay_us(200000);
    speaker_play_sound(dev, 'd', false, "low");
    upm_delay_us(200000);
    speaker_play_sound(dev, 'e', false, "low");
    upm_delay_us(200000);
    speaker_play_sound(dev, 'f', false, "low");
    upm_delay_us(200000);
    speaker_play_sound(dev, 'g', false, "low");
    upm_delay_us(500000);
    speaker_play_sound(dev, 'a', false, "low");
    upm_delay_us(500000);
    speaker_play_sound(dev, 'b', false, "low");
    upm_delay_us(500000);
}

void speaker_play_sound(const speaker_context dev, char letter, bool sharp,
                        const char *vocal_weight)
{
    assert(dev != NULL);

    if (dev->is_pwm)
        return;

    int index = 0;
    switch (letter)
    {
    case 'a':
        index = 0;
        break;
    case 'b':
        index = 1;
        break;
    case 'c':
        index = 2;
        break;
    case 'd':
        index = 3;
        break;
    case 'e':
        index = 4;
        break;
    case 'f':
        index = 5;
        break;
    case 'g':
        index = 6;
        break;
    default:
        printf("%s: The note '%c' is invalid.\n", __FUNCTION__, letter);
        return;
    }

    int delayTime = 0;
    bool valid = true;
    if (sharp)
    {
        if (strstr(vocal_weight, "low"))
            delayTime = note_list[index].delayTimeLowSharp;
        else if (strstr(vocal_weight, "med"))
            delayTime = note_list[index].delayTimeMedSharp;
        else if (strstr(vocal_weight, "high"))
            delayTime = note_list[index].delayTimeHighSharp;
        else
            valid = false;
    }
    else
    {
        if (strstr(vocal_weight, "low"))
            delayTime = note_list[index].delayTimeLow;
        else if (strstr(vocal_weight, "med"))
            delayTime = note_list[index].delayTimeMed;
        else if (strstr(vocal_weight, "high"))
            delayTime = note_list[index].delayTimeHigh;
        else
            valid = false;
    }

    if (!valid)
        printf("%s: Correct voice weight values are low, med, or high.\n",
               __FUNCTION__);

    // If delayTime is zero, that means you tried to choose a sharp note
    // for a note that has no sharp
    if (sharp && !delayTime)
    {
        printf("%s: the key '%c' doesn't have a sharp note.\n",
               __FUNCTION__, letter);
        return;
    }

    speaker_sound(dev, delayTime);
}

static void speaker_sound(const speaker_context dev, int note_delay)
{
    assert(dev != NULL);

    for (int i=0; i<100; i++)
    {
        mraa_gpio_write (dev->gpio, 1);
        upm_delay_us(note_delay);
        mraa_gpio_write (dev->gpio, 0);
        upm_delay_us(note_delay);
    }
}
