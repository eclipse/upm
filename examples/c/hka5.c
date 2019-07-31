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

#include "hka5.h"
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

    // Instantiate a HKA5 sensor on uart 0.  We don't use the set or
    // reset pins, so we pass -1 for them.
    hka5_context sensor = hka5_init(0, -1, -1);

    if (!sensor)
    {
        printf("hka5_init() failed.\n");
        return 1;
    }

    // update once every 2 seconds and output data
    while (shouldRun)
    {
        if (hka5_update(sensor) != UPM_SUCCESS)
        {
            printf("hka5_update() failed, exiting.\n");
            shouldRun = false;
        }

        printf("PM 1  : %d ug/m3\n", hka5_get_pm1(sensor));
        printf("PM 2.5: %d ug/m3\n", hka5_get_pm2_5(sensor));
        printf("PM 10 : %d ug/m3\n", hka5_get_pm10(sensor));
        printf("\n");

        upm_delay(2);
    }

    //! [Interesting]

    printf("Exiting\n");

    hka5_close(sensor);

    return 0;
}
