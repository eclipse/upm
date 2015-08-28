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


var sensorObj = require('jsupm_grovegprs');


/************** Functions **************/
function printUsage(progname)
{
    var outputStr = "Usage: " + progname + " [AT command]\n\n" +
	"If an argument is supplied on the command line, that argument is\n" +
	"sent to the module and the response is printed out.\n\n" +
	"If no argument is used, then the manufacturer and the current\n" +
	"saved profiles are queried and the results printed out.\n\n"
    console.log(outputStr);
}

// simple helper function to send a command and wait for a response
function sendCommand(sensor, cmd, callback)
{
    // commands need to be terminated with a carriage return
    cmd += "\r";
    sensor.writeDataStr(cmd);

    // wait up to 1 second
    if (sensor.dataAvailable(1000))
    {
	console.log("Returned: " + sensor.readDataStr(1024));
    }
    else
	console.log("Timed out waiting for response");

    if (callback)
	callback();
}

/************** Main code **************/
// Instantiate a GROVEGPRS Module on UART 0
var sensor = new sensorObj.GroveGPRS(0);

// Set the baud rate, 19200 baud is the default.
if (sensor.setBaudRate(19200))
{
    console.log("Failed to set baud rate");
    process.exit(0);
}

printUsage(process.argv[1]);

// Note: in nodeJS, command-line argument 0 is "node".
// Command-line argument 1 is "grovegprs.js"
// If you have a third argument, then it's a command
if (process.argv.length > 2)
{
    console.log("Sending command line argument (" + process.argv[2] + ")...");
    sendCommand(sensor, process.argv[2]);
}
else
{
    // sending this command as a synchronous callback ensures better timing
    var callbackFunc = function()
    {
	setTimeout(function()
        {
            // query the saved profiles
            console.log("Querying the saved profiles (AT&V)...");
            sendCommand(sensor, "AT&V");
            
            // A comprehensive list is available from the
            // datasheet at:
            // http://www.seeedstudio.com/wiki/images/7/72/AT_Commands_v1.11.pdf
	}, 1000);
    };

    // query the module manufacturer
    console.log("Querying module manufacturer (AT+CGMI)...");
    sendCommand(sensor, "AT+CGMI", callbackFunc);
}


/************** Exit code **************/
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting...");
    process.exit(0);
});
