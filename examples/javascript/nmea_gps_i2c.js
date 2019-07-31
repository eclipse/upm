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

function sleepFor(millis)
{
    var now = new Date().getTime();
    while(new Date().getTime() < now + millis)
    {
        /* do nothing */
    }
}

// Instantiate a NMEA_GPS UBLOX based i2c sensor on i2c bus 0 at
// address 0x42
var sensor = new sensorObj.NMEAGPS(0, 0x42);

// loop, dumping NMEA data out as fast as it comes in
while (true)
{
    if (sensor.dataAvailable(0))
        process.stdout.write(sensor.readStr(256));
    else
        sleepFor(10);
}

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});
