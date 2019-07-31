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

#include "mma7361.h"
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

    // Instantiate a MMA7361 sensor on analog pins A0 (X), A1 (Y) A2
    // (Z), selftest pin on D2, sleep pin on D3 nd an analog reference
    // value of 5.0.  The freefall pin and the range pin are unused
    // (-1).
    mma7361_context sensor = mma7361_init(0, 1, 2, 2, 3, -1, -1, 5.0);

    if (!sensor)
    {
        printf("mma7361_init() failed.\n");
        return(1);
    }

    // 1.5g (true = 6g)
    mma7361_set_range(sensor, false);

    // Every 10th of a second, update and print values

    while (shouldRun)
    {
        mma7361_update(sensor);

        float x, y, z;

        mma7361_get_acceleration(sensor, &x, &y, &z);
        printf("Acceleration x = %f y = %f z = %f\n",
                x, y, z);

        mma7361_get_volts(sensor, &x, &y, &z);
        printf("Volts x = %f y = %f z = %f\n\n",
                x, y, z);

        upm_delay_ms(500);
    }

    //! [Interesting]

    printf("Exiting...\n");

    mma7361_close(sensor);

    return 0;
}
