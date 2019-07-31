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

#include "dfrec.hpp"
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
main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]

    // Instantiate a DFRobot EC sensor on analog pin A0, with a ds18b20
    // temperature sensor connected to UART 0, and a device index (for
    // the ds1820b uart bus) of 0, and an analog reference voltage of
    // 5.0.
    upm::DFREC sensor(0, 0, 0, 5.0);

    // Every 2 seconds, update and print values
    while (shouldRun) {
        sensor.update();

        cout << "EC = " << sensor.getEC() << " ms/cm" << endl;

        cout << "Volts = " << sensor.getVolts() << ", Temperature = " << sensor.getTemperature()
             << " C" << endl;

        cout << endl;

        upm_delay(2);
    }

    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}
