#!/usr/bin/python
# Author: Zion Orent <zorent@ics.com>
# Copyright (c) 2015 Intel Corporation.
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

import time, sys, signal, atexit
import pyupm_ds1307 as upmDs1307

# load RTC clock on i2c bus 0
myRTCClock = upmDs1307.DS1307(0)

def printTime(RTCObj):
	timeStr = "The time is: {0}/{1}/{2} {3}:{4}:{5}".format(
	RTCObj.month, RTCObj.dayOfMonth, RTCObj.year,
	RTCObj.hours, RTCObj.minutes, RTCObj.seconds)

	if (RTCObj.amPmMode):
		timeStr += (" PM " if RTCObj.pm else " AM ")

	print timeStr

	print "Clock is in", ("AM/PM mode"
	if RTCObj.amPmMode else "24hr mode")


# always do this first
print "Loading the current time... "
result = myRTCClock.loadTime()
if (not result):
	print "myRTCClock.loadTime() failed."
	sys.exit(0)

printTime(myRTCClock);

# set the year as an example
print "setting the year to 50"
myRTCClock.year = 50
myRTCClock.setTime()

# reload the time and print it
myRTCClock.loadTime()
printTime(myRTCClock)
