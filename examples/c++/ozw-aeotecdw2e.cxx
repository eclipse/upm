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

#include "aeotecdw2e.hpp"
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

    // Instantiate an Aeotec Door/Window 2nd Edition sensor instance, on
    // device node 10.  You will almost certainly need to change this to
    // reflect your own network.  Use the ozwdump example to see what
    // nodes are available.
    upm::AeotecDW2E sensor(10);

    // The first thing to do is create options, then lock them when done.
    sensor.optionsCreate();
    sensor.optionsLock();

    // Next, initialize it.
    cout << "Initializing, this may take awhile depending on your ZWave network" << endl;

    sensor.init(defaultDev);
    cout << "Initialization complete" << endl;

    cout << "Querying data..." << endl;
    while (shouldRun) {
        if (sensor.isDeviceAvailable()) {
            cout << "Alarm status: " << sensor.isAlarmTripped() << endl;

            cout << "Tamper Switch status: " << sensor.isTamperTripped() << endl;

            cout << "Battery Level: " << sensor.getBatteryLevel() << "%" << endl;
            cout << endl;
        } else {
            cout << "Device has not yet responded to probe." << endl;

            cout << "Try waking it, or wait until it wakes itself if configured"
                 << " to do so." << endl;
            cout << endl;
        }

        upm_delay(1);
    }

    cout << "Exiting..." << endl;

    //! [Interesting]

    return 0;
}
