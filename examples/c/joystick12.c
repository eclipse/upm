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

#include "joystick12.h"
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

    // Instantiate a joystick12 sensor
    //    X = Analog pin A0
    //    Y = Analog pin A1
    joystick12_context sensor = joystick12_init(0, 1);

    if (!sensor)
    {
        printf("joystick12_init() failed.\n");
        return -1;
    }

    printf("Press any key to zero joystick...\n");
    getchar();

    // Zero x and y axis
    joystick12_zero(sensor);

    printf("Hold the X axis to the far neg or pos value, then press a key...\n");
    getchar();
    joystick12_calibrate_x(sensor);
    printf("Hold the Y axis to the far neg or pos value, then press a key...\n");
    getchar();
    joystick12_calibrate_y(sensor);

    // Every half a second, sample the sensor output
    float x, y;
    while (shouldRun)
    {
        joystick12_get_value_x(sensor, &x);
        joystick12_get_value_y(sensor, &y);

        printf("X: %5.02f Y: %5.02f\n", x, y);
        upm_delay_ms(500);
    }

    //! [Interesting]

    printf("Exiting\n");

    joystick12_close(sensor);

    return 0;
}
