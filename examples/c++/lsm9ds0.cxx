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

#include <iostream>
#include <signal.h>

#include "lsm9ds0.hpp"
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

    // Instantiate an LSM9DS0 using default parameters (bus 1, gyro addr 6b,
    // xm addr 1d)
    upm::LSM9DS0 sensor;

    sensor.init();

    while (shouldRun) {
        sensor.update();

        float x, y, z;

        sensor.getAccelerometer(&x, &y, &z);
        cout << "Accelerometer: ";
        cout << "AX: " << x << " AY: " << y << " AZ: " << z << endl;

        sensor.getGyroscope(&x, &y, &z);
        cout << "Gryoscope:     ";
        cout << "GX: " << x << " GY: " << y << " GZ: " << z << endl;

        sensor.getMagnetometer(&x, &y, &z);
        cout << "Magnetometer:  ";
        cout << "MX = " << x << " MY = " << y << " MZ = " << z << endl;

        cout << "Temperature:   " << sensor.getTemperature() << endl;
        cout << endl;

        upm_delay_us(500000);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
