/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

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

var sensorObj = require('jsupm_nmea_gps');

// Instantiate a NMEAGPS sensor on uart 0 at 9600 baud with enable
// pin on D3
var sensor = new sensorObj.NMEAGPS(0, 9600, 3);

// loop, dumping NMEA data out as fast as it comes in
while (sensor.dataAvailable(5000))
{
    process.stdout.write(sensor.readStr(256));
}
console.log("Timed out");

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});

