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

#include "ds2413.hpp"

using namespace std;
using namespace upm;

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate a DS2413 Module on a Dallas 1-wire bus connected to UART 0
    upm::DS2413 sensor(0);

    // find all of the DS2413 devices present on the bus
    sensor.init();

    // how many devices were found?
    cout << "Found " << sensor.devicesFound() << " device(s)" << endl;

    // read the gpio and latch values from the first device
    // the lower 4 bits are of the form:
    // <gpioB latch> <gpioB value> <gpioA latch> <gpioA value>
    cout << "GPIO device 0 values: " << sensor.readGpios(0) << endl;

    // set the gpio latch values of the first device
    cout << "Setting GPIO latches to on" << endl;
    sensor.writeGpios(0, 0x03);

    cout << "Exiting..." << endl;

    //! [Interesting]
    return 0;
}
