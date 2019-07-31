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
#include <stdio.h>
#include <string.h>
#include <string>

#include "hm11.hpp"
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
    cout << "If no argument is used, then the address and PIN of the module" << endl;
    cout << "are queried and the results printed out." << endl;
    cout << endl;
    cout << endl;
}

// simple helper function to send a command and wait for a response
void
sendCommand(upm::HM11& ble, char* cmd)
{
    char buffer[BUFSIZ];
    ble.writeData(cmd, strlen(cmd));

    // wait up to 1 second
    if (ble.dataAvailable(1000)) {
        memset(buffer, 0, BUFSIZ);

        ble.readData(buffer, BUFSIZ - 1);
        cout << "Returned: " << buffer << endl;
    } else {
        cerr << "Timed out waiting for response" << endl;
    }
}

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate a HM11 BLE Module on UART 0

    upm::HM11 ble(0);

    // make sure port is initialized properly.  9600 baud is the default.
    if (!ble.setupTty(B9600)) {
        cerr << "Failed to setup tty port parameters" << endl;
        return 1;
    }

    printUsage(argv[0]);

    if (argc > 1) {
        cout << "Sending command line argument (" << argv[1] << ")..." << endl;
        sendCommand(ble, argv[1]);
    } else {
        // query the module address
        char addr[] = "AT+ADDR?";
        cout << "Querying module address (" << addr << ")..." << endl;
        sendCommand(ble, addr);

        upm_delay(1);

        // query the module address
        char pin[] = "AT+PASS?";
        cout << "Querying module PIN (" << pin << ")..." << endl;
        sendCommand(ble, pin);

        // Other potentially useful commands are:
        //
        // AT+VERS? - query module version
        // AT+ROLE0 - set as slave
        // AT+ROLE1 - set as master
        // AT+CLEAR - clear all previous settings
        // AT+RESET - restart the device
        //
        // A comprehensive list is available from the datasheet at:
        // http://www.seeedstudio.com/wiki/images/c/cd/Bluetooth4_en.pdf
    }

    //! [Interesting]

    return 0;
}
