/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <stdio.h>
#include "speaker.h"

int main ()
{
//! [Interesting]

    // Instantiate a Speaker (PWM) on digital pin D3
    speaker_context speaker = speaker_init_pwm(26);

    if (!speaker)
    {
        printf("speaker_init() failed\n");
        return 1;
    }

    // emit a series of frequencies for 500ms each
    speaker_emit(speaker, 50, 500);
    speaker_emit(speaker, 75, 500);
    speaker_emit(speaker, 125, 500);
    speaker_emit(speaker, 250, 500);
    speaker_emit(speaker, 500, 500);
    speaker_emit(speaker, 1000, 500);
    speaker_emit(speaker, 2000, 500);
    speaker_emit(speaker, 3000, 500);
    speaker_emit(speaker, 5000, 500);
    speaker_emit(speaker, 10000, 500);

    printf("Exiting\n");

    speaker_close(speaker);

//! [Interesting]

    return 0;
}
