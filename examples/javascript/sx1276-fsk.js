/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */


var sensorObj = require('jsupm_sx1276');

var count = 0;
var interval;

/************** Functions **************/
function transmit(sensor)
{
    var buffer = "Ping " + count;
    count++;
    console.log("Sending... " + buffer);

    sensor.sendStr(buffer, 3000);
    sensor.setSleep();
}

function receive(sensor)
{
    console.log("Attempting to receive... ");
    var rv = 0;
    rv = sensor.setRx(3000);

    if (rv)
    {
        console.log("setRx returned " + rv);
    }
    else
    {
        console.log("Received Buffer: " + sensor.getRxBufferStr());
    }

    sensor.setSleep();
}

/************** Main code **************/
// Instantiate an SX1276 using default parameters

var sensor = new sensorObj.SX1276();

console.log("Specify an argument to go into receive mode.  Default is transmit");

// 915Mhz
sensor.setChannel(915000000)

// FSK configuration (rx and tx must be configured the same):
// Tx output power = 14 dBm
// FSK freq deviation = 25000 Hz
// FSK bandwidth = 50000 bps
// FSK AFC bandwidth = 83333 Hz
// FSK datarate = 50000 bps
// FSK preamble len = 5
// FSK fixed length payload = false
// FSK CRC check = true
// FSK (rx) continuous Rx mode = False

sensor.setTxConfig(sensorObj.SX1276.MODEM_FSK, 14, 25000, 0,
                   50000, 0, 5, false, true, false, 0, false);
    
sensor.setRxConfig(sensorObj.SX1276.MODEM_FSK, 50000, 50000,
                   0, 83333, 5, 0, false, 0, true,
                   false, 0, false, true);


if (process.argv.length > 2)
{
    // receive mode
    interval = setInterval(function() { receive(sensor); }, 250);
}
else
{
    // transmit mode
    interval = setInterval(function() { transmit(sensor); }, 1000);
}


/************** Exit code **************/
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting...");
    process.exit(0);
});
