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
