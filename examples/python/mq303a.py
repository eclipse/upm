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

# Load alcohol sensor module
import pyupm_mq303a as upmMq303a

# Instantiate an mq303a sensor on analog pin A0
# This device uses a heater powered from an analog I/O pin. 
# If using A0 as the data pin, then you need to use A1, as the heater
# pin (if using a grove mq303a).  For A1, we can use the D15 gpio, 
# setup as an output, and drive it low to power the heater.
myAlcoholSensor = upmMq303a.MQ303A(0, 15)


## Exit handlers ##
# This function stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit, including functions from myAlcoholSensor
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


print "Enabling heater and waiting 2 minutes for warmup."

# give time updates every 30 seconds until 2 minutes have passed
# for the alcohol sensor to warm up
def warmup(iteration):
	totalSeconds = (30 * iteration)
	time.sleep(30)
	print totalSeconds, "seconds have passed"
warmup(1)
warmup(2)
warmup(3)
warmup(4)

notice = ("This sensor may need to warm "
"until the value drops below about 450.")
print notice

# Print the detected alcohol value every second
while(1):
	val = myAlcoholSensor.value()
	msg = "Alcohol detected "
	msg += "(higher means stronger alcohol): "
	print msg + str(val)
	time.sleep(1)
