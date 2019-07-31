/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

var factory = require('jsupm_ms5611');

// Instantiate a MS5611 instance using default i2c bus and address
var sensor = new factory.MS5611();

// now output data every 250 milliseconds
setInterval(function()
{
    var temperature = sensor.getTemperatureCelsius();
    var pressure = sensor.getPressurePa();
    console.log("Temperature: " + temperature + " C");
    console.log("Pressure: " + pressure + " Pa");
}, 250);

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});
