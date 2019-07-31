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

// For this example, we will just try transmitting a packet over
// LoRa.  We reset the device to defaults, and we do not make any
// adjustments to the radio configuration.  You will probably want
// to do so for a real life application.

// The first thing to do is to suspend the LoRaWAN stack on the device.
sensor.macPause();

// the default radio watchdog timer is set for 15 seconds, so we
// will send a packet every 10 seconds.  In reality, local
// restrictions limit the amount of time on the air, so in a real
// implementation, you would not want to send packets that
// frequently.

var count = 0;
function transmit()
{
    var output = "Ping " + count;
    count++;

    // All payloads must be hex encoded
    var payload = sensor.toHex(output);

    console.log("Transmitting a packet, data: '"
                + output
                + "' -> hex: '"
                + payload
                + "'");

    var rv = sensor.radioTx(payload);

    if (rv == sensorObj.RN2903_RESPONSE_OK)
        console.log("Transmit successful.");
    else
        console.log("Transmit failed with code " + rv);

    console.log();
}

setInterval(transmit, 10000);

process.on('SIGINT', function() {
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting...");
    process.exit(0);
});
