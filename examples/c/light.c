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

#include "light.h"
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

    // Instantiate a light sensor on analog pin A0
    light_context sensor = light_init(0);

    if (!sensor)
    {
        printf("light_init() failed.\n");
        return -1;
    }

    // Set the aref, scale, and offset
    light_set_aref(sensor, 5.0);
    light_set_scale(sensor, 1.0);
    light_set_offset(sensor, -.1);
    printf("aRef: %0.03f scale: %0.03f offset: %0.03f\n\n",
            light_get_aref(sensor),
            light_get_scale(sensor),
            light_get_offset(sensor));

    // Every half a second, sample the sensor output
    while (shouldRun)
    {
        float normalized = 0.0;
        float raw_volts = 0.0;
        float lux = 0.0;

        light_get_normalized(sensor, &normalized);
        light_get_raw_volts(sensor, &raw_volts);
        light_get_lux(sensor, &lux);

        printf("Normalized output: %0.03f, raw light sensor output: %0.03f v "
                "light output: %0.03f lux\n", normalized, raw_volts, lux);

        upm_delay_ms(500);
    }

    //! [Interesting]

    printf("Exiting\n");

    light_close(sensor);

    return 0;
}
