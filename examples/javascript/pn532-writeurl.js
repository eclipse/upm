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

// Load PN532 module
var pn532 = require('jsupm_pn532');

// Instantiate an PN532 on I2C bus 0 (default) using gpio 3 for the
// IRQ, and gpio 2 for the reset pin.
var myNFCObj = new pn532.PN532(3, 2);

function writeUrl()
{
    if (uidSize.getitem(0) != 7)
    {
        console.log("This example will only write an NDEF URI to preformatted");
        console.log("Mifare Ultralight or NTAG2XX tags");
        exit();
    }

    // 48 bytes is maximum data area on ultralight cards, so we use that
    // as the maximum datasize here.  Obviously if you have a bigger
    // card, you can write more data.
    if (!myNFCObj.ntag2xx_WriteNDEFURI(pn532.PN532.NDEF_URIPREFIX_HTTP, 
                                       url, 48))
    {
        // failure
        console.log("Failed to write NDEF record tag.");
        exit(1);
    }

    console.log("Success, URL record written to tag.");
}

function toHex(d, pad)
{
    // pad should be between 1 and 8
    return  ("00000000"+(Number(d).toString(16))).slice(-pad)
}

function exit()
{
	clearInterval(myInterval);
	myNFCObj = null;
	pn532.cleanUp();
	pn532 = null;
	console.log("Exiting");
	process.exit(0);
}

// When exiting: clear interval, and print message
process.on('SIGINT', function()
{
	exit();
});

// "main"
if (!myNFCObj.init())
	console.log("init() failed");

var vers = myNFCObj.getFirmwareVersion();

if (vers)
	console.log("Got firmware version: " + toHex(vers, 8));
else
{
	console.log("Could not identify PN532");
	exit();
}

// Now scan and identify any cards that come in range (1 for now)

// Retry forever
myNFCObj.setPassiveActivationRetries(0xff);

myNFCObj.SAMConfig();

var uidSize = new pn532.uint8Array(0);
var uid = new pn532.uint8Array(7);

// the URL we want to add as an NDEF record
// NOTE: this cannot exceed 34 characters.
url = "iotdk.intel.com";

var myInterval = setInterval(function()
{
	for (var x = 0; x < 7; x++)
		uid.setitem(x, 0);
	if (myNFCObj.readPassiveTargetID(pn532.PN532.BAUD_MIFARE_ISO14443A,
		                         uid, uidSize, 2000))
	{
		// found a card
		console.log("Found a card: UID len " + uidSize.getitem(0));
		process.stdout.write("UID: ");
		for (var i = 0; i < uidSize.getitem(0); i++)
                {
                        var byteVal = uid.getitem(i);
			process.stdout.write(toHex(byteVal, 2) + " ");
                }
		process.stdout.write("\n");
		console.log("SAK: " + toHex(myNFCObj.getSAK(), 2));
		console.log("ATQA: " + toHex(myNFCObj.getATQA(), 4));
		console.log(" ");

                // write the URL
                writeUrl();
            	clearInterval(myInterval);
                return;
	}
	else
		console.log("Waiting for a card...");
}, 1000);

