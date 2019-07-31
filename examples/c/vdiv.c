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

#include "vdiv.h"
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

    // Instantiate a sensor on analog pin A0
    vdiv_context sensor = vdiv_init(0, 5);

    if (!sensor)
    {
        printf("vdiv_init() failed.\n");
        return(1);
    }

    // Every half a second, sample the sensor output
    while (shouldRun)
    {
        float raw_volts = 0.0, computed_volts = 0.0;

        vdiv_get_raw_volts(sensor, &raw_volts);
        vdiv_get_computed_volts(sensor, &computed_volts);

        printf("Divide SW: %d ADC voltage: %0.03f Sensor voltage: %0.03f\n",
                vdiv_get_divsw(sensor), raw_volts, computed_volts);

        upm_delay_ms(500);
    }

    //! [Interesting]

    printf("Exiting\n");

    vdiv_close(sensor);

    return 0;
}
