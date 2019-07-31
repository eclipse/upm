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

#include "rn2903.hpp"
#include "rn2903_defs.h"

using namespace std;

int
main(int argc, char** argv)
{
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
    // upm::RN2903 sensor = upm::RN2903(0, RN2903_DEFAULT_BAUDRATE);

    // enable debugging
    // sensor.setDebug(true);

    // get version
    if (sensor.command("sys get ver")) {
        cout << "Failed to retrieve device version string" << endl;
        return 1;
    }
    cout << "Firmware version: " << sensor.getResponse() << endl;

    cout << "Hardware EUI: " << sensor.getHardwareEUI() << endl;

    // we can support two types of join, OTAA and ABP. Each requires
    // that certain parameters be set first.  We will only attempt ABP
    // joining with this example since it's the only one that can
    // succeed without actual configuration.  In both cases, if you
    // are actually attempting to join a real LoRaWAN network, you
    // must change the parameters below to match the network you are
    // attempting to join.

    // For OTAA, you need to supply valid Device EUI, Application EUI,
    // and Application key:
    //
    // sensor.setDeviceEUI("0011223344556677");
    // sensor.setApplicationEUI("0011223344556677");
    // sensor.setApplicationKey("01234567012345670123456701234567");
    //
    // RN2903_JOIN_STATUS_T rv = sensor.join(RN2903_JOIN_TYPE_OTAA);
    // A successful join will return RN2903_JOIN_STATUS_ACCEPTED (0).
    // cout << "JOIN: got rv " << int(rv) << endl;

    // Try an ABP join.  Note, these parameters are made up.  For a
    // real network, you will want to use the correct values
    // obviously.  For an ABP join, you need to supply the Device
    // Address, Network Session Key, and the Application Session Key.

    sensor.setDeviceAddr("00112233");
    sensor.setNetworkSessionKey("00112233001122330011223300112233");
    sensor.setApplicationSessionKey("00112233001122330011223300112233");

    RN2903_JOIN_STATUS_T rv = sensor.join(RN2903_JOIN_TYPE_ABP);
    if (rv == RN2903_JOIN_STATUS_ACCEPTED) {
        cout << "Join successful." << endl;

        // All transmit payloads must be hex encoded strings, so
        // pretend we have a temperature sensor that gave us a value
        // of 25.6 C, and we want to transmit it.
        string faketemp = "25.6";
        cout << "Transmitting a packet..." << endl;

        RN2903_MAC_TX_STATUS_T trv;
        trv = sensor.macTx(RN2903_MAC_MSG_TYPE_UNCONFIRMED,
                           1, // port number
                           sensor.toHex(faketemp));

        if (trv == RN2903_MAC_TX_STATUS_TX_OK)
            cout << "Transmit successful." << endl;
        else {
            // check to see if we got a downlink packet
            if (trv == RN2903_MAC_TX_STATUS_RX_RECEIVED) {
                cout << "Transmit successful, downlink packet received: " << sensor.getResponse();
            } else {
                cout << "Transmit failed with code " << int(trv) << endl;
            }
        }
    } else {
        cout << "Join failed with code " << int(rv) << endl;
    }

    cout << "Exiting" << endl;

    //! [Interesting]

    return 0;
}
