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

#include "tb7300.hpp"
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

    // Instantiate an TB7300 object for an TB7300 device that has 73001
    // as it's unique Device Object Instance ID.  NOTE: You will
    // certainly want to change this to the correct value for your
    // device(s).
    TB7300 sensor(73001);

    // Initialize our BACnet master, if it has not already been
    // initialized, with the device and baudrate, choosing 1000001 as
    // our unique Device Object Instance ID, 2 as our MAC address and
    // using default values for maxMaster and maxInfoFrames
    sensor.initMaster(defaultDev, 38400, 1000001, 2);

    // Uncomment to enable debugging output
    // sensor.setDebug(true);

    cout << endl;
    cout << "Device Name: " << sensor.getDeviceName() << endl;
    cout << "Device Description: " << sensor.getDeviceDescription() << endl;
    cout << "Device Location: " << sensor.getDeviceLocation() << endl;
    cout << endl;

    cout << "Fan Mode: " << sensor.getMultiStateValueText(TB7300::MV_Fan_Mode) << endl;

    cout << "Fan Status: " << sensor.getMultiStateValueText(TB7300::MV_Fan_Status) << endl;

    cout << "System Mode: " << sensor.getMultiStateValueText(TB7300::MV_System_Mode) << endl;

    cout << "Service Alarm: " << sensor.getBinaryInputText(TB7300::BI_Service_Alarm) << endl;

    cout << endl;

    // update and print the room temperature every 5 seconds
    while (shouldRun) {
        // update our values
        sensor.update();

        // we show both C and F for temperature
        cout << "Temperature: " << sensor.getTemperature() << " C / " << sensor.getTemperature(true)
             << " F" << endl;

        cout << endl;
        upm_delay(5);
    }

    cout << "Exiting..." << endl;

    //! [Interesting]

    return 0;
}
