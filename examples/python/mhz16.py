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
import pyupm_mhz16 as upmMhz16

# Instantiate a MHZ16 serial CO2 sensor on uart 0.
# This example was tested on the Grove CO2 sensor module.
myCO2 = upmMhz16.MHZ16(0)


## Exit handlers ##
# This stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit,
# including functions from myCO2
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


# make sure port is initialized properly.  9600 baud is the default.
if (not myCO2.setupTty(upmMhz16.cvar.int_B9600)):
	print "Failed to setup tty port parameters"
	sys.exit(0)

print ("Make sure that the sensor has had "
"at least 3 minutes to warm up\n"
"or you will not get valid results.\n"
"The temperature reported is not the ambient temperature,\n"
"but rather the temperature of the sensor elements.")

gas = upmMhz16.new_intp()
temp = upmMhz16.new_intp()

time.sleep(1)

while(1):
	myCO2.getData(gas, temp)
	outputStr = ("CO2 concentration: {0} PPM, "
	"Temperature (in C): {1}".format(
	upmMhz16.intp_value(gas), upmMhz16.intp_value(temp)))
	print outputStr

	time.sleep(2)
