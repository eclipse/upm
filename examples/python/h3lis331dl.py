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
import pyupm_h3lis331dl as upmH3LIS331DL

# Instantiate an H3LIS331DL on I2C bus 0
myDigitalAccelerometer = upmH3LIS331DL.H3LIS331DL(
        upmH3LIS331DL.H3LIS331DL_I2C_BUS, 
        upmH3LIS331DL.H3LIS331DL_DEFAULT_I2C_ADDR);


## Exit handlers ##
# This function stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit, including functions from myDigitalAccelerometer
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


# Initialize the device with default values
myDigitalAccelerometer.init()

x = upmH3LIS331DL.new_intp()
y = upmH3LIS331DL.new_intp()
z = upmH3LIS331DL.new_intp()

ax = upmH3LIS331DL.new_floatp()
ay = upmH3LIS331DL.new_floatp()
az = upmH3LIS331DL.new_floatp()

while (1):
	myDigitalAccelerometer.update()
	myDigitalAccelerometer.getRawXYZ(x, y, z)
	outputStr = ("Raw: X = {0}"
	" Y = {1}" 
	" Z = {2}").format(upmH3LIS331DL.intp_value(x),
	upmH3LIS331DL.intp_value(y),
	upmH3LIS331DL.intp_value(z))
	print outputStr

	myDigitalAccelerometer.getAcceleration(ax, ay, az)
	outputStr = ("Acceleration: AX = {0}"
	" AY = {1}"
	" AZ = {2}").format(upmH3LIS331DL.floatp_value(ax),
	upmH3LIS331DL.floatp_value(ay),
	upmH3LIS331DL.floatp_value(az))
	print outputStr
	time.sleep(.5)
