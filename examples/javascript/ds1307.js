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

