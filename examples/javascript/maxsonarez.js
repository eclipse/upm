/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*
* Author: Jon Trulson <jtrulson@ics.com>
* Copyright (c) 2015 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
