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
var fingerprint_lib = require('jsupm_zfm20');

// Instantiate a ZFM20 Fingerprint reader on UART 0
var myFingerprintSensor = new fingerprint_lib.ZFM20(0);

// make sure port is initialized properly.  57600 baud is the default.
if (!myFingerprintSensor.setupTty(fingerprint_lib.int_B57600))
{
	console.log("Failed to setup tty port parameters");
	process.exit(1);
}

// This example demonstrates registering a fingerprint on the zfm20
// module.  The procedure is as follows:
//
// 1. get an image, store it in characteristics buffer 1
// 2. get another image, store it in characteristics buffer 2
// 3. store the image, assuming the two fingerprints match

// first, we need to register our address and password
myFingerprintSensor.setPassword(fingerprint_lib.ZFM20_DEFAULT_PASSWORD);
myFingerprintSensor.setAddress(fingerprint_lib.ZFM20_DEFAULT_ADDRESS);

// now verify the password.  If this fails, any other commands
// will be ignored, so we just bail.
if (myFingerprintSensor.verifyPassword())
	console.log("Password verified.");
else
{
	console.log("Password verification failed.");
	process.exit(1);
}

console.log(" ");

// get the first image
console.log("Place a finger on the sensor.");
while (myFingerprintSensor.generateImage() != fingerprint_lib.ZFM20.ERR_OK)
	;

// in theory, we have an image
console.log("Image captured, converting...");

var rv = myFingerprintSensor.image2Tz(1);

if (rv != fingerprint_lib.ZFM20.ERR_OK)
{
	console.log("Image conversion failed with error code " + rv);
	process.exit(1)
}

console.log("Image conversion succeeded, remove finger.");
setTimeout(function()
{
	while (myFingerprintSensor.generateImage() != fingerprint_lib.ZFM20.ERR_NO_FINGER)
		;

	console.log(" ");
	console.log("Now place the same finger on the sensor.");

	while (myFingerprintSensor.generateImage() == fingerprint_lib.ZFM20.ERR_NO_FINGER)
		;

	console.log("Image captured, converting...");

	// save this one in slot 2
	rv = myFingerprintSensor.image2Tz(2)
	if (rv != fingerprint_lib.ZFM20.ERR_OK)
	{
		console.log("Image conversion failed with error code %d" + rv);
		process.exit(1);
	}

	console.log("Image conversion succeeded, remove finger.");
	console.log(" ");

	console.log("Storing fingerprint at id 1");

	// create the model
	rv = myFingerprintSensor.createModel()
	if (rv != fingerprint_lib.ZFM20.ERR_OK)
	{
		if (rv == fingerprint_lib.ZFM20.ERR_FP_ENROLLMISMATCH)
			console.log("Fingerprints did not match.");
		else
			console.log("createModel failed with error code " + rv);
		process.exit(1);
	}

	// now store it, we hard code the id (second arg) to 1 here
	rv = myFingerprintSensor.storeModel(1, 1);
	if (rv != fingerprint_lib.ZFM20.ERR_OK)
	{
		console.log("storeModel failed with error code " + rv);
		process.exit(1);
	}

	console.log(" ");
	console.log("Fingerprint stored at id 1.");
}, 1000);

// Print message when exiting
function exit()
{
	myFingerprintSensor = null;
	fingerprint_lib.cleanUp();
	fingerprint_lib = null;
	console.log("Exiting");
	process.exit(0);
}
process.on('exit', exit);
process.on('SIGINT', exit);
