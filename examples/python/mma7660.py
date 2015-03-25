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
import pyupm_mma7660 as upmMMA7660

# Instantiate an MMA7660 on I2C bus 0
myDigitalAccelerometer = upmMMA7660.MMA7660(
					upmMMA7660.MMA7660_I2C_BUS, 
					upmMMA7660.MMA7660_DEFAULT_I2C_ADDR);


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


# place device in standby mode so we can write registers
myDigitalAccelerometer.setModeStandby()

# enable 64 samples per second
myDigitalAccelerometer.setSampleRate(upmMMA7660.MMA7660.AUTOSLEEP_64)

# place device into active mode
myDigitalAccelerometer.setModeActive()

x = upmMMA7660.new_intp()
y = upmMMA7660.new_intp()
z = upmMMA7660.new_intp()

ax = upmMMA7660.new_floatp()
ay = upmMMA7660.new_floatp()
az = upmMMA7660.new_floatp()

while (1):
	myDigitalAccelerometer.getRawValues(x, y, z)
	outputStr = ("Raw values: x = {0}"
	" y = {1}" 
	" z = {2}").format(upmMMA7660.intp_value(x),
	upmMMA7660.intp_value(y),
	upmMMA7660.intp_value(z))
	print outputStr

	myDigitalAccelerometer.getAcceleration(ax, ay, az)
	outputStr = ("Acceleration: x = {0}"
	"g y = {1}"
	"g z = {2}g").format(upmMMA7660.floatp_value(ax),
	upmMMA7660.floatp_value(ay),
	upmMMA7660.floatp_value(az))
	print outputStr
	time.sleep(.5)
