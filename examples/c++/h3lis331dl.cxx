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

#include "h3lis331dl.hpp"
#include "upm_utilities.h"

using namespace std;
using namespace upm;

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
    // Instantiate an H3LIS331DL on I2C bus 0

    upm::H3LIS331DL accel(H3LIS331DL_I2C_BUS, H3LIS331DL_DEFAULT_I2C_ADDR);

    // Initialize the device with default values
    accel.init();

    while (shouldRun) {
        int x, y, z;
        float ax, ay, az;

        accel.update();

        accel.getRawXYZ(&x, &y, &z);
        accel.getAcceleration(&ax, &ay, &az);

        cout << "Raw: X = " << x << " Y = " << y << " Z = " << z << endl;

        cout << "Acceleration: AX = " << ax << " AY = " << ay << " AZ = " << az << endl;

        cout << endl;

        upm_delay_us(500000);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
