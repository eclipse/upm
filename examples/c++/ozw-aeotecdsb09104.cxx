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

#include "aeotecdsb09104.hpp"
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

    // Instantiate an Aeotec DSB09104 instance, on device node 12.  You
    // will almost certainly need to change this to reflect your own
    // network.  Use the ozwdump example to see what nodes are
    // available.
    upm::AeotecDSB09104 sensor(12);

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

        cout << "Watts, Channel 1: " << sensor.getWattsC1() << " W" << endl;

        cout << "Watts, Channel 2: " << sensor.getWattsC2() << " W" << endl;

        cout << "Watts, Channel 3: " << sensor.getWattsC3() << " W" << endl;

        cout << "Energy, Channel 1: " << sensor.getEnergyC1() << " kWh" << endl;

        cout << "Energy, Channel 2: " << sensor.getEnergyC2() << " kWh" << endl;

        cout << "Energy, Channel 3: " << sensor.getEnergyC3() << " kWh" << endl;

        cout << "Battery Level: " << sensor.getBatteryLevel() << "%" << endl;

        cout << endl;

        upm_delay(3);
    }

    //  sensor.dumpNodes(true);
    cout << "Exiting..." << endl;

    //! [Interesting]
    return 0;
}
