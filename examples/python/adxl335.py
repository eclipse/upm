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
import pyupm_adxl335 as upmAdxl335

myAnalogAccel = upmAdxl335.ADXL335(0, 1, 2)

print "Please make sure the sensor is completely still."
print "Sleeping for 2 seconds"
time.sleep(2)


## Exit handlers ##
# This stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit,
# including functions from myAnalogAccel
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


print "Calibrating..."
myAnalogAccel.calibrate()

x = upmAdxl335.new_intPointer()
y = upmAdxl335.new_intPointer()
z = upmAdxl335.new_intPointer()

aX = upmAdxl335.new_floatPointer()
aY = upmAdxl335.new_floatPointer()
aZ = upmAdxl335.new_floatPointer()

while (1):
	myAnalogAccel.values(x, y, z)
	outputStr = "Raw Values: X: {0} Y: {1} Z: {2}".format(
	upmAdxl335.intPointer_value(x), upmAdxl335.intPointer_value(y),
	upmAdxl335.intPointer_value(z))
	print outputStr

	myAnalogAccel.acceleration(aX, aY, aZ)
	outputStr = ("Acceleration: X: {0}g\n"
	"Acceleration: Y: {1}g\n"
	"Acceleration: Z: {2}g").format(upmAdxl335.floatPointer_value(aX),
	upmAdxl335.floatPointer_value(aY),
	upmAdxl335.floatPointer_value(aZ))
	print outputStr

	print " "

	time.sleep(.2)
