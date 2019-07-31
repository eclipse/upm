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

#include <ppd42ns.h>

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
    // Instantiate a dust sensor on GPIO pin D8
    ppd42ns_context dust = ppd42ns_init(8);

    ppd42ns_dust_data data;
    printf("This program will give readings every 30 seconds until "
           "you stop it\n");

    while (shouldRun)
    {
        data = ppd42ns_get_data(dust);
        printf("Low pulse occupancy: %d\n", data.lowPulseOccupancy);
        printf("Ratio: %f\n", data.ratio);
        printf("Concentration: %f\n\n", data.concentration);
    }

    printf("Exiting...\n");

    ppd42ns_close(dust);

//! [Interesting]
    return 0;
}
