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

#include "kxcjk1013.hpp"
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
    upm::KXCJK1013* accelerometer = static_cast<upm::KXCJK1013*>(args);

    accelerometer->extract3Axis(data, &x, &y, &z);
    cout << fixed << setprecision(1);
    cout << x << '\t' << y << '\t' << z << "[m/s^2]" << endl;
}

int
main()
{
    signal(SIGINT, sig_handler);
    //! [Interesting]
    // Instantiate a KXCJK1013 Accelerometer Sensor on iio device 0
    upm::KXCJK1013 accelerometer(0);
    // Available scales are 0.009582(2g), 0.019163(4g), and 0.038326(8g)
    accelerometer.setScale(0.019163);
    // Available sampling frequency are 0.781000, 1.563000, 3.125000, 6.250000,
    // 12.500000, 25, 50,
    // 100, 200, 400, 800, and 1600
    accelerometer.setSamplingFrequency(25.0);
    accelerometer.enable3AxisChannel();
    accelerometer.installISR(data_callback, &accelerometer);
    accelerometer.enableBuffer(16);

    while (shouldRun) {
        upm_delay(1);
    }
    accelerometer.disableBuffer();

    //! [Interesting]
    cout << "Exiting" << endl;

    return 0;
}
