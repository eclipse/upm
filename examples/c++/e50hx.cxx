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

#include "e50hx.hpp"
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

    // Instantiate an E50HX object for an E50HX device that has 1075425
    // as it's unique Device Object Instance ID.  NOTE: You will
    // certainly want to change this to the correct value for your
    // device(s).
    E50HX sensor(1075425);

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
        cout << "System Voltage: " << sensor.getAnalogValue(E50HX::AV_System_Voltage) << " "
             << sensor.getAnalogValueUnits(E50HX::AV_System_Voltage) << endl;

        cout << "System Type: " << sensor.getAnalogValue(E50HX::AV_System_Type) << endl;

        cout << "Energy Consumption: " << sensor.getAnalogInput(E50HX::AI_Energy) << " "
             << sensor.getAnalogInputUnits(E50HX::AI_Energy) << endl;

        cout << "Power Up Counter: " << sensor.getAnalogInput(E50HX::AI_Power_Up_Count) << endl;

        cout << endl;
        upm_delay(5);
    }

    cout << "Exiting..." << endl;

    //! [Interesting]

    return 0;
}
