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

    // LORA configuration (rx and tx must be configured the same):
    // Tx output power = 14 dBm
    // LORA bandwidth = 125000 (can also be 250K and 500K)
    // LORA spreading factor = 7
    // LORA coding rate = 1 (4/5)
    // LORA preamble len = 8
    // LORA symbol timeout = 5
    // LORA fixed payload = false
    // LORA IQ inversion = false
    // LORA (rx) continuous Rx mode = true

    sensor.setTxConfig(sensor.MODEM_LORA, 14, 0, 125000, 7, 1, 8, false, true, false, 0, false);

    sensor.setRxConfig(
    sensor.MODEM_LORA, 125000, 7, 1, 0, 8, 5, false, 0, true, false, 0, false, true);

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
            upm_delay_us(5000);
        }
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
