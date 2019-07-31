/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include "rotaryencoder.h"
#include "upm_utilities.h"

int shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}


int main()
{
    signal(SIGINT, sig_handler);

//! [Interesting]
    // Instantiate a Grove Rotary Encoder, using signal pins D2 and D3
    rotaryencoder_context sensor = rotaryencoder_init(2, 3);

    if (!sensor)
    {
        printf("rotaryencoder_init() failed.\n");
        return 1;
    }

    while (shouldRun)
    {
        printf("Position: %d\n", rotaryencoder_get_position(sensor));

        upm_delay_ms(100);
    }


    printf("Exiting...\n");

    rotaryencoder_close(sensor);

//! [Interesting]
    return 0;
}
