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
#include <string>

#include "t8100.hpp"
#include "upm_utilities.h"

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
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // You will need to edit this example to conform to your site and your
    // devices, specifically the Device Object Instance ID passed to the
    // constructor, and the arguments to initMaster() that are
    // appropriate for your BACnet network.

    string defaultDev = "/dev/ttyUSB0";

    // if an argument was specified, use it as the device instead
    if (argc > 1)
        defaultDev = string(argv[1]);

    cout << "Using device " << defaultDev << endl;
    cout << "Initializing..." << endl;

    // Instantiate an T8100 object for an T8100 device that has 568000
    // as it's unique Device Object Instance ID.  NOTE: You will
    // certainly want to change this to the correct value for your
    // device(s).
    T8100 sensor(568000);

    // Initialize our BACnet master, if it has not already been
    // initialized, with the device and baudrate, choosing 1000001 as
    // our unique Device Object Instance ID, 2 as our MAC address and
    // using default values for maxMaster and maxInfoFrames
    sensor.initMaster(defaultDev, 38400, 1000001, 2);

    // Uncomment to enable debugging output
    // sensor.setDebug(true);

    cout << endl;
    cout << "Device Description: " << sensor.getDeviceDescription() << endl;
    cout << "Device Location: " << sensor.getDeviceLocation() << endl;
    cout << endl;

    // update and print a few values every 5 seconds
    while (shouldRun) {
        // update our values
        sensor.update();

        cout << "CO2 Concentration: " << sensor.getCO2() << " ppm" << endl;

        // we show both C and F for temperature
        cout << "Temperature: " << sensor.getTemperature() << " C / " << sensor.getTemperature(true)
             << " F" << endl;

        cout << "Humidity: " << sensor.getHumidity() << " %RH" << endl;

        cout << "Relay State: " << sensor.getRelayState() << endl;

        cout << endl;
        upm_delay(5);
    }

    cout << "Exiting..." << endl;

    //! [Interesting]

    return 0;
}
