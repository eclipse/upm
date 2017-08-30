/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
