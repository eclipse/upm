#!/usr/bin/python
# Author:  Jon Trulson <jtrulson@ics.com>
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
import pyupm_uln200xa as upmULN200XA

# Instantiate a Stepper motor on a ULN200XA Darlington Motor Driver
# This was tested with the Grove Geared Step Motor with Driver

# Instantiate a ULN2003XA stepper object
myUln200xa = upmULN200XA.ULN200XA(4096, 8, 9, 10, 11)

## Exit handlers ##
# This stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This lets you run code on exit,
# including functions from myUln200xa
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


myUln200xa.setSpeed(5) # 5 RPMs
myUln200xa.setDirection(upmULN200XA.ULN200XA.DIR_CW)

print "Rotating 1 revolution clockwise."
myUln200xa.stepperSteps(4096)

print "Sleeping for 2 seconds..."
time.sleep(2)

print "Rotating 1/2 revolution counter clockwise."
myUln200xa.setDirection(upmULN200XA.ULN200XA.DIR_CCW)
myUln200xa.stepperSteps(2048)

# release
myUln200xa.release()

# exitHandler is called automatically
