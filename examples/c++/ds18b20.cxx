/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>

#include "ds18b20.hpp"
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

    cout << "Initializing..." << endl;

    // Instantiate an DS18B20 instance using the default values (uart 0)
    upm::DS18B20 sensor;

    cout << "Found " << sensor.devicesFound() << " device(s)" << endl;
    cout << endl;

    // bail if we didn't find anything
    if (!sensor.devicesFound())
        return 1;

    // update and print available values every 2 seconds
    while (shouldRun) {
        // update our values for all of the detected sensors
        sensor.update(-1);

        // we show both C and F for temperature for the sensors
        int i;
        for (i = 0; i < sensor.devicesFound(); i++) {
            cout << "Device " << i << ": Temperature: " << sensor.getTemperature(i) << " C / "
                 << sensor.getTemperature(i, true) << " F" << endl;
        }
        cout << endl;

        upm_delay(2);
    }

    cout << "Exiting..." << endl;

    //! [Interesting]

    return 0;
}
