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
#include <string>

#include "adxl335.hpp"
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
main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // Instantiate an ADXL335 accelerometer on analog pins A0, A1, and A2
    upm::ADXL335 accel(0, 1, 2);

    cout << "Please make sure the sensor is completely still. Sleeping for"
         << " 2 seconds." << endl;
    upm_delay(2);
    cout << "Calibrating..." << endl;

    accel.calibrate();

    while (shouldRun) {
        int x, y, z;
        float aX, aY, aZ;

        accel.values(&x, &y, &z);
        cout << "Raw Values: X: " << x << " Y: " << y << " Z: " << z << endl;

        accel.acceleration(&aX, &aY, &aZ);
        cout << "Acceleration: X: " << aX << "g" << endl;
        cout << "Acceleration: Y: " << aY << "g" << endl;
        cout << "Acceleration: Z: " << aZ << "g" << endl;
        cout << endl;

        upm_delay_us(200000);
    }
    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}
