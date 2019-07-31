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
#include <signal.h>
#include <string>

#include "rn2903.hpp"
#include "rn2903_defs.h"

using namespace std;

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

    string defaultDev = "/dev/ttyUSB0";
    if (argc > 1)
        defaultDev = argv[1];

    cout << "Using device: " << defaultDev << endl;

    // Instantiate a RN2903 sensor on defaultDev at 57600 baud.
    upm::RN2903 sensor(defaultDev, RN2903_DEFAULT_BAUDRATE);

    // To use an internal UART understood by MRAA, use the following
    // to inititialize rather than the above, which by default uses a
    // tty path.
    //
    //     upm::RN2903 sensor = upm::RN2903(0, RN2903_DEFAULT_BAUDRATE);

    // enable debugging
    // sensor.setDebug(true);

    // get version
    if (sensor.command("sys get ver")) {
        cout << "Failed to retrieve device version string" << endl;
        return 1;
    }
    cout << "Firmware version: " << sensor.getResponse() << endl;

    cout << "Hardware EUI: " << sensor.getHardwareEUI() << endl;

    // For this example, we will just try to receive a packet
    // transmitted by the p2p-tx rn2903 example.  We reset the
    // device to defaults, and we do not make any adjustments to the
    // radio configuration.  You will probably want to do so for a
    // real life application.

    // The first thing to do is to suspend the LoRaWAN stack on the device.
    sensor.macPause();

    // We will use continuous mode (window_size 0), though the default
    // radio watch dog timer will expire every 15 seconds.  We will
    // just loop here.

    while (shouldRun) {
        cout << "Waiting for packet..." << endl;
        RN2903_RESPONSE_T rv;
        rv = sensor.radioRx(0);
        if (rv) {
            cout << "radioRx() failed with code " << int(rv) << endl;
        } else {
            string resp = sensor.getResponse();
            string payload = sensor.getRadioRxPayload();
            if (!payload.size())
                cout << "Got response: '" << resp << "'" << endl;
            else
                cout << "Got payload: '" << sensor.fromHex(payload) << "'" << endl;
        }

        cout << endl;
    }

    cout << "Exiting" << endl;

    //! [Interesting]

    return 0;
}
