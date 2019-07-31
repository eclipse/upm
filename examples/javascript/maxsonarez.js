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

// JavaScript example for the MaxSonar-EZ family of ultrasonic range finders

// analog voltage, usually 3.3 or 5.0
var MAXSONAREZ_AREF = 5.0;

var MaxSonarEZ = require('jsupm_maxsonarez');

// Instantiate a MaxSonar-EZ on analog pin A1, with an analog
// reference voltage of MAXSONAREZ_AREF
var mySonar = new MaxSonarEZ.MAXSONAREZ(1, MAXSONAREZ_AREF);

// Every second, sample the sonar and output the distance to an
// object in inches.

// With the EZ3 version, the minimum and maximum ranges seemed to be
// between 6 and 33 inches

var myInterval = setInterval(function()
{
	console.log("AREF: " + MAXSONAREZ_AREF + 
                    ", distance in inches: " + mySonar.inches());
}, 1000);

// Print message when exiting
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	mySonar = null;
	MaxSonarEZ.cleanUp();
	MaxSonarEZ = null;
	console.log("Exiting...");
	process.exit(0);
});
