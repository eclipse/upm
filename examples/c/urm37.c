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

#include "urm37.h"
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

    // Instantiate a URM37 sensor on analog pin A0, reset pin on D2,
    // trigger pin on D3 with an analog reference voltage of 5.0
    urm37_context sensor = urm37_init(0, 2, 3, 5.0, 0, true);

    if (!sensor)
    {
        printf("urm37_init() failed.\n");
        return(1);
    }

    // Every half a second, sample the URM37 and output the measured
    // distance in cm.

    while (shouldRun)
    {
        float distance;

        urm37_get_distance(sensor, &distance, 0);
        printf("Detected distance (cm): %f\n", distance);
        upm_delay_ms(500);
    }

    //! [Interesting]

    printf("Exiting\n");

    urm37_close(sensor);

    return 0;
}
