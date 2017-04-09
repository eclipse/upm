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
