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
