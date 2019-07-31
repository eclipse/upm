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
#include <string>

#include "sm130.hpp"

using namespace std;
using namespace upm;

int
main(int argc, char** argv)
{
    //! [Interesting]

    // Instantiate a UART based SM130 RFID Module using defaults
    upm::SM130 sensor;

    // set the baud rate.  19200 baud is the default.
    if (sensor.setBaudRate(19200)) {
        cerr << "Failed to set baud rate" << endl;
        return 1;
    }

    cout << "Resetting..." << endl;
    sensor.reset();

    cout << "Firmware revision: " << sensor.getFirmwareVersion() << endl;

    cout << "Waiting up to 5 seconds for a tag..." << endl;

    if (sensor.waitForTag(5000)) {
        cout << "Found tag, UID: " << sensor.string2HexString(sensor.getUID()) << endl;
        cout << "Tag Type: " << sensor.tag2String(sensor.getTagType()) << endl;
    } else {
        // error
        cout << "waitForTag failed: " << sensor.getLastErrorString() << endl;
    }

    //! [Interesting]

    cout << "Exiting" << endl;
    return 0;
}
