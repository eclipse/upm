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

#include "ds18b20.h"
#include "upm_utilities.h"

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main(int argc, char **argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]

    printf("Initializing...\n");

    // Instantiate an DS18B20 instance using the uart 0
    ds18b20_context sensor = ds18b20_init(0);

    if (!sensor)
    {
        printf("ds18b20_init() failed.\n");
        return(1);
    }

    printf("Found %d device(s)\n\n", ds18b20_devices_found(sensor));

    // update and print available values every 2 seconds
    while (shouldRun)
    {
        // update our values for all sensors
        ds18b20_update(sensor, -1);

        for (unsigned int i=0; i<ds18b20_devices_found(sensor); i++)
        {
            printf("Device %02d: Temperature: %f C\n",
                    i, ds18b20_get_temperature(sensor, i));
        }
        printf("\n");

        upm_delay(2);
    }

    printf("Exiting...\n");

    ds18b20_close(sensor);
    //! [Interesting]

    return 0;
}
