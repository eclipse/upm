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

#include <ecezo.hpp>
#include <iostream>
#include <signal.h>
#include <upm_utilities.h>

using namespace std;
using namespace upm;

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

    // Instantiate a ECEZO sensor on uart 0 at 9600 baud.
    upm::ECEZO sensor(0, 9600, false);

    // For I2C, assuming the device is configured for address 0x64 on
    // I2C bus 0, you could use something like:
    //
    // upm::ECEZO sensor(0, 0x64, true);

    while (shouldRun) {
        // this will take about 1 second to complete
        sensor.update();

        cout << "EC " << sensor.getEC() << " uS/cm, TDS " << sensor.getTDS() << " mg/L, Salinity "
             << sensor.getSalinity() << " PSS-78, SG " << sensor.getSG() << endl;

        upm_delay(5);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
