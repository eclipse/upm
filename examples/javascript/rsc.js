/*
* Author: Abhishek Malik <abhishek.malik@intel.com>
* Copyright (c) 2017 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var rsc = require("jsupm_rsc");

// Instantiate a Honeywell RSC Pressure Sensor at bus 0
var rsc_sensor = new rsc.RSC(0, 9, 8);

console.log("Sensor Name: " + rsc_sensor.getSensorName());
console.log("Sensor Serial No.: " + rsc_sensor.getSensorSerialNumber());

var myInterval = setInterval(function()
{
    console.log("Pressure: " + rsc_sensor.getPressure() + " " + rsc_sensor.getPressureUnit());
    console.log("Temperature: " + rsc_sensor.getTemperature() + " C");
}, 100);

// When exiting: clear interval and print message
process.on('SIGINT', function()
{
    clearInterval(myInterval);
    console.log("Exiting...");
    process.exit(0);
});
