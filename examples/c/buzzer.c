/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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
