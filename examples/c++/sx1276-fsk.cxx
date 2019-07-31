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
#include <stdio.h>
#include <string>

#include "sx1276.hpp"
#include "upm_utilities.h"

using namespace std;

int shouldRun = true;

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

    cout << "Specify an argument to go into receive mode.  Default is transmit" << endl;

    bool rx = false;
    if (argc > 1)
        rx = true;

    // Instantiate an SX1276 using default parameters
    upm::SX1276 sensor;

    // 915Mhz
    sensor.setChannel(915000000);

    // FSK configuration (rx and tx must be configured the same):
    // Tx output power = 14 dBm
    // FSK freq deviation = 25000 Hz
    // FSK bandwidth = 50000 bps
    // FSK AFC bandwidth = 83333 Hz
    // FSK datarate = 50000 bps
    // FSK preamble len = 5
    // FSK fixed length payload = false
    // FSK CRC check = true
    // FSK (rx) continuous Rx mode = false

    sensor.setTxConfig(sensor.MODEM_FSK, 14, 25000, 0, 50000, 0, 5, false, true, false, 0, false);

    sensor.setRxConfig(
    sensor.MODEM_FSK, 50000, 50000, 0, 83333, 5, 0, false, 0, true, false, 0, false, true);

    int count = 0;
    int buflen = 64;
    char buffer[buflen];

    while (shouldRun) {
        if (!rx) {
            snprintf(buffer, buflen, "Ping %d", count++);
            cout << "Sending..." << std::string(buffer) << endl;
            sensor.sendStr(string(buffer), 3000);

            sensor.setSleep();
            upm_delay(1);
        } else {
            // receiving
            cout << "Attempting to receive..." << endl;
            int rv;
            if ((rv = sensor.setRx(3000))) {
                cout << "setRx returned " << rv << endl;
            } else {
                cout << "Received Buffer: " << sensor.getRxBufferStr() << endl;
            }

            // go back to upm_delay when done
            sensor.setSleep();
            upm_delay_us(250000);
        }
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
