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

#include "nmea_gps.hpp"

using namespace std;

bool shouldRun = true;

const size_t bufferLength = 256;

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

    // Instantiate a NMEA_GPS sensor on uart 0 at 9600 baud with enable
    // pin on D3.  If you do not need an enable pin, you can specify -1.
    upm::NMEAGPS sensor(0, 9600, 3);

    // loop, dumping NMEA data out as fast as it comes in
    while (shouldRun && sensor.dataAvailable(5000)) {
        cout << sensor.readStr(bufferLength);
    }

    if (shouldRun)
        cerr << "Timed out" << endl;

    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}
