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


/************** Variables **************/
// normal read/write mode
var bufferLength = 256;

var ble = require('jsupm_hm11');


/************** Functions **************/
function printUsage(progname)
{
	var outputStr = "Usage: " + progname + " [AT command]\n\n" +
	"If an argument is supplied on the command line, that argument is\n" +
	"sent to the module and the response is printed out.\n\n" +
	"If no argument is used, then the address and PIN of the module\n" +
	"are queried and the results printed out.\n\n"
	console.log(outputStr);
}

// simple helper function to send a command and wait for a response
function sendCommand(bleObj, cmd, callback)
{
	var bleBuffer = new ble.charArray(bufferLength);
	bleObj.writeData(cmd, cmd.length);

	// wait up to 1 second
	if (bleObj.dataAvailable(1000))
    {
		bleObj.readData(bleBuffer, bufferLength);
		var bleData = "";
		// read only the number of characters
		// specified by myGPSSensor.readData
		for (var x = 0; x < bufferLength; x++)
		{
			if (bleBuffer.getitem(x) == '\0')
				break;
			else
				bleData += bleBuffer.getitem(x);
		}
		console.log(bleData);
    }
	else
		console.log("Timed out waiting for response");
	if (callback)
		callback();
}

/************** Main code **************/
// Instantiate a HM11 BLE Module on UART 0
var my_ble_obj = new ble.HM11(0);

// make sure port is initialized properly. 9600 baud is the default.
if (!my_ble_obj.setupTty(ble.int_B9600))
{
	console.log("Failed to setup tty port parameters");
	process.exit(0);
}

printUsage(process.argv[1]);

// Note: in nodeJS, command-line argument 0 is "node".
// Command-line argument 1 is "hm11.js"
// If you have a third argument, then it's a command for BLE
if (process.argv.length > 2)
{
	console.log("Sending command line argument (" + process.argv[2] + ")...");
	sendCommand(my_ble_obj, process.argv[2]);
}
else
{
	// query the module address
	var addr = "AT+ADDR?";
	console.log("Querying module address (" + addr + ")...");

	// sending this command as a synchronous callback ensures better timing
	var callbackFunc = function()
	{
		setTimeout(function()
		{
			// query the module address
			var pin = "AT+PASS?";
			console.log("Querying module PIN (" + pin + ")...");
			sendCommand(my_ble_obj, pin);

			// Other potentially useful commands are:
			//
			// AT+VERS? - query module version
			// AT+ROLE0 - set as slave
			// AT+ROLE1 - set as master
			// AT+CLEAR - clear all previous settings
			// AT+RESET - restart the device
			//
			// A comprehensive list is available from the datasheet at:
			// http://www.seeedstudio.com/wiki/images/c/cd/Bluetooth4_en.pdf
		}, 1000);
	};
	sendCommand(my_ble_obj, addr, callbackFunc);
}


/************** Exit code **************/
process.on('SIGINT', function()
{
	my_ble_obj = null;
	ble.cleanUp();
	ble = null;
	console.log("Exiting...");
	process.exit(0);
});
