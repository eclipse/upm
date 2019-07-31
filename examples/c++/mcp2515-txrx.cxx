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
#include <signal.h>
#include <string>
#include <upm_utilities.h>

#include "mcp2515.hpp"
#include "mcp2515_regs.h"

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

    // by default we just receive packets
    bool do_tx = false;

    // if an argument is given, we will transmit packets
    if (argc > 1)
        do_tx = true;
    //! [Interesting]

    // NOTE: This example assumes that only two devices are connected
    // to the CAN bus, and that both devices are running this example;
    // one in TX (transmit) mode, and one in RX (receive) mode.

    // If this is not the case, then in rx mode you may see other
    // packets from other devices, or not, depending on whether the
    // speed is correct for the network.  In tx mode, errors may be
    // generated on the bus, especially if the CAN bus speed does not
    // match the rest of the network.

    // You should start the receiver example first.  The transmitter
    // example will fail after about 5 seconds (timeout) of not being
    // able to transmit a message.

    // Instantiate a MCP2515 on SPI bus 0 using a hw CS pin (-1).
    upm::MCP2515 sensor(0, 9);

    // By default, after initialization, the baud rate is set to
    // 50Kbps, and the mode is NORMAL, so we don't need to set any of
    // that here.

    // our TX payload.  If transmitting, the first number will be
    // incremented (and rollover) on each transmission.
    string myPayload = "01234567";

    while (shouldRun) {
        if (do_tx) {
            cout << "Loading a packet of 8 numbers (0-7) into a TX buffer..." << endl;
            sensor.loadTXBuffer(MCP2515_TX_BUFFER0, 0, false, false, myPayload);

            // now lets try to transmit it
            cout << "Transmitting packet..." << endl;
            sensor.transmitBuffer(MCP2515_TX_BUFFER0, true);

            myPayload[0]++;
            cout << "Transmit successful" << endl;
            cout << endl;

            upm_delay_ms(500);
        } else {
            // RX mode
            // Look for a packet waiting for us in RXB0
            if (sensor.rxStatusMsgs() == MCP2515_RXMSG_RXB0) {
                cout << "Packet received in RXB0, decoding..." << endl;

                // now lets retrieve and print it
                sensor.getRXMsg(MCP2515_RX_BUFFER0);

                sensor.printMsg();
                cout << endl;
            }

            upm_delay_ms(100);
        }
    }

    cout << "Exiting..." << endl;

    //! [Interesting]

    return 0;
}
