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

#include "bmi160.h"
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

    // Instantiate a BMI160 sensor on I2C bus 0, address 0x69, and
    // disable the magnetometer (since it is an optional component
    // that may not be present).  Since we are using I2C, pass a valid
    // I2C address, and supply -1 as the cs_pin.
    bmi160_context sensor = bmi160_init(0, 0x69, -1, false);

    // For SPI, you would use something like this, using gpio 10 as
    // the cs pin.  Passing -1 as the address indicates SPI operation.
    //
//    bmi160_context sensor = bmi160_init(0, -1, 10, false);

    if (!sensor)
    {
        printf("bmi160_init() failed.\n");
        return(1);
    }

    // Every half second, update and print values

    while (shouldRun)
    {
        bmi160_update(sensor);

        float x, y, z;

        bmi160_get_accelerometer(sensor, &x, &y, &z);
        printf("Acceleration x = %f y = %f z = %f\n",
               x, y, z);

        bmi160_get_gyroscope(sensor, &x, &y, &z);
        printf("Gyroscope    x = %f y = %f z = %f\n",
               x, y, z);

        //bmi160_get_magnetometer(sensor, &x, &y, &z);
        //printf("Magnetometer x = %f y = %f z = %f\n",
        //       x, y, z);

        printf("\n");

        upm_delay_ms(500);
    }

    //! [Interesting]

    printf("Exiting...\n");

    bmi160_close(sensor);

    return 0;
}
