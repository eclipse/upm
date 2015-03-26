#!/usr/bin/python

# Author: Brendan Le Foll <brendan.le.foll@intel.com>
# Contributions: Zion Orent <zorent@ics.com>
# Copyright (c) 2014 Intel Corporation.
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
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE

import time, sys, signal, atexit
import pyupm_lsm303 as lsm303

# Instantiate LSM303 compass on I2C
myAccelrCompass = lsm303.LSM303(0)


## Exit handlers ##
# This stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This lets you run code on exit,
# including functions from myAccelrCompass
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


while(1):
	# Load coordinates into LSM303 object
	successFail = myAccelrCompass.getCoordinates()
	# in XYZ order. The sensor returns XZY,
	# but the driver compensates and makes it XYZ
	coords = myAccelrCompass.getRawCoorData()

	# Print out the X, Y, and Z coordinate data
	# using two different methods
	outputStr = "coor: rX {0} - rY {1} - rZ {2}".format(
	coords.__getitem__(0), coords.__getitem__(1),
	coords.__getitem__(2))
	print outputStr

	outputStr = "coor: gX {0} - gY {1} - gZ {2}".format(
	myAccelrCompass.getCoorX(), myAccelrCompass.getCoorY(),
	myAccelrCompass.getCoorZ())
	print outputStr

	# Get and print out the heading
	print "heading:", myAccelrCompass.getHeading()

	# Get the acceleration
	myAccelrCompass.getAcceleration();
	accel = myAccelrCompass.getRawAccelData();

	# Print out the X, Y, and Z acceleration data
	# using two different methods
	outputStr = "acc: rX {0} - rY {1} - Z {2}".format(
	accel.__getitem__(0), accel.__getitem__(1), accel.__getitem__(2))
	print outputStr

	outputStr = "acc: gX {0} - gY {1} - gZ {2}".format(
	myAccelrCompass.getAccelX(), myAccelrCompass.getAccelY(),
	myAccelrCompass.getAccelZ())
	print outputStr

	print " "
	time.sleep(1)
