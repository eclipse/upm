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

#include "yg1006.h"
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
    // Instantiate a YG1006 sensor on digital pin D2
    yg1006_context sensor = yg1006_init(2);

    if (!sensor)
    {
        printf("yg1006_init() failed.\n");
        return(1);
    }

    while (shouldRun)
    {
        if (yg1006_flame_detected(sensor))
            printf("Flame detected.\n");
        else
            printf("No flame detected.\n");

        upm_delay(1);
    }
//! [Interesting]

    printf("Exiting...\n");

    yg1006_close(sensor);

    return 0;
}
