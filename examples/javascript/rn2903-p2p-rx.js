/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
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

var sensorObj = require('jsupm_rn2903');

var defaultDev = "/dev/ttyUSB0";

// if an argument was specified, use it as the device instead
if (process.argv.length > 2)
{
    defaultDev = process.argv[2];
}

console.log("Using device:", defaultDev);

// Instantiate a RN2903 sensor on defaultDev at 57600 baud.
var sensor = new sensorObj.RN2903(defaultDev,
                                  sensorObj.RN2903_DEFAULT_BAUDRATE);

// To use an internal UART understood by MRAA, use the following
// to inititialize rather than the above, which by default uses a
// tty path.
//
// var sensor = new sensorObj.RN2903(0,
//                                   sensorObj.RN2903_DEFAULT_BAUDRATE);

// enable debugging
// sensor.setDebug(true);

// get version
if (sensor.command("sys get ver"))
{
    console.log("Failed to retrieve device version string");
    process.exit(1);
}
console.log("Firmware version: " + sensor.getResponse());

console.log("Hardware EUI: " + sensor.getHardwareEUI());

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

while (true)
{
    console.log("Waiting for packet...");
    var rv = sensor.radioRx(0);
    if (rv)
    {
        console.log("radioRx() failed with code " + rv);
    }
    else
    {
        var resp = sensor.getResponse();
        var payload = sensor.getRadioRxPayload();
        if (!payload.length)
            console.log("Got response: '" + resp + "'");
        else
            console.log("Got payload: '"
                        + sensor.fromHex(payload)
                        + "'");
    }

    console.log();
}

process.on('SIGINT', function() {
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting...");
    process.exit(0);
});
