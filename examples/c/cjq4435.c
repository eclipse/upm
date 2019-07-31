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
#include <signal.h>

#include <upm_utilities.h>
#include "cjq4435.h"

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
    // Instantiate a CJQ4435 MOSFET on a PWM capable digital pin D3
    cjq4435_context mosfet = cjq4435_init(3);

    if (!mosfet)
    {
        printf("cjq4435_init() failed.\n");
        return 1;
    }

    cjq4435_set_period_ms(mosfet, 10);
    cjq4435_enable(mosfet, true);

    while (shouldRun)
    {
        // start with a duty cycle of 0.0 (off) and increment to 1.0 (on)
        for (float i=0.0; i <= 1.0; i+=0.1)
        {
            cjq4435_set_duty_cycle(mosfet, i);
            upm_delay_ms(100);
        }
        upm_delay(1);

        // Now bring it back down
        // start with a duty cycle of 1.0 (on) and decrement to 0.0 (off)
        for (float i=1.0; i >= 0.0; i-=0.1)
        {
            cjq4435_set_duty_cycle(mosfet, i);
            upm_delay_ms(100);
        }
        upm_delay(1);
    }

    printf("Exiting...\n");

    cjq4435_close(mosfet);
//! [Interesting]
    return 0;
}
