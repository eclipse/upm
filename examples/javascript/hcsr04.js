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

var HCSR04 = require('jsupm_hcsr04');

// Instantiate a HCSR04 ultrasonic distance sensor on digital pins D2 and D4
var myHCSR04 = new HCSR04.HCSR04(2, 4);

// Check every second for the presence of a magnetic field (south polarity)
setInterval(function()
{
        console.log("Distance: " + myHCSR04.getDistance(0));
}, 1000);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});
