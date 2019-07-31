/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>

#include "bmp280.hpp"
#include "upm_utilities.h"

using namespace std;
using namespace upm;

bool shouldRun = true;

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

    // Instantiate a BMP280 instance using default i2c bus and address
    upm::BMP280 sensor;

    // For SPI, bus 0, you would pass -1 as the address, and a valid pin for CS:
    // BMP280(0, -1, 10);

    while (shouldRun) {
        // update our values from the sensor
        sensor.update();

        // we show both C and F for temperature
        cout << "Compensation Temperature: " << sensor.getTemperature() << " C / "
             << sensor.getTemperature(true) << " F" << endl;
        cout << "Pressure: " << sensor.getPressure() << " Pa" << endl;
        cout << "Computed Altitude: " << sensor.getAltitude() << " m" << endl;

        cout << endl;

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
