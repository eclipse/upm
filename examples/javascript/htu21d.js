/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

//Load the library
var upm = require('jsupm_htu21d');

//Create a sensor instance
var sensor = new upm.HTU21D(0);

//Update sensor data and print the values every second
var ival = setInterval(function () {
    sensor.sampleData();
    console.log("Relative Humidity: " + sensor.getHumidity() + "%");
    console.log("Compensated RH: " + sensor.getCompRH() + "%");
    console.log("Temperature: " + sensor.getTemperature() + "C");
    console.log("Dew Point: " + sensor.getDewPoint() + "C");
}, 1000);

//Clear interval and exit on ^C
process.on('SIGINT', function()
{
    clearInterval(ival);
    process.exit(0);
});
