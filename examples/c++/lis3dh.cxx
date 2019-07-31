/*
 * Author: Alex Tereschenko <alext.mkrs@gmail.com>
 * Copyright (c) 2018 Alex Tereschenko.
 *
 * Based on LIS2DS12 example by
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>

#include "lis3dh.hpp"
#include "upm_utilities.h"

using namespace std;

int shouldRun = true;

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

    // Instantiate an LIS3DH using default I2C parameters
    upm::LIS3DH sensor;

    // For SPI, bus 0, you would pass -1 as the address, and a valid pin
    // for CS: LIS3DH(0, -1, 10);

    // now output data every 250 milliseconds
    while (shouldRun) {
        float x, y, z;

        sensor.update();

        sensor.getAccelerometer(&x, &y, &z);
        cout << "Accelerometer x: " << x << " y: " << y << " z: " << z << " g" << endl;

        // we show both C and F for temperature
        cout << "Compensation Temperature: " << sensor.getTemperature() << " C / "
             << sensor.getTemperature(true) << " F" << endl;

        cout << endl;

        upm_delay_us(250000);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
