/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2015 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/
var fingerprint_lib = require('jsupm_zfm20');

// Instantiate a ZFM20 Fingerprint reader on UART 0
var myFingerprintSensor = new fingerprint_lib.ZFM20(0);

// make sure port is initialized properly.  57600 baud is the default.
if (!myFingerprintSensor.setupTty(fingerprint_lib.int_B57600))
{
	console.log("Failed to setup tty port parameters");
	process.exit(1);
}

// how many valid stored templates (fingerprints) do we have?
console.log("Total stored templates: " + myFingerprintSensor.getNumTemplates());
console.log(" ");

// now spin waiting for a fingerprint to successfully image
console.log("Waiting for finger print...");

while (myFingerprintSensor.generateImage() == fingerprint_lib.ZFM20.ERR_NO_FINGER)
	;

// in theory, we have an image
console.log("Image captured, converting...");

var rv = myFingerprintSensor.image2Tz(1);
if (rv != fingerprint_lib.ZFM20.ERR_OK)
{
	console.log("Image conversion failed with error code " + rv);
	process.exit(1);
}

console.log("Image conversion succeeded.");
console.log("Searching database...");

var myid = new fingerprint_lib.uint16Array(0);
myid.setitem(0, 0);
var myscore = new fingerprint_lib.uint16Array(0);
myscore.setitem(0, 0);

// we search for a print matching slot 1, where we stored our last
// converted fingerprint
rv = myFingerprintSensor.search(1, myid, myscore)
if (rv != fingerprint_lib.ZFM20.ERR_OK)
{
	if (rv == fingerprint_lib.ZFM20.ERR_FP_NOTFOUND)
	{
		console.log("Finger Print not found");
		process.exit(0);
	}
	else
	{
		console.log("Search failed with error code " + rv);
		process.exit(1);
	}
}

console.log("Fingerprint found!");
console.log("ID: " + myid.getitem(0) + ", Score: " + myscore.getitem(0));


// Print message when exiting
function exit()
{
	myFingerprintSensor = null;
	fingerprint_lib.cleanUp();
	fingerprint_lib = null;
	console.log("Exiting");
	process.exit(0);
}
process.on('SIGINT', exit);
process.on('exit', exit);
