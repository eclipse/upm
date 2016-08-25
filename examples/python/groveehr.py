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
import pyupm_groveehr as upmGroveehr

# Instantiate a Grove Ear-clip Heart Rate sensor on digital pin D2
myHeartRateSensor = upmGroveehr.GroveEHR(2)


## Exit handlers ##
# This stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This lets you run code on exit,
# including functions from myHeartRateSensor
def exitHandler():
	myHeartRateSensor.stopBeatCounter()
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


# set the beat counter to 0, init the clock and start counting beats
myHeartRateSensor.clearBeatCounter()
myHeartRateSensor.initClock()
myHeartRateSensor.startBeatCounter()

while(1):
	# we grab these (millis and flowCount) just for display
	# purposes in this example
	millis = myHeartRateSensor.getMillis()
	beats = myHeartRateSensor.beatCounter()

	# heartRate() requires that at least 5 seconds pass before
	# returning anything other than 0
	fr = myHeartRateSensor.heartRate()

	# output milliseconds passed, beat count, and computed heart rate
	outputStr = "Millis: {0} Beats: {1} Heart Rate: {2}".format(
	millis, beats, fr)
	print outputStr
	time.sleep(1)
