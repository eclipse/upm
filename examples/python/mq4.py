#!/usr/bin/python
# Author: Jon Trulson <jtrulson@ics.com>
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
import pyupm_gas as upmGAS

# Attach gas sensor to Analog A0
sensor = upmGAS.MQ4(0)


## Exit handlers ##
# This function stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit, including functions from sensor
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


threshContext = upmGAS.thresholdContext()
threshContext.averageReading = 0
threshContext.runningAverage = 0
threshContext.averagedOver = 2

# Infinite loop, ends when script is cancelled
# Repeatedly, take a sample every 2 microseconds;
# find the average of 128 samples; and
# print a running graph of asteriskss as averages
mybuffer = upmGAS.uint16Array(128)
while(1):
	samplelen = sensor.getSampledWindow(2, 128, mybuffer)
	if samplelen:
		thresh = sensor.findThreshold(threshContext, 30, mybuffer, samplelen)
		sensor.printGraph(threshContext, 5)
#		if(thresh):
#			print "Threshold is ", thresh
