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

speaker_context speaker_init(int pin)
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

    if (!(dev->gpio = mraa_gpio_init(pin)))
    {
        printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
        speaker_close(dev);
        return NULL;
    }

#if defined(UPM_PLATFORM_LINUX)
    // Would prefer, but not fatal if not available
    mraa_gpio_use_mmaped(dev->gpio, 1);
#endif // UPM_PLATFORM_LINUX

    mraa_gpio_dir(dev->gpio, MRAA_GPIO_OUT);

    return dev;
}

void speaker_close(speaker_context dev)
{
    assert(dev != NULL);

    if (dev->gpio)
        mraa_gpio_close(dev->gpio);

    free(dev);
}

void speaker_play_all(const speaker_context dev)
{
    assert(dev != NULL);

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
