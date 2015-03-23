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
import pyupm_at42qt1070 as upmAt42qt1070

# functions
def printButtons(touchObj):
	buttonPressed = False
	buttons = touchObj.getButtons()

	sys.stdout.write("Buttons Pressed: ")
	for i in range(7):
		if (buttons & (1 << i)):
			sys.stdout.write(str(i) + " ")
			buttonPressed = True

	if (not buttonPressed):
		sys.stdout.write("None")

	print " "

	if (touchObj.isCalibrating()):
		print "Calibration is occurring."

	if (touchObj.isOverflowed()):
		print "Overflow was detected."


# Global code that runs on startup

I2C_BUS = upmAt42qt1070.AT42QT1070_I2C_BUS
DEFAULT_I2C_ADDR = upmAt42qt1070.AT42QT1070_DEFAULT_I2C_ADDR

# Instantiate an AT42QT1070 on I2C bus 0
myTouchSensor = upmAt42qt1070.AT42QT1070(I2C_BUS,
                                         DEFAULT_I2C_ADDR)


# Exit handlers
def SIGINTHandler(signum, frame):
	raise SystemExit

def exitHandler():
	print "Exiting"
	sys.exit(0)


# This function lets you run code on exit, including functions from myTouchSensor
atexit.register(exitHandler)
# This function stops python from printing a stacktrace when you hit control-C
signal.signal(signal.SIGINT, SIGINTHandler)


# Print the button being touched every 0.1 seconds
while(1):
	myTouchSensor.updateState()
	printButtons(myTouchSensor)
	time.sleep(.1)
