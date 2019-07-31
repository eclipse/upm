/*
 * Author: Lay, Kuan Loon <kuan.loon.lay@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iomanip>
#include <iostream>
#include <signal.h>

#include "l3gd20.hpp"
#include "upm_utilities.h"

using namespace std;

int shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

void
data_callback(char* data, void* args)
{
    float x, y, z;
    upm::L3GD20* gyroscope = static_cast<upm::L3GD20*>(args);

    if (gyroscope->extract3Axis(data, &x, &y, &z)) {
        cout << fixed << setprecision(1);
        cout << x << '\t' << y << '\t' << z << "[rad/sec]" << endl;
    }
}

int
main()
{
    signal(SIGINT, sig_handler);
    //! [Interesting]
    upm::L3GD20 gyroscope(3);
    // Instantiate a L3GD20 Gyroscope Sensor on iio device 3
    // Available scales are 0.000153(250dps), 0.000305(500dps), and
    // 0.001222(2000dps)
    gyroscope.setScale(0.001222);
    // Available sampling frequency are 95, 190, 380, and 760
    gyroscope.setSamplingFrequency(95.0);
    gyroscope.enable3AxisChannel();
    gyroscope.installISR(data_callback, &gyroscope);
    gyroscope.enableBuffer(16);

    while (shouldRun) {
        upm_delay(1);
    }
    gyroscope.disableBuffer();

    //! [Interesting]
    cout << "Exiting" << endl;

    return 0;
}
