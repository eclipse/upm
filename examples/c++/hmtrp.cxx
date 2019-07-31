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
#include <signal.h>
#include <string.h>
#include <string>

#include "hmtrp.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

void
printUsage()
{
    cout << "Usage:" << endl;
    cout << "Pass a commandline argument (any argument) to this program" << endl;
    cout << "to query the radio configuration and output it.  NOTE: the" << endl;
    cout << "radio must be in CONFIG mode for this to work." << endl;
    cout << endl;
    cout << "Running this program without arguments will simply transmit" << endl;
    cout << "'Hello World!' every second, and output any data received from" << endl;
    cout << "another radio." << endl;
    cout << endl;
}

const size_t bufferLength = 256;

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // Instantiate a HMTRP radio device on uart 0

    upm::HMTRP radio(0);

    // make sure port is initialized properly.  9600 baud is the default.
    if (!radio.setupTty(B9600)) {
        cerr << "Failed to setup tty port parameters" << endl;
        return 1;
    }

    printUsage();

    // By default, this radio simply transmits data sent via writeData()
    // and reads any available data via readData().

    // It can be placed into a configuration mode by grounding the
    // CONFIG pin on the module.  When this is done, the various
    // configuration query and config methods can be used.  In this
    // example, by default, we just read any data available fom the
    // device, and periodically transmit "Hello World".

    // If any argument was specified on the command line, do a simple
    // configuration query and output the results.  The radio must be in
    // CONFIG mode for this to work.

    if (argc > 1) {
        // config mode
        uint32_t freq;
        uint32_t dataRate;
        uint16_t rxBandwidth;
        uint8_t modulation;
        uint8_t txPower;
        uint32_t uartBaud;

        if (radio.getConfig(&freq, &dataRate, &rxBandwidth, &modulation, &txPower, &uartBaud)) {
            cout << "Radio configuration:" << endl;
            cout << "freq: " << freq << " dataRate: " << dataRate << " rxBandwidth: " << rxBandwidth
                 << "Khz" << endl;

            cout << "modulation: " << int(modulation) << "Khz txPower: " << int(txPower)
                 << " uartBaud: " << uartBaud << endl;
        } else {
            cerr << "getConfig() failed.  Make sure the radio is in "
                 << "CONFIG mode." << endl;
        }
    } else {
        // normal read/write mode
        char radioBuffer[bufferLength];
        int counter = 0;
        cout << "Running in normal read/write mode." << endl;

        while (shouldRun) {
            // we don't want the read to block in this example, so always
            // check to see if data is available first.
            if (radio.dataAvailable()) {
                memset(radioBuffer, 0, bufferLength);
                int rv = radio.readData(radioBuffer, bufferLength - 1);

                if (rv > 0)
                    cout << "Received: " << radioBuffer << endl;

                if (rv < 0) // some sort of read error occurred
                {
                    cerr << "Port read error." << endl;
                    break;
                }

                continue;
            }

            upm_delay_us(100000); // 100ms
            counter++;
            // every second, transmit "Hello World"
            if (counter > 10) {
                static const char* hello = "Hello World!";
                cout << "Transmitting hello world..." << endl;
                radio.writeData((char*) hello, strlen(hello) + 1);
                counter = 0;
            }
        }
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
