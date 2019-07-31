/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This driver was rewritten based on the original driver written by:
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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

#include "bmpx8x.hpp"
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
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);
    //! [Interesting]

    // Instantiate a BMPX8X sensor on I2C using defaults.
    upm::BMPX8X sensor;

    // Print the pressure, altitude, sea level, and
    // temperature values every 0.5 seconds
    while (shouldRun) {
        sensor.update();

        cout << "Pressure: " << sensor.getPressure()
             << " Pa, Temperature: " << sensor.getTemperature()
             << " C, Altitude: " << sensor.getAltitude()
             << " m, Sea level: " << sensor.getSealevelPressure() << " Pa" << endl;

        upm_delay_us(500000);
    }

    cout << "Exiting..." << endl;

    //! [Interesting]
    return 0;
}
