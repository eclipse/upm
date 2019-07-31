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

#include "tex00.hpp"
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

    // Instantiate an TEX00 instance, using A0 for the analog input.  In
    // this example, we are using a 10K Ohm balance resistor and a TED
    // (10k type 2) thermistor.
    upm::TEX00 sensor(0, 10000, upm::TEX00::STYPE_THERMISTOR_TED);

    cout << "Minimum temperature: " << sensor.getTemperatureRangeMin() << " C" << endl;
    cout << "Maximum temperature: " << sensor.getTemperatureRangeMax() << " C" << endl;
    cout << endl;

    // update and print available values every second
    while (shouldRun) {
        // update our values from the sensor
        sensor.update();

        if (sensor.isOutOfRange()) {
            cout << "Temperature out of range" << endl;
        } else {
            // we show both C and F for temperature
            cout << "Temperature: " << sensor.getTemperature() << " C / "
                 << sensor.getTemperature(true) << " F" << endl;
        }

        cout << endl;

        upm_delay(1);
    }

    cout << "Exiting..." << endl;

    //! [Interesting]

    return 0;
}
