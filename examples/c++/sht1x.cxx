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

#include "sht1x.hpp"
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

    // Instantiate a SHT1X sensor using D2 as the clock, and D3 as the
    // data pin.
    upm::SHT1X sensor(2, 3);

    // Every 2 seconds, update and print values
    while (shouldRun) {
        sensor.update();

        cout << "Temperature: " << sensor.getTemperature() << " C" << endl;

        cout << "Humidity:    " << sensor.getHumidity() << " RH" << endl;

        cout << endl;

        upm_delay(2);
    }

    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}
