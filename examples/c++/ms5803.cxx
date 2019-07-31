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

#include "ms5803.hpp"
#include "upm_utilities.h"

using namespace std;
using namespace upm;

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

    // Instantiate a MS5803 instance using i2c bus 0 and default address
    upm::MS5803 sensor(0);

    // For SPI, bus 0, you would pass -1 as the address, and a valid
    // pin for CS (or -1 if you are using a hw pin you have no control
    // over, like edison):
    // MS5803(0, -1, 9);

    while (shouldRun) {
        // update our values from the sensor
        sensor.update();

        cout << "Temperature: " << sensor.getTemperature() << " C, "
             << "Pressure: " << sensor.getPressure() << " mbar" << endl;

        upm_delay(1);
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
