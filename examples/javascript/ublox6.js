/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */
/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2014 Intel Corporation.
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

var GPSSensor = require('jsupm_ublox6');

// Instantiate a Ublox6 GPS device on uart 0.
var myGPSSensor = new GPSSensor.Ublox6(0);

if (!myGPSSensor.setupTty(GPSSensor.int_B9600))
{
	console.log("Failed to setup tty port parameters");
	process.exit(0);
}

// Collect and output NMEA data.

// This device also supports numerous configuration options, which
// you can set with writeData().  Please refer to the Ublox-6 data
// sheet for further information on the formats of the data sent and
// received, and the various operating modes available.

var bufferLength = 256;
var nmeaBuffer  = new GPSSensor.charArray(bufferLength);

function getGPSInfo()
{
	// we don't want the read to block in this example, so always
	// check to see if data is available first.
	if (myGPSSensor.dataAvailable())
	{
		var rv = myGPSSensor.readData(nmeaBuffer, bufferLength);

		var GPSData, dataCharCode, isNewLine, lastNewLine;
		var numlines= 0;
		if (rv > 0)
		{
			GPSData = "";
			// read only the number of characters
			// specified by myGPSSensor.readData
			for (var x = 0; x < rv; x++)
				GPSData += nmeaBuffer.getitem(x);
			process.stdout.write(GPSData)
		}

		if (rv < 0) // some sort of read error occured
		{
			console.log("Port read error.");
			process.exit(0);
		}
	}
}

setInterval(getGPSInfo, 100);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});
