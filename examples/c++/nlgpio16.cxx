/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include "nlgpio16.hpp"

using namespace std;
using namespace upm;

//! [Interesting]
int
main(int argc, char** argv)
{
    // Instantiate a NLGPIO16 Module on the default UART (/dev/ttyACM0)
    upm::NLGPIO16 sensor;

    // get the Version
    cout << "Device Version: " << sensor.getVersion() << endl;
    // read the gpio at pin 3
    cout << "GPIO 3 Value: " << sensor.gpioRead(3) << endl;
    // read the analog voltage at pin 5
    cout << "Analog 5 Voltage: " << sensor.analogReadVolts(5) << endl;
    // set the gpio at pin 14 to HIGH
    sensor.gpioSet(14);

    return 0;
}

//! [Interesting]
