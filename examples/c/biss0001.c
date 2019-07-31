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

#include "biss0001.h"
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
    // Instantiate a BISS0001 sensor on digital pin D2
    biss0001_context sensor = biss0001_init(2);

    if (!sensor)
    {
        printf("biss0001_init() failed.\n");
        return(1);
    }

    while (shouldRun)
    {
        if (biss0001_motion_detected(sensor))
            printf("Motion detected.\n");
        else
            printf("No motion detected.\n");

        upm_delay(1);
    }
//! [Interesting]

    printf("Exiting...\n");

    biss0001_close(sensor);

    return 0;
}
