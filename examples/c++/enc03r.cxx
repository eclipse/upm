/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>

#include "enc03r.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

// analog voltage, usually 3.3 or 5.0
#define CALIBRATION_SAMPLES 1000

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

    // Instantiate a ENC03R on analog pin A0
    upm::ENC03R gyro(0);

    // The first thing we need to do is calibrate the sensor.
    cout << "Please place the sensor in a stable location, and do not" << endl;
    cout << "move it while calibration takes place." << endl;
    cout << "This may take a couple of minutes." << endl;

    gyro.calibrate(CALIBRATION_SAMPLES);
    cout << "Calibration complete.  Reference value: " << gyro.calibrationValue() << endl;

    // Read the input and print both the raw value and the angular velocity,
    // waiting 0.1 seconds between readings
    while (shouldRun) {
        gyro.update();

        cout << "Angular velocity: " << gyro.angularVelocity() << " deg/s" << endl;

        upm_delay_us(100000);
    }
    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}
