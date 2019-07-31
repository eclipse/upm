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

#include "dfrph.h"
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

    // Instantiate a dfrph sensor on analog pin A0
    dfrph_context sensor = dfrph_init(0);

    if (!sensor)
    {
        printf("dfrph_init() failed.\n");
        return(1);
    }

    // Every half a second, sample the sensor output
    while (shouldRun)
    {
        float volts = 0.0, pH = 0.0;

        dfrph_get_raw_volts(sensor, &volts);
        dfrph_get_ph(sensor, &pH);

        printf("Detected volts: %0.03f\n", volts);
        printf("pH value: %0.03f\n", pH);

        upm_delay_ms(500);
    }

    //! [Interesting]

    printf("Exiting\n");

    dfrph_close(sensor);

    return 0;
}
