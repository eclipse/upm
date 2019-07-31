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

    // Instantiate a Speaker on digital pin D2
    speaker_context speaker = speaker_init(2);

    if (!speaker)
    {
        printf("speaker_init() failed\n");
        return 1;
    }

    // Play all 7 of the lowest notes
    speaker_play_all(speaker);

    // Play a medium C-sharp
    speaker_play_sound(speaker, 'c', true, "med");

    printf("Exiting\n");

    speaker_close(speaker);

//! [Interesting]

    return 0;
}
