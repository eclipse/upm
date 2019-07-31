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

#include <iostream>
#include <signal.h>

#include "lsm303d.hpp"
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

    // Instantiate an LSM303D using default I2C parameters
    upm::LSM303D sensor;

    // now output data every 250 milliseconds
    while (shouldRun) {
        float x, y, z;

        sensor.update();

        sensor.getAccelerometer(&x, &y, &z);
        cout << "Accelerometer x: " << x << " y: " << y << " z: " << z << " g" << endl;

        sensor.getMagnetometer(&x, &y, &z);
        cout << "Magnetometer x: " << x << " y: " << y << " z: " << z << " uT" << endl;

        cout << "Temperature: " << sensor.getTemperature() << " C" << endl;

        cout << endl;

        upm_delay_us(250000);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
