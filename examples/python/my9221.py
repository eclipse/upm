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
import pyupm_my9221 as upmMy9221

# Instantiate a MY9221, we use D2 for the data, and D3 for the
# data clock.  This was tested with a Grove LED bar.
myLEDBar = upmMy9221.MY9221(2, 3)


# Exit handlers
def SIGINTHandler(signum, frame):
	raise SystemExit

def exitHandler():
	myLEDBar.setBarLevel(0, True)
	print "Exiting"
	sys.exit(0)

# This function lets you run code on exit
atexit.register(exitHandler)
# This function stops python from printing a stacktrace when you hit control-C
signal.signal(signal.SIGINT, SIGINTHandler)


directionBool = True
level = 1

x = 0
while(1):
	# If it's less than 10
	# light up the LED now
	# call show_LED again in 50 ms
	if (level <= 10):
		myLEDBar.setBarLevel(level, directionBool)
		level += 1
	# Switch LED lighting directions between lighting cycles
	else:
		directionBool = not directionBool
		level = 1
		time.sleep(1)
	time.sleep(.05)
	x += 1
