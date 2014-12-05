/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */

//Load RTC Clock module
var ds1307 = require('jsupm_ds1307');
// load this on i2c
var myRTCClockObj = new ds1307.DS1307(0);
console.log("Loading the current time... ");

var result = myRTCClockObj.loadTime();
if (!result)
{
	console.log("myRTCClockObj.loadTime() failed.");
	process.exit(1);
}

printTime(myRTCClockObj);

myRTCClockObj.year = 50;
myRTCClockObj.setTime();
myRTCClockObj.loadTime();

printTime(myRTCClockObj);
for (var x in myRTCClockObj)
	console.log(x);

function printTime(RTCObj)
{
	var timeStr = "The time is: " + RTCObj.month + "/" + RTCObj.dayOfMonth + "/" + RTCObj.year + " " + RTCObj.hours + ":" + RTCObj.minutes + ":" + RTCObj.seconds;

	if (RTCObj.amPmMode)
		timeStr += (RTCObj.pm ? " PM " : " AM ");

	console.log(timeStr);

	console.log("Clock is in " + (RTCObj.amPmMode ? "AM/PM mode" : "24hr mode"));
}

