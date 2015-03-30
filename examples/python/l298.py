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
import pyupm_l298 as upmL298

# Instantiate one of the 2 possible DC motors on a L298 Dual
# H-Bridge.  For controlling a stepper motor, see the l298-stepper
# example.
myHBridge = upmL298.L298(3, 4, 7)


## Exit handlers ##
# This stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This lets you run code on exit,
# including functions from myHBridge
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


print "Starting motor at 50% for 3 seconds..."
myHBridge.setSpeed(50)
myHBridge.setDirection(upmL298.L298.DIR_CW)
myHBridge.enable(True)

time.sleep(3)

print "Reversing direction..."
myHBridge.setDirection(upmL298.L298.DIR_NONE) # fast stop
myHBridge.setDirection(upmL298.L298.DIR_CCW)
time.sleep(3);

myHBridge.setSpeed(0)
myHBridge.enable(False)

# exitHandler is called automatically
