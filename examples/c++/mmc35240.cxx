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
#include <iostream>
#include <math.h>
#include <signal.h>
#include <string>

#include "mmc35240.hpp"
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
    upm::MMC35240* magnetometer = static_cast<upm::MMC35240*>(args);
    float x, y, z;
    double azimuth;
    int level;
    magnetometer->extract3Axis(data, &x, &y, &z);
    /* calibrated level
   * UNRELIABLE = 0
   * ACCURACY_LOW = 1
   * ACCURACY_MEDIUM = 2
   * ACCURACY_HIGH = >=3
   */
    level = magnetometer->getCalibratedLevel();

    if ((x == 0) && (y == 0)) {
        cout << "Point (0, 0) is invalid!\n" << endl;
        return;
    }

    if (x == 0) {
        if (y > 0)
            azimuth = 0;
        else
            azimuth = 180;
    } else if (y == 0) {
        if (x > 0)
            azimuth = 90;
        else
            azimuth = 270;
    } else {
        if (x > 0)
            azimuth = 90 - atan(y / x) * 180 / M_PI;
        else
            azimuth = 270 - atan(y / x) * 180 / M_PI;
    }

    cout << "[Calibrated Level:" << level << ']' << "[Azimuth:" << (int) (360 - azimuth) << ']'
         << '\t' << (int) x << '\t' << (int) y << '\t' << (int) z << "[uT]" << endl;
}

int
main()
{
    signal(SIGINT, sig_handler);
    //! [Interesting]
    // Instantiate a MMC35240 Magnetic Sensor on iio device 5. This configuration
    // is a reference and
    // should be changed per platform/board type.
    // Kernel driver does not allow changing the value of scale at run-time,
    // default scale is
    // 0.001000
    upm::MMC35240 magnetometer(5);
    magnetometer.setScale(0.001000);
    // Available sampling frequency are 1.5, 13, 25, 50
    magnetometer.setSamplingFrequency(25.000000);
    magnetometer.enable3AxisChannel();
    magnetometer.installISR(data_callback, &magnetometer);
    magnetometer.enableBuffer(16);

    while (shouldRun) {
        upm_delay(1);
    }
    magnetometer.disableBuffer();

    //! [Interesting]
    cout << "Exiting" << endl;

    return 0;
}
