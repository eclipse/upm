/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>

#include "uartat.hpp"

using namespace std;

const size_t bufferLength = 256;

int
main(int argc, char** argv)
{
    //! [Interesting]

    string defaultDev = string("/dev/ttyUSB0");
    if (argc > 1)
        defaultDev = string(argv[1]);

    cout << "Using device: " << defaultDev << endl;

    // Instantiate a UARTAT sensor on defaultDev at 115200 baud.
    upm::UARTAT sensor(defaultDev, 115200);

    // This is a simplistic example that tries to configure the LE910,
    // and use it's built-in socket capabilities to connect to a
    // remote host, obtain a small piece of data, and return it.  It's
    // mainly intended to show you how you can use the various AT
    // commands supported by the LE910 to perform simple tasks.
    //
    // You must have a valid SIM card with an active data plan for
    // this example to do anything interesting.
    //
    // See the LE910 AT Commands reference for full information on
    // what is possible with this device.  The uartat driver is
    // intended to make it a little easier to control AT-style
    // devices, but is by no means a full-featured communication
    // infrastructure.  A "real" application will probably need to be
    // much more sophisticated with regard to parsing, doing retries,
    // etc.
    //
    // For experimenting with various AT commands, try using an
    // interactive terminal emulator like minicom or screen.

    // make sure we are in command mode
    if (!sensor.inCommandMode()) {
        cout << "Not in command mode, switching..." << endl;
        sensor.commandMode("+++", 1000);
    }

    // flter out CR's in responses by default
    sensor.filterCR(true);

    cout << "Configuring modem..." << endl;

    // discard any waiting characters
    sensor.drain();

    // reset modem
    sensor.command("ATZ\r");

    // turn off command echo, set verbosity to 1, enable data
    // connection mode
    sensor.command("ATE0 V1 +FCLASS=0\r");
    sensor.drain();

    // Now issue some commands and output the results.

    cout << "Modem and SIM information:" << endl;
    std::string buffer;
    buffer = sensor.commandWithResponse("AT+ICCID\r", bufferLength);
    if (!buffer.empty())
        cout << "ICCID (SIM ID): " << buffer << endl;

    buffer = sensor.commandWithResponse("AT+CGSN=1\r", bufferLength);
    if (!buffer.empty())
        cout << "IMEI: " << buffer << endl;

    // see if we are on the network....
    buffer = sensor.commandWithResponse("AT+CREG?\r", bufferLength);
    if (!buffer.empty()) {
        cout << buffer << endl;

        // look for "CGREG: 0,1" or "CGREG: 0,5"
        if (sensor.find(buffer, "CREG: 0,1") || sensor.find(buffer, "CREG: 0,5")) {
            cout << "Connected to the cell data network." << endl;

            // wait up to 5 seconds for responses now...
            sensor.setResponseWaitTime(5000);

            // setup PDP context (socket 1).  An ERROR repsonse is
            // possible if the PDP context is already set up.
            sensor.command("AT#SGACT=1,1\r");

            // setup a TCP socket to nist.gov and read the timestamp.

            cout << "Connecting to time-a.nist.gov, TCP port 13" << endl;

            // Wait up to 60 seconds to find the NO CARRIER
            // string, which will be present at the end, if the
            // connection succeeded and the requested data was
            // obtained.
            buffer = sensor.commandWaitFor("AT#SD=1,0,13,\"time-a.nist.gov\"\r",
                                           bufferLength,
                                           "\nNO CARRIER\n",
                                           60000);
            if (!buffer.empty()) {
                // print out the response
                cout << "RESPONSE: " << endl << buffer << endl;
            } else {
                cout << "No response." << endl;
            }

            // destroy PDP context
            sensor.setResponseWaitTime(250);
            sensor.command("AT#SGACT=1,0\r");
        } else {
            cout << "You do not appear to be connected to the network..." << endl;
        }
    } else {
        cout << "Error executing query\n" << endl;
    }

    // reset the modem
    sensor.command("ATZ\r");

    cout << "Exiting" << endl;

    //! [Interesting]

    return 0;
}
