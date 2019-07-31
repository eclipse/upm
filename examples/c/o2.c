/*
 * Author: Noel Eck <noel.eck@intel.com>
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

#include "o2.h"
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

    // Instantiate a o2 sensor on analog pin A0
    o2_context sensor = o2_init(0);

    if (!sensor)
    {
        printf("o2_init() failed.\n");
        return -1;
    }

    // Every half a second, sample the sensor output
    while (shouldRun)
    {
        float raw_volts = 0.0;
        float o2_percent = 0.0;

        o2_get_raw_volts(sensor, &raw_volts);
        o2_get_value(sensor, &o2_percent);

        printf("O2 raw volts: %0.03f v, o2: %0.03f %%\n",
                raw_volts, o2_percent);

        upm_delay_ms(500);
    }

    //! [Interesting]

    printf("Exiting\n");

    o2_close(sensor);

    return 0;
}
