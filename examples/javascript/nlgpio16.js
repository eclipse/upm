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


var sensorObj = require('jsupm_nlgpio16');

// Instantiate a NLGPIO16 Module on the default UART (/dev/ttyACM0)
var sensor = new sensorObj.NLGPIO16();

// get the Version
console.log("Device Version:", sensor.getVersion());
// read the gpio at pin 3
console.log("GPIO 3 Value:", sensor.gpioRead(3));
// read the analog voltage at pin 5
console.log("Analog 5 Voltage:", sensor.analogReadVolts(5));
// set the gpio at pin 14 to HIGH
sensor.gpioSet(14);

/************** Exit code **************/
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting...");
    process.exit(0);
});
