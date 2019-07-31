/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#include <upm_utilities.h>
#include <buzzer.h>


int
main(int argc, char **argv)
{
//! [Interesting]
    int chord[] = { BUZZER_DO, BUZZER_RE, BUZZER_MI,
                    BUZZER_FA, BUZZER_SOL, BUZZER_LA,
                    BUZZER_SI };

    // create Buzzer context, using PWM pin 5
    buzzer_context sound = buzzer_init(5);

    if (!sound)
    {
        printf("buzzer_init() failed\n");
        return 1;
    }

    printf("Playing...\n");

    // play each sound (DO, RE, MI, etc...) for .5 seconds, pausing
    // for 0.1 seconds between notes
    for (int chord_ind = 0; chord_ind < 7; chord_ind++)
    {
        buzzer_play_sound(sound, chord[chord_ind], 500000);
        printf("%d\n", chord[chord_ind]);
        upm_delay_ms(100);
    }

    printf("Exiting...\n");

    buzzer_close(sound);

//! [Interesting]

    return 0;
}
