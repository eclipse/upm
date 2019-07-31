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
#include "linefinder.h"

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
    // Instantiate a  Line Finder sensor on digital pin D2
    linefinder_context sensor = linefinder_init(2);

    if (!sensor)
    {
        printf("linefinder_init() failed\n");
        return 1;
    }

    // check every second for the presence of white or black detection
    while (shouldRun)
    {
        if (linefinder_white_detected(sensor))
            printf("White detected.\n");
        else
            printf("Black detected.\n");

        upm_delay(1);
    }

    printf("Exiting...\n");

    linefinder_close(sensor);

//! [Interesting]
    return 0;
}
