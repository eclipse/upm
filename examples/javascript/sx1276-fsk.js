/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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
