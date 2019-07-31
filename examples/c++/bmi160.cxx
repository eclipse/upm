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

#include "bmi160.hpp"
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

    // Instantiate a BMI160 instance using default i2c bus and address
    upm::BMI160 sensor;

    while (shouldRun) {
        // update our values from the sensor
        sensor.update();

        float dataX, dataY, dataZ;

        sensor.getAccelerometer(&dataX, &dataY, &dataZ);
        cout << "Accelerometer: ";
        cout << "AX: " << dataX << " AY: " << dataY << " AZ: " << dataZ << endl;

        sensor.getGyroscope(&dataX, &dataY, &dataZ);
        cout << "Gryoscope:     ";
        cout << "GX: " << dataX << " GY: " << dataY << " GZ: " << dataZ << endl;

        sensor.getMagnetometer(&dataX, &dataY, &dataZ);
        cout << "Magnetometer:  ";
        cout << "MX: " << dataX << " MY: " << dataY << " MZ: " << dataZ << endl;

        cout << endl;

        upm_delay_us(500000);
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
