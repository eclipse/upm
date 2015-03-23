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
import pyupm_grovewfs as upmGrovewfs

# Instantiate a Grove Water Flow Sensor on digital pin D2
myWaterFlow = upmGrovewfs.GroveWFS(2)


## Exit handlers ##
# This stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit,
# including functions from myWaterFlow
def exitHandler():
	myWaterFlow.stopFlowCounter()
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


# set the flow counter to 0 and start counting
myWaterFlow.clearFlowCounter()
myWaterFlow.startFlowCounter()

while (1):
	# we grab these (millis and flowCount) just for display
	# purposes in this example
	millis = myWaterFlow.getMillis()
	flowCount = myWaterFlow.flowCounter()

	fr = myWaterFlow.flowRate()

	# output milliseconds passed, flow count, and computed flow rate
	outputStr = "Millis: {0} Flow Count: {1} Flow Rate: {2} LPM".format(
	millis, flowCount, fr)
	print outputStr
	time.sleep(2)
