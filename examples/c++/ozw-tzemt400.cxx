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
#include <string>

#include "tzemt400.hpp"
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

    string defaultDev = "/dev/ttyACM0";

    // if an argument was specified, use it as the device instead
    if (argc > 1)
        defaultDev = string(argv[1]);

    // Instantiate a TZEMT400 instance, on device node 13.  You will
    // almost certainly need to change this to reflect your own network.
    // Use the ozwdump example to see what nodes are available.
    upm::TZEMT400 sensor(13);

    // The first thing to do is create options, then lock them when done.
    sensor.optionsCreate();
    sensor.optionsLock();

    // Next, initialize it.
    cout << "Initializing, this may take awhile depending on your ZWave network" << endl;

    sensor.init(defaultDev);
    cout << "Initialization complete" << endl;

    cout << "Querying data..." << endl;
    while (shouldRun) {
        sensor.update();

        // we show both C and F for temperatures
        cout << "Temperature: " << sensor.getTemperature() << " C / " << sensor.getTemperature(true)
             << " F" << endl;

        cout << "Mode: " << sensor.getMode() << endl;

        cout << "Operating State: " << sensor.getOperatingState() << endl;

        cout << "Heating Point: " << sensor.getHeatingPointTemperature() << " C / "
             << sensor.getHeatingPointTemperature(true) << " F" << endl;

        cout << "Cooling Point: " << sensor.getCoolingPointTemperature() << " C / "
             << sensor.getCoolingPointTemperature(true) << " F" << endl;

        cout << "Fan Mode: " << sensor.getFanMode() << endl;

        cout << "Fan State: " << sensor.getFanState() << endl;

        cout << endl;

        upm_delay(5);
    }

    cout << "Exiting..." << endl;

    //! [Interesting]
    return 0;
}
