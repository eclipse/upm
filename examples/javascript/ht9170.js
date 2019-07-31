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
var ht9170_dtmf_lib = require("jsupm_ht9170");

// Instantiate a DTMF decoder
var dtmf_obj = new ht9170_dtmf_lib.HT9170(12, 11, 10, 9, 8);

// Now we just spin in a loop, sleeping every 100ms, checking to see
// if a digit is available.  If so, we decode and print the digit,
// and continue looping.
var myInterval = setInterval(function()
{
	if (dtmf_obj.digitReady())
	{
		console.log("Got DTMF code: " + dtmf_obj.decodeDigit());
		// now spin until digitReady() goes false again
		while (dtmf.digitReady())
			;
	}
}, 100);

// When exiting: clear interval, and print message
process.on('SIGINT', function()
{
  clearInterval(myInterval);
	dtmf_obj = null;
	ht9170_dtmf_lib.cleanUp();
	ht9170_dtmf_lib = null;
	console.log("Exiting");
	process.exit(0);
});
