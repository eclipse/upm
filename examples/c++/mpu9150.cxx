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

#include "mpu9150.hpp"
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

    upm::MPU9150 sensor;

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
