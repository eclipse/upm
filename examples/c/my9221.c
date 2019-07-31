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
#include <my9221.h>

int shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}


int main ()
{
    signal(SIGINT, sig_handler);

//! [Interesting]

    // Instantiate a GroveLEDBar, we use D8 for the data, and D9 for the
    // clock.  We only use a single instance.
    my9221_context leds = my9221_init(8, 9, 1);

    if (!leds)
    {
        printf("my9221_init() failed\n");
        return 1;
    }

    while (shouldRun)
    {
        // count up
        printf("Counting up: ");
        for (int i=0; i<my9221_get_max_leds(leds); i++)
        {
            printf("%d ", i);
            my9221_clear_all(leds);
            my9221_set_led(leds, i, true);
            upm_delay_ms(100);
        }
        printf("\n");
        upm_delay_ms(100);

        // count down
        printf("Counting down: ");
        for (int i=my9221_get_max_leds(leds) - 1; i>=0; i--)
        {
            printf("%d ", i);
            my9221_clear_all(leds);
            my9221_set_led(leds, i, true);
            upm_delay_ms(100);
        }
        printf("\n");
        upm_delay_ms(100);
    }

    printf("Exiting...\n");

    my9221_close(leds);
//! [Interesting]
    return 0;
}
