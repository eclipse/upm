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

#include "adxrs610.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

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

    // Instantiate a ADXRS610 sensor on analog pin A0 (dataout), and
    // analog A1 (temp out) with an analog reference voltage of
    // 5.0
    upm::ADXRS610 sensor(0, 1, 5.0);

    // set a deadband region around the zero point to report 0.0 (optional)
    sensor.setDeadband(0.015);

    // Every tenth of a second, sample the ADXRS610 and output it's
    // corresponding temperature and angular velocity

    while (shouldRun) {
        cout << "Vel (deg/s): " << sensor.getAngularVelocity() << endl;
        cout << "Temp (C): " << sensor.getTemperature() << endl;

        upm_delay_us(100000);
    }

    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}
