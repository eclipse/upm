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


var sensorObj = require('jsupm_ds2413');


/************** Main code **************/
// Instantiate a DS2413 Module on a Dallas 1-wire bus connected to UART 0
var sensor = new sensorObj.DS2413(0);

// find all of the DS2413 devices present on the bus
sensor.init();

// how many devices were found?
console.log("Found", sensor.devicesFound(), "device(s)");

// read the gpio and latch values from the first device
// the lower 4 bits are of the form:
// <gpioB latch> <gpioB value> <gpioA latch> <gpioA value>
console.log("GPIO device 0 values:", sensor.readGpios(0));

// set the gpio latch values of the first device
console.log("Setting GPIO latches to on");;
sensor.writeGpios(0, 0x03);

sensor = null;
sensorObj.cleanUp();
sensorObj = null;
console.log("Exiting...");
process.exit(0);
