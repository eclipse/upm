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

#include <upm_utilities.h>
#include <guvas12d.h>

bool shouldRun = true;

// analog voltage, usually 3.3 or 5.0
#define GUVAS12D_AREF   5.0

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int main()
{
    signal(SIGINT, sig_handler);

//! [Interesting]
    // This was tested with the Grove UV Sensor module.
    // It has a sensing range from between 240-370nm.  It's strongest
    // response is around 320-360nm.

    // Instantiate a GUVAS12D on analog pin A0
    guvas12d_context uv = guvas12d_init(0, GUVAS12D_AREF);

    if (!uv)
    {
        printf("guvas12d_init() failed\n");
        return 1;
    }

    // The higher the voltage the more intense the UV radiation.
    while (shouldRun)
    {
        float volts = 0;
        float intensity = 0;

        if (guvas12d_get_volts(uv, &volts))
        {
            printf("guvas12d_get_volts() failed\n");
            return 1;
        }

        if (guvas12d_get_intensity(uv, &intensity))
        {
            printf("guvas12d_get_intensity() failed\n");
            return 1;
        }

        printf("Volts: %f, Intensity %f mW/m^2\n", volts, intensity);

        upm_delay(1);
    }

    printf("Exiting\n");

    guvas12d_close(uv);
//! [Interesting]
    return 0;
}
