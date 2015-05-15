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
import pyupm_nunchuck as upmNunchuck

# Instantiate a nunchuck controller bus 0 on I2C
myNunchuck = upmNunchuck.NUNCHUCK(0)


## Exit handlers ##
# This function stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit, including functions from myNunchuck
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


# always do this first
print "Initializing... "
if (not myNunchuck.init()):
	print "nunchuck->init() failed."
	sys.exit(0);

def buttonStateStr(buttonState):
	return "pressed" if buttonState else "not pressed"

# Print the X and Y input values every second
while(1):
	myNunchuck.update()

	outputStr = "stickX: {0}, stickY: {1}".format(
	myNunchuck.stickX, myNunchuck.stickY)
	print outputStr
	outputStr = "accelX: {0}, accelY: {1}, accelZ: {2}".format(
	myNunchuck.accelX, myNunchuck.accelY, myNunchuck.accelZ)
	print outputStr

	outputStr = "button C: {0}".format(
	buttonStateStr(myNunchuck.buttonC))
	print outputStr
	outputStr = "button Z: {0}".format(
	buttonStateStr(myNunchuck.buttonZ))
	print outputStr

	time.sleep(.1)
