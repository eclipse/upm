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
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

const size_t bufferLength = 128;

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

    // Instantiate a NMEA_GPS UBLOX based i2c sensor on i2c bus 0 at
    // address 0x42
    upm::NMEAGPS sensor(0, 0x42);

    // loop, dumping NMEA data out as fast as it comes in
    while (shouldRun) {
        if (sensor.dataAvailable(0))
            upm_delay_us(100);
        else
            cout << sensor.readStr(bufferLength);
    }

    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}
