/*
 * Author: Alex Tereschenko <alext.mkrs@gmail.com>
 * Copyright (c) 2018 Alex Tereschenko.
 *
 * Based on LIS2DS12 example by
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

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "lis3dh.h"
#include "upm_utilities.h"

bool shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);
//! [Interesting]

#if defined(CONFIG_BOARD_ARDUINO_101_SSS)
    // ARDUINO_101_SSS (ARC core) must use I2C
    // Instantiate a LIS3DH instance using default i2c bus and address
    lis3dh_context sensor = lis3dh_init(LIS3DH_DEFAULT_I2C_BUS, LIS3DH_DEFAULT_I2C_ADDR, -1);
#elif defined(CONFIG_BOARD_ARDUINO_101)
    // ARDUINO_101 (Quark core) must use SPI
    // Instantiate a LIS3DH instance using default SPI bus and pin 10 as CS
    lis3dh_context sensor = lis3dh_init(LIS3DH_DEFAULT_SPI_BUS, -1, 10);
#else
    // everything else use I2C by default
    // Instantiate a LIS3DH instance using default i2c bus and address
    lis3dh_context sensor = lis3dh_init(LIS3DH_DEFAULT_I2C_BUS, LIS3DH_DEFAULT_I2C_ADDR, -1);
#endif

    if (!sensor) {
        printf("lis3dh_init() failed.\n");
        return 1;
    }

    // now output data every 250 milliseconds
    while (shouldRun) {
        float x, y, z;

        if (lis3dh_update(sensor)) {
            printf("lis3dh_update() failed\n");
            lis3dh_close(sensor);
            return 1;
        }

        lis3dh_get_accelerometer(sensor, &x, &y, &z);
        printf("Acceleration x: %f y: %f z: %f g\n", x, y, z);

        printf("Compensation Temperature: %f C\n\n", lis3dh_get_temperature(sensor));

        upm_delay_ms(250);
    }

    printf("Exiting...\n");

    lis3dh_close(sensor);

    //! [Interesting]

    return 0;
}
