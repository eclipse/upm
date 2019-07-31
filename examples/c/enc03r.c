/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2017 Intel Corporation.
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
#include <upm_platform.h>

#include "enc03r.h"

bool shouldRun = true;

// The number of samples used for calibration
#if defined(UPM_PLATFORM_ZEPHYR)
# define CALIBRATION_SAMPLES 500
#else
# define CALIBRATION_SAMPLES 1000
#endif

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int main()
{
    signal(SIGINT, sig_handler);

//! [Interesting]

    // Instantiate a ENC03R on analog pin A0
    enc03r_context sensor = enc03r_init(0, 5.0);

    if (!sensor)
    {
        printf("%s: enc03r_init() failed\n", __FUNCTION__);
        return 1;
    }

    // The first thing we need to do is calibrate the sensor.
    printf("Please place the sensor in a stable location, and do not\n");
    printf("move it while calibration takes place.\n");
    printf("This may take a little time to complete.\n");

    enc03r_calibrate(sensor, CALIBRATION_SAMPLES);
    printf("Calibration complete.  Reference value: %f\n\n",
           enc03r_calibration_value(sensor));

    // Read the input and print both the raw value and the angular velocity,
    // waiting 0.1 seconds between readings
    while (shouldRun)
    {
        enc03r_update(sensor);

        printf("Angular velocity: %f deg/s\n",
               enc03r_angular_velocity(sensor));

        upm_delay_ms(100);
    }

    printf("Exiting\n");

    enc03r_close(sensor);
//! [Interesting]
    return 0;
}
