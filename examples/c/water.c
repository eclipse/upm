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
#include <signal.h>

#include "water.h"
#include "upm_utilities.h"

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
    // Instantiate a Water sensor on digital pin D2
    water_context sensor = water_init(2);

    if (!sensor)
    {
        printf("water_init() failed.\n");
        return(1);
    }

    while (shouldRun)
    {
        if (water_is_wet(sensor))
            printf("Sensor is wet\n");
        else
            printf("Sensor is dry\n");

        upm_delay(1);
    }
//! [Interesting]

    printf("Exiting...\n");

    water_close(sensor);

    return 0;
}
