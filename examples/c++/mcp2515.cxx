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
#include <string>

#include "mcp2515.hpp"
#include "mcp2515_regs.h"

using namespace std;

int
main(int argc, char** argv)
{
    //! [Interesting]

    // Instantiate a MCP2515 on SPI bus 0 using a hw CS pin (-1).
    upm::MCP2515 sensor(0, -1);

    cout << "Setting loopback mode..." << endl;

    // set the mode to loopback mode.  In loopback mode, transmitted
    // packets are sent directly to an appropriate receive buffer
    // without actually going out onto the CAN bus.
    sensor.setOpmode(MCP2515_OPMODE_LOOPBACK);

    // lets build up a packet and try loading it (8 bytes max).
    string myPayload = "01234567";
    cout << "Loading a packet of 8 numbers (0-7) into a TX buffer..." << endl;
    sensor.loadTXBuffer(MCP2515_TX_BUFFER0, 0, false, false, myPayload);

    // now lets try to transmit it
    cout << "Transmitting packet..." << endl;
    sensor.transmitBuffer(MCP2515_TX_BUFFER0, true);

    cout << "Transmit successful" << endl;

    // There should now be a packet waiting for us in RXB0
    if (sensor.rxStatusMsgs() == MCP2515_RXMSG_RXB0) {
        cout << "Packet received in RXB0, decoding..." << endl;

        // now lets retrieve and print it
        sensor.getRXMsg(MCP2515_RX_BUFFER0);

        sensor.printMsg();
    } else {
        cout << "No packet found, how strange." << endl;
    }

    cout << "Exiting..." << endl;

    //! [Interesting]

    return 0;
}
