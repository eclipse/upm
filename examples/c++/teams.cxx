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

#include "teams.hpp"
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

    // Instantiate an TEAMS instance, using A0 for temperature, and
    // 165.0 ohms for the rResistor value (for the libelium 4-20ma
    // interface)
    upm::TEAMS sensor(0, 165.0);

    // update and print available values every second
    while (shouldRun) {
        // update our values from the sensor
        sensor.update();

        // is the sensor connected? (current >= 4ma)
        cout << "Is Connected: " << sensor.isConnected() << endl;

        // print computed current on the loop.  This includes the offset,
        // if one was set by setOffsetMilliamps().
        cout << "Milliamps: " << sensor.getRawMilliamps() << endl;

        // we show both C and F for temperature
        cout << "Temperature: " << sensor.getTemperature() << " C / " << sensor.getTemperature(true)
             << " F" << endl;

        cout << endl;

        upm_delay(1);
    }

    cout << "Exiting..." << endl;

    //! [Interesting]

    return 0;
}
