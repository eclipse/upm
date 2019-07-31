/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
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

#include "upm_utilities.h"
#include "lsm303d.h"

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

    // Instantiate a LSM303D instance using default i2c bus and addresses
    lsm303d_context sensor = lsm303d_init(LSM303D_DEFAULT_I2C_BUS,
                                          LSM303D_DEFAULT_I2C_ADDR);

    if (!sensor)
    {
        printf("lsm303d_init() failed.\n");
        return 1;
    }

    // now output data every 250 milliseconds
    while (shouldRun)
    {
        float x, y, z;

        if (lsm303d_update(sensor))
        {
            printf("lsm303d_update() failed\n");
            return 1;
        }

        lsm303d_get_accelerometer(sensor, &x, &y, &z);
        printf("Accelerometer x: %f y: %f z: %f g\n",
               x, y, z);

        lsm303d_get_magnetometer(sensor, &x, &y, &z);
        printf("Magnetometer x: %f y: %f z: %f uT\n",
               x, y, z);

        printf("Temperature: %f C\n\n", lsm303d_get_temperature(sensor));

        upm_delay_ms(250);
    }

    printf("Exiting...\n");

    lsm303d_close(sensor);

//! [Interesting]

    return 0;
}
