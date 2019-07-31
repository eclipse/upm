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
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "upm_utilities.h"
#include "mma7660.h"

int shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int main(int argc, char **argv)
{
    signal(SIGINT, sig_handler);

//! [Interesting]
    // Instantiate an MMA7660 on I2C bus 0

    mma7660_context accel = mma7660_init(MMA7660_DEFAULT_I2C_BUS,
                                         MMA7660_DEFAULT_I2C_ADDR);

    if (!accel)
    {
        printf("mma7660_init() failed\n");
        return 1;
    }

    // place device in standby mode so we can write registers
    mma7660_set_mode_standby(accel);

    // enable 64 samples per second
    mma7660_set_sample_rate(accel, MMA7660_AUTOSLEEP_64);

    // place device into active mode
    mma7660_set_mode_active(accel);

    while (shouldRun)
    {
        float ax, ay, az;

        mma7660_get_acceleration(accel, &ax, &ay, &az);
        printf("Acceleration: x = %f y = %f z = %f\n\n",
               ax, ay, az);

        upm_delay_ms(500);
    }

    printf("Exiting...\n");

    mma7660_close(accel);

//! [Interesting]
    return 0;
}
