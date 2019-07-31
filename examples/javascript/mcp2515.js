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

var sensorObj = require('jsupm_mcp2515');

// Instantiate a MCP2515 on SPI bus 0 using a hw CS pin (-1).
var sensor = new sensorObj.MCP2515(0, -1);

// set the mode to loopback mode.  In loopback mode, transmitted
// packets are sent directly to an appropriate receive buffer
// without actually going out onto the CAN bus.
sensor.setOpmode(sensorObj.MCP2515_OPMODE_LOOPBACK);

// lets build up a packet and try loading it (8 bytes max).
var myPayload = "01234567";
console.log("Loading a packet of 8 numbers (0-7) into a TX buffer...");
sensor.loadTXBuffer(sensorObj.MCP2515_TX_BUFFER0, 0, false, false, myPayload);

// now lets try to transmit it
console.log("Transmitting packet...");
sensor.transmitBuffer(sensorObj.MCP2515_TX_BUFFER0, true);

console.log("Transmit successful");

// There should now be a packet waiting for us in RXB0
if (sensor.rxStatusMsgs() == sensorObj.MCP2515_RXMSG_RXB0)
{
    console.log("Packet received in RXB0, decoding...");

    // now lets retrieve and print it
    sensor.getRXMsg(sensorObj.MCP2515_RX_BUFFER0);

    sensor.printMsg();
}
else
{
    console.log("No packet found, how strange.");
}

console.log("Exiting...");

sensor = null;
sensorObj.cleanUp();
sensorObj = null;
process.exit(0);
