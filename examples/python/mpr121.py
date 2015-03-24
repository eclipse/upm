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
import pyupm_mpr121 as upmMpr121

I2C_BUS = upmMpr121.MPR121_I2C_BUS
DEFAULT_I2C_ADDR = upmMpr121.MPR121_DEFAULT_I2C_ADDR

# Instantiate an MPR121 touch sensor on I2C
myTouchSensor = upmMpr121.MPR121(I2C_BUS, DEFAULT_I2C_ADDR)


## Exit handlers ##
# This stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit,
# including functions from myTouchSensor
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)



def printButtons(touchSensor):
	buttonPressed = False

	outputStr = "Buttons Pressed: "
	for i in range(12):
		if (touchSensor.m_buttonStates & (1 << i)):
			outputStr += (str(i) + " ")
			buttonPressed = True

	if (not buttonPressed):
		outputStr += "None"

	print outputStr

	if (touchSensor.m_overCurrentFault):
		print "Over Current Fault detected!"


while(1):
	myTouchSensor.readButtons()
	printButtons(myTouchSensor)

	time.sleep(1)
