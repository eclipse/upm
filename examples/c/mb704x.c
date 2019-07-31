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

#include <upm_utilities.h>
#include <mb704x.h>

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

    // Instantiate a MB704X sensor on i2c bus 0, address 112.
    mb704x_context sensor = mb704x_init(0, 112);

    if (!sensor)
    {
        printf("mb704x_init() failed.\n");
        return 1;
    }

    while (shouldRun)
    {
        // this will take about 1 second to complete
        int range = mb704x_get_range(sensor);
        if (range < 0)
        {
            printf("Error getting range.\n");
        }
        else
        {
            printf("Range: %d cm\n", range);
        }

        upm_delay_ms(500);
    }

    printf("Exiting\n");

    mb704x_close(sensor);

//! [Interesting]

    return 0;
}
