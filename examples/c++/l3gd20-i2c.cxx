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

#include <iomanip>
#include <iostream>
#include <math.h>
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

float
rad2deg(float x)
{
    return x * (180.0 / M_PI);
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);
    //! [Interesting]

    // Instantiate an L3GD20 using default parameters
    upm::L3GD20 sensor(L3GD20_DEFAULT_I2C_BUS, L3GD20_DEFAULT_I2C_ADDR);

    // set some parameters (these are already the defaults, but are
    // provided here as an example)

    // 250 deg/s sensitivity
    sensor.setRange(sensor.FS_250);

    // Set ODR to 95Hz, 25Hz cut-off
    sensor.setODR(sensor.ODR_CUTOFF_95_25);

    // If you already have calibration data, you can specify it here
    //  sensor.loadCalibratedData(-0.0296269637, -0.0080939643, -0.0077121737);

    // now output data every 100 milliseconds
    while (shouldRun) {
        float x, y, z;

        sensor.update();

        cout << "Calibrated: " << sensor.getCalibratedStatus() << endl;

        // output is in radians/s
        sensor.getGyroscope(&x, &y, &z);
        cout << fixed << setprecision(1) << "Gyroscope x: " << x << " y: " << y << " z: " << z
             << " radians" << endl;

        // same data converted to degrees/s
        cout << "Gyroscope x: " << rad2deg(x) << " y: " << rad2deg(y) << " z: " << rad2deg(z)
             << " degrees" << endl;

        // we show both C and F for temperature
        cout << "Compensation Temperature: " << sensor.getTemperature(false) << " C / "
             << sensor.getTemperature(true) << " F" << endl;

        cout << endl;

        upm_delay_us(100000);
    }

    // dump the calibration values if we managed to calibrate
    if (sensor.getCalibratedStatus()) {
        float calX, calY, calZ;
        sensor.getCalibratedData(&calX, &calY, &calZ);

        cout << setprecision(10) << "Calibration values x: " << calX << " y: " << calY
             << " z: " << calZ << endl;
    }

    cout << "Exiting..." << endl;

    //! [Interesting]
    return 0;
}
