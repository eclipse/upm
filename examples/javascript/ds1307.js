/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2014 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

// Load RTC Clock module for Grove - RTC clock
var ds1307 = require('jsupm_ds1307');
// load this on i2c bus 0
var myRTCClockObj = new ds1307.DS1307(0);

// always do this first
console.log("Loading the current time... ");

var result = myRTCClockObj.loadTime();
if (!result)
{
	console.log("myRTCClockObj.loadTime() failed.");
	process.exit(1);
}

printTime(myRTCClockObj);

// set the year as an example
console.log("setting the year to 50");
myRTCClockObj.year = 50;
myRTCClockObj.setTime();

// reload the time and print it
myRTCClockObj.loadTime();
printTime(myRTCClockObj);

function printTime(RTCObj)
{
	var timeStr = "The time is: " + 
        RTCObj.month + "/" + RTCObj.dayOfMonth + "/" + RTCObj.year + " " +
        RTCObj.hours + ":" + RTCObj.minutes + ":" + RTCObj.seconds;

	if (RTCObj.amPmMode)
		timeStr += (RTCObj.pm ? " PM " : " AM ");

	console.log(timeStr);

	console.log("Clock is in " + 
                (RTCObj.amPmMode ? "AM/PM mode" : "24hr mode"));
}

