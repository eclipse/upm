/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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
#include "nunchuck.h"

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
    // Instantiate a nunchuck controller bus 3
    nunchuck_context sensor = nunchuck_init(3);

    if (!sensor)
    {
        printf("%s: nunchuck_init() failed\n", __FUNCTION__);
        return 1;
    }

    while (shouldRun)
    {
        if (nunchuck_update(sensor))
        {
            printf("%s: nunchuck_update() failed\n", __FUNCTION__);
            nunchuck_close(sensor);
            return 1;
        }

        int x, y, z;
        nunchuck_get_stick(sensor, &x, &y);
        printf("stickX: %d stickY: %d\n", x, y);

        nunchuck_get_acceleration(sensor, &x, &y, &z);
        printf("accelX: %d accelY: %d accelZ: %d\n", x, y, z);

        bool bc, bz;
        nunchuck_get_buttons(sensor, &bc, &bz);
        printf("button C: %s\n",
               ((bc) ? "pressed" : "not pressed"));
        printf("button Z: %s\n\n",
               ((bz) ? "pressed" : "not pressed"));

        upm_delay_ms(100);
    }

    nunchuck_close(sensor);

    //! [Interesting]
    return 0;
}
