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
import pyupm_gp2y0a as upmGp2y0a

# Note, for the Grove 80cm version of this sensor, due to the way it is wired,
# you need to plug this into the A0 port, where it will use the available
# A1 pin for data.
# Instantiate a GP2Y0A on analog pin A1
myIRProximity = upmGp2y0a.GP2Y0A(1)


## Exit handlers ##
# This stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This lets you run code on exit,
# including functions from myIRProximity
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


# analog voltage, usually 3.3 or 5.0
GP2Y0A_AREF = 5.0;
SAMPLES_PER_QUERY = 20;

# The higher the voltage (closer to AREF) the closer the object is.
# NOTE: The measured voltage will probably not exceed 3.3 volts.
# Every second, print the averaged voltage value
# (averaged over 20 samples).
while (1):
	print "AREF: {0}, Voltage value (higher means closer): {1}".format(
	GP2Y0A_AREF,
	myIRProximity.value(GP2Y0A_AREF, SAMPLES_PER_QUERY))
	time.sleep(1)
