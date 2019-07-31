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
#include "bmpx8x.h"

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

    // Instantiate a BMPX8X instance using default i2c bus and address
    bmpx8x_context sensor = bmpx8x_init(BMPX8X_DEFAULT_I2C_BUS,
                                        BMPX8X_DEFAULT_I2C_ADDR);

    if (!sensor)
    {
        printf("bmpx8x_init() failed.\n");
        return 1;
    }

    // Print the pressure, altitude, sea level, and
    // temperature values every 0.5 seconds
    while (shouldRun)
    {
        if (bmpx8x_update(sensor))
        {
            printf("bmpx8x_update() failed\n");
            bmpx8x_close(sensor);
            return 1;
        }

        // assume sea level pressure is 101325 Pa.
        float altitude = bmpx8x_get_altitude(sensor, 101325);
        int   sealevel = bmpx8x_get_sealevel_pressure(sensor, altitude);

        printf("Pressure: %d Pa, Temperature: %f C, "
               "Altitude %f m, Sea level %d Pa\n",
               bmpx8x_get_pressure(sensor),
               bmpx8x_get_temperature(sensor),
               altitude,
               sealevel);

        upm_delay_ms(500);
    }

    printf("Exiting...\n");
    bmpx8x_close(sensor);

//! [Interesting]
    return 0;
}
