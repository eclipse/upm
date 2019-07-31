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

#include "sht1x.h"
#include "upm_utilities.h"

bool shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]

    // Instantiate a SHT1X sensor using D2 as the clock, and D3 as the
    // data pin.
    sht1x_context sensor = sht1x_init(2, 3);

    if (!sensor)
    {
        printf("sht1x_init() failed.\n");
        return 1;
    }

    // Every 2 seconds, update and print values
    while (shouldRun)
    {
        if (sht1x_update(sensor))
        {
            printf("sht1x_update() failed, exiting.\n");
            break;
        }

        printf("Temperature: %f C\n", sht1x_get_temperature(sensor));
        printf("Humidity:    %f RH\n", sht1x_get_humidity(sensor));
        printf("\n");

        upm_delay(2);
    }

    //! [Interesting]

    printf("Exiting\n");

    sht1x_close(sensor);

    return 0;
}
