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
#include "lsm6dsl.h"

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

#if defined(CONFIG_BOARD_ARDUINO_101_SSS)
    // ARDUINO_101_SSS (ARC core) must use I2C
    // Instantiate a LSM6DSL instance using default i2c bus and address
    lsm6dsl_context sensor = lsm6dsl_init(LSM6DSL_DEFAULT_I2C_BUS,
                                          LSM6DSL_DEFAULT_I2C_ADDR, -1);
#elif defined(CONFIG_BOARD_ARDUINO_101)
    // ARDUINO_101 (Quark core) must use SPI
    // Instantiate a LSM6DSL instance using default SPI bus and pin 10 as CS
    lsm6dsl_context sensor = lsm6dsl_init(LSM6DSL_DEFAULT_SPI_BUS,
                                          -1, 10);
#else
    // everything else use I2C by default
    // Instantiate a LSM6DSL instance using default i2c bus and address
    lsm6dsl_context sensor = lsm6dsl_init(LSM6DSL_DEFAULT_I2C_BUS,
                                          LSM6DSL_DEFAULT_I2C_ADDR, -1);
#endif

    if (!sensor)
    {
        printf("lsm6dsl_init() failed.\n");
        return 1;
    }

    // now output data every 250 milliseconds
    while (shouldRun)
    {
        float x, y, z;

        if (lsm6dsl_update(sensor))
        {
            printf("lsm6dsl_update() failed\n");
            lsm6dsl_close(sensor);
            return 1;
        }

        lsm6dsl_get_accelerometer(sensor, &x, &y, &z);
        printf("Acceleration x: %f y: %f z: %f g\n",
               x, y, z);

        lsm6dsl_get_gyroscope(sensor, &x, &y, &z);
        printf("Gyroscope x: %f y: %f z: %f dps\n",
               x, y, z);

        printf("Compensation Temperature: %f C\n\n",
               lsm6dsl_get_temperature(sensor));

        upm_delay_ms(250);
    }

    printf("Exiting...\n");

    lsm6dsl_close(sensor);

//! [Interesting]

    return 0;
}
