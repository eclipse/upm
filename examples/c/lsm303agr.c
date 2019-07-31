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
#include "lsm303agr.h"

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

    // Instantiate a LSM303AGR instance using default i2c bus and addresses
    lsm303agr_context sensor = lsm303agr_init(LSM303AGR_DEFAULT_I2C_BUS,
                                              LSM303AGR_DEFAULT_ACC_ADDR,
                                              LSM303AGR_DEFAULT_MAG_ADDR);

    if (!sensor)
    {
        printf("lsm303agr_init() failed.\n");
        return 1;
    }

    // now output data every 250 milliseconds
    while (shouldRun)
    {
        float x, y, z;

        if (lsm303agr_update(sensor))
        {
            printf("lsm303agr_update() failed\n");
            return 1;
        }

        lsm303agr_get_accelerometer(sensor, &x, &y, &z);
        printf("Accelerometer x: %f y: %f z: %f g\n",
               x, y, z);

        lsm303agr_get_magnetometer(sensor, &x, &y, &z);
        printf("Magnetometer x: %f y: %f z: %f uT\n",
               x, y, z);

        printf("Temperature: %f C\n\n", lsm303agr_get_temperature(sensor));

        upm_delay_ms(250);
    }

    printf("Exiting...\n");

    lsm303agr_close(sensor);

//! [Interesting]

    return 0;
}
