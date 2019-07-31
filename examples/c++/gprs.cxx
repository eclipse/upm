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

#include "gprs.hpp"
#include "upm_utilities.h"

using namespace std;
using namespace upm;

void
printUsage(char* progname)
{
    cout << "Usage: " << progname << " [AT command]" << endl;
    cout << endl;

    cout << "If an argument is supplied on the command line, that argument is" << endl;
    cout << "sent to the module and the response is printed out." << endl;
    cout << endl;
    cout << "If no argument is used, then the manufacturer and the current" << endl;
    cout << "saved profiles are queried and the results printed out." << endl;
    cout << endl;
    cout << endl;
}

// simple helper function to send a command and wait for a response
void
sendCommand(upm::GPRS& sensor, string cmd)
{
    // commands need to be terminated with a carriage return
    cmd += "\r";

    sensor.writeDataStr(cmd);

    // wait up to 1 second
    if (sensor.dataAvailable(1000)) {
        cout << "Returned: " << sensor.readDataStr(1024) << endl;
    } else {
        cerr << "Timed out waiting for response" << endl;
    }
}

int
main(int argc, char** argv)
{
    //! [Interesting]

    // Instantiate a GPRS Module on UART 0
    upm::GPRS sensor(0);

    // Set the baud rate, 19200 baud is the default.
    if (sensor.setBaudRate(19200) != 0) {
        cerr << "Failed to set tty baud rate" << endl;
        return 1;
    }

    printUsage(argv[0]);

    if (argc > 1) {
        cout << "Sending command line argument (" << argv[1] << ")..." << endl;
        sendCommand(sensor, argv[1]);
    } else {
        // query the module manufacturer
        cout << "Querying module manufacturer (AT+CGMI)..." << endl;
        sendCommand(sensor, "AT+CGMI");

        upm_delay(1);

        // query the saved profiles
        cout << "Querying the saved profiles (AT&V)..." << endl;
        sendCommand(sensor, "AT&V");

        // A comprehensive list is available from the datasheet at:
        // http://www.seeedstudio.com/wiki/images/7/72/AT_Commands_v1.11.pdf
    }

    //! [Interesting]

    return 0;
}
