#!/usr/bin/python
#
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
import pyupm_grovecircularled as upmGroveCircularLED

# Exit handlers
def SIGINTHandler(signum, frame):
	raise SystemExit

def exitHandler():
	circle.setLevel(0, True)
	print "Exiting"
	sys.exit(0)

# This function lets you run code on exit
atexit.register(exitHandler)
# This function stops python from printing a stacktrace when you hit control-C
signal.signal(signal.SIGINT, SIGINTHandler)

# Instantiate a Grove Circular LED on gpio pins 5 and 4
circle = upmGroveCircularLED.GroveCircularLED(5, 4)

level = 0

while(1):
        circle.setSpinner(level)
        level = (level + 1) % 24
	time.sleep(.1)
