/*
* Author: Zion Orent <zorent@ics.com>
* Author: Jon Trulson <jtrulson@ics.com>
* Copyright (c) 2014-2017 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var sensorObj = require('jsupm_bmpx8x');

// Instantiate a BMPX8X sensor on I2C using defaults.
var sensor = new sensorObj.BMPX8X();

// Print the pressure, altitude, sea level, and
// temperature values every 0.5 seconds
setInterval(function()
{
    sensor.update();

    console.log("Pressure: "
                + sensor.getPressure()
                + " Pa, Temperature: "
                + sensor.getTemperature()
                + " C, Altitude: "
                + sensor.getAltitude()
                + " m, Sea level: "
                + sensor.getSealevelPressure()
                + " Pa");
}, 500);

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});
