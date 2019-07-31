/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>

#include "bno055.hpp"
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

    // Instantiate an BNO055 using default parameters (bus 0, addr
    // 0x28).  The default running mode is NDOF absolute orientation
    // mode.
    upm::BNO055 sensor;

    // First we need to calibrate....
    cout << "First we need to calibrate.  4 numbers will be output every" << endl;
    cout << "second for each sensor.  0 means uncalibrated, and 3 means" << endl;
    cout << "fully calibrated." << endl;
    cout << "See the UPM documentation on this sensor for instructions on" << endl;
    cout << "what actions are required to calibrate." << endl;
    cout << endl;

    // do the calibration...
    while (shouldRun && !sensor.isFullyCalibrated()) {
        int mag, acc, gyr, sys;
        sensor.getCalibrationStatus(&mag, &acc, &gyr, &sys);

        cout << "Magnetometer: " << mag << " Accelerometer: " << acc << " Gyroscope: " << gyr
             << " System: " << sys << endl;

        upm_delay(1);
    }

    cout << endl;
    cout << "Calibration complete." << endl;
    cout << endl;

    // now output various fusion data every 250 milliseconds
    while (shouldRun) {
        float w, x, y, z;

        sensor.update();

        sensor.getEulerAngles(&x, &y, &z);
        cout << "Euler: Heading: " << x << " Roll: " << y << " Pitch: " << z << " degrees" << endl;

        sensor.getQuaternions(&w, &x, &y, &z);
        cout << "Quaternion: W: " << w << " X: " << x << " Y: " << y << " Z: " << z << endl;

        sensor.getLinearAcceleration(&x, &y, &z);
        cout << "Linear Acceleration: X: " << x << " Y: " << y << " Z: " << z << " m/s^2" << endl;

        sensor.getGravityVectors(&x, &y, &z);
        cout << "Gravity Vector: X: " << x << " Y: " << y << " Z: " << z << " m/s^2" << endl;

        cout << endl;
        upm_delay_us(250000);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
