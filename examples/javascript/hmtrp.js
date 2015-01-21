/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

/*
* Author: Zion Orent <zorent@ics.com>
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

function printUsage()
{
	var outputStr = "Usage:\n" +
	"Pass a commandline argument (any argument) to this program\n" +
	"to query the radio configuration and output it.  NOTE: the\n" +
	"radio must be in CONFIG mode for this to work.\n\n" +
	"Running this program without arguments will simply transmit\n" +
	"'Hello World!' every second, and output any data received from\n" +
	"another radio.\n\n";
	console.log(outputStr);
}

var HMTRP_Radio = require('jsupm_hmtrp');

// Instantiate a HMTRP radio device on uart 0
var my_HMTRP_Radio = new HMTRP_Radio.HMTRP(0);

var g_myInterval;
var g_counter = 0;

// normal read/write mode
var bufferLength = 256;
var radioBuffer = new HMTRP_Radio.charArray(bufferLength);

// make sure port is initialized properly. 9600 baud is the default.
if (!my_HMTRP_Radio.setupTty(HMTRP_Radio.int_B9600))
{
	console.log("Failed to setup tty port parameters");
	process.exit(0);
}

printUsage();

// By default, this radio simply transmits data sent via writeData()
// and reads any available data via readData().

// It can be placed into a configuration mode by grounding the
// CONFIG pin on the module.  When this is done, the various
// configuration query and config methods can be used.  In this
// example, by default, we just read any data available fom the
// device, and periodically transmit "Hello World".

// If any argument was specified on the command line, do a simple
// configuration query and output the results.  The radio must be in 
// CONFIG mode for this to work.


// Note that the first command-line argument
// should be "node" and the second is "hmtrp.js".
// The data we want would be the third... if it exists
if (process.argv.length > 2)
{
	// config mode
	var freq = new HMTRP_Radio.uint32Array(0);
	var dataRate = new HMTRP_Radio.uint32Array(0);
	var rxBandwidth = new HMTRP_Radio.uint16Array(0);
	var modulation = new HMTRP_Radio.uint8Array(0);
	var txPower = new HMTRP_Radio.uint8Array(0);
	var uartBaud = new HMTRP_Radio.uint32Array(0);

	if (my_HMTRP_Radio.getConfig(
	freq, dataRate, rxBandwidth, modulation, txPower, uartBaud))
	{
		console.log("Radio configuration:");
		var outputStr = "freq: " + freq.getitem(0) +
		" dataRate: " + dataRate.getitem(0) +
		" rxBandwidth: " + rxBandwidth.getitem(0) + "Khz";
		console.log(outputStr);

		outputStr = "modulation: " + parseInt(modulation.getitem(0));
		outputStr += " Khz txPower: " + parseInt(txPower.getitem(0));
		outputStr += " uartBaud: " + uartBaud.getitem(0);
		console.log(outputStr);
	}
	else
	{
		var errString = "getConfig() failed.  Make sure the radio " +
					"is in CONFIG mode.";
		console.log(errString);
	}
}
else
{
	console.log("Running in normal read/write mode.");
	g_myInterval = setInterval(runRadio, 100); // 100ms
}

function runRadio()
{
	// we don't want the read to block in this example, so always
	// check to see if data is available first.
	if (my_HMTRP_Radio.dataAvailable())
	{
		var rv = my_HMTRP_Radio.readData(radioBuffer, bufferLength);

		if (rv > 0)
		{
			var resultStr = "";
			for (var x = 0; x < rv; x++)
				resultStr += radioBuffer.getitem(x);
			console.log("Received: " + resultStr);
		}

		if (rv < 0) // some sort of read error occured
		{
			console.log("Port read error.");
			return;
		}
	}	
	g_counter++;
	// every second, transmit "Hello World"
	if (g_counter > 10)
	{
		var msg = "Hello World!";

		console.log("Transmitting " + msg + "...");

		// Adding 1 for NULL terminator.
		// Note that SWIG automatically adds a NULL terminator,
		// so no need to NULL-terminate ourselves.
		// Just increment the message length to include
		// the NULL that's already there
		my_HMTRP_Radio.writeData(msg, (msg.length + 1));
		g_counter = 0;
	}
}

// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(g_myInterval);
	console.log("Exiting...");
	process.exit(0);
});
