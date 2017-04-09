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

import upm_mcp2515.MCP2515;

public class MCP2515_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a MCP2515 on SPI bus 0 using a hw CS pin (-1).
        MCP2515 sensor = new MCP2515(0, -1);

        // set the mode to loopback mode.  In loopback mode, transmitted
        // packets are sent directly to an appropriate receive buffer
        // without actually going out onto the CAN bus.
        sensor.setOpmode(upm_mcp2515.MCP2515_OPMODE_T.MCP2515_OPMODE_LOOPBACK);

        // lets build up a packet and try loading it (8 bytes max).
        String myPayload = "01234567";
        System.out.println("Loading a packet of 8 numbers (0-7) into a TX buffer...");
        sensor.loadTXBuffer(upm_mcp2515.MCP2515_TX_BUFFER_T.MCP2515_TX_BUFFER0,
                            0, false, false, myPayload);

        // now lets try to transmit it
        System.out.println("Transmitting packet...");
        sensor.transmitBuffer(upm_mcp2515.MCP2515_TX_BUFFER_T.MCP2515_TX_BUFFER0,
                              true);

        System.out.println("Transmit successful");

        // There should now be a packet waiting for us in RXB0
        if (sensor.rxStatusMsgs() == upm_mcp2515.MCP2515_RXMSG_T.MCP2515_RXMSG_RXB0)
        {
            System.out.println("Packet received in RXB0, decoding...");

            // now lets retrieve and print it
            sensor.getRXMsg(upm_mcp2515.MCP2515_RX_BUFFER_T.MCP2515_RX_BUFFER0);

            sensor.printMsg();
        }
        else
        {
            System.out.println("No packet found, how strange.");
        }

        System.out.println("Exiting...");

// ! [Interesting]
    }
}
