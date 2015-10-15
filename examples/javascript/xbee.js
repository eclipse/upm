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


var sensorObj = require('jsupm_xbee');


/************** Functions **************/
function printUsage(progname)
{
    var outputStr = "Usage: " + progname + " [AT command]\n\n" +
	"If an argument is supplied on the command line, that argument is\n" +
	"sent to the module and the response is printed out.\n\n" +
	"If no argument is used, then the firmware revision, serial number\n" +
	"and the current IP address (if set) are queried.\n\n"
    console.log(outputStr);
}

// simple helper function to send a command and wait for a response
function sendCommand(sensor, cmd)
{
    // commands need to be terminated with a carriage return
    cmd += "\r";
    sensor.writeDataStr(cmd);

    var resp = "";
    while (sensor.dataAvailable(2000))
    {
        resp += sensor.readDataStr(1024);
    }

    if (!resp) 
    {
        console.log("Timed out waiting for response");
    }
    else
    {
        resp = sensor.stringCR2LF(resp);
        console.log("Returned (" + resp.length + " bytes):");
        console.log(resp);
    }
}

/************** Main code **************/
// Instantiate a XBEE Module on UART 0
var sensor = new sensorObj.XBee(0);

// Set the baud rate, 9600 baud is the default.
if (sensor.setBaudRate(9600))
{
    console.log("Failed to set baud rate");
    process.exit(0);
}

printUsage(process.argv[1]);

// Note: in nodeJS, command-line argument 0 is "node".
// Command-line argument 1 is "xbee.js"
// If you have a third argument, then it's a command
if (process.argv.length > 2)
{
    // enable command mode
    sensor.commandMode();
    console.log("Sending command line argument (" + process.argv[2] + ")...");
    sendCommand(sensor, process.argv[2]);
}
else
{
    // enable command mode
    sensor.commandMode();
    // query the verbose firmware revision
    console.log("Querying verbose firmware revision (ATVL)...");
    sendCommand(sensor, "ATVL");
    // query the number
    console.log("Querying Serial Number High (ATSH)...");
    sendCommand(sensor, "ATSH");
    console.log("Querying Serial Number Low (ATSL)...");
    sendCommand(sensor, "ATSL");
    
    console.log("Querying address, if set (ATMY)...");
    sendCommand(sensor, "ATMY");
    
    // For the XBee WiFi S6B
    // A comprehensive list of commands and command modes is
    // available from the datasheet at:
    // ftp1.digi.com/support/documentation/90002180_L.pdf
    
    // For the XBee S1
    // A comprehensive list of commands and command modes is
    // available from the datasheet at:
    // http://www.sparkfun.com/datasheets/Wireless/Zigbee/XBee-Datasheet.pdf
    
    // For the XBee WiFi module:
    // An example using AT commands to connect to an AP, with a
    // private Key using WPA2:
    
    // Connect to AP with SSID 'mySSID':
    //  ATIDmySSID
    
    // Provide the private key 'secret':
    //  ATPKsecret
    
    // Use WPA2 encryption
    //  ATEE2
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
