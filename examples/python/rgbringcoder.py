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
import pyupm_rgbringcoder as upmRGBRingCoder

# There are a lot of pins to hook up.  These pins are valid for the
# Edison board, but may need to be adjusted for other platforms.

# In order:
# enable      - 4
# latch       - 10
# clear       - 11
# clock       - 2
# data        - 9
# switch      - 7

# red pwm     - 3
# green pwm   - 5
# blue pwm    - 6

# encA        - 12
# encB        - 13
ringCoder = upmRGBRingCoder.RGBRingCoder(4, 10, 11, 2, 9, 7, 12, 13, 3, 
                                         5, 6)

## Exit handlers ##
# This stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit,
# including functions from ringCoder
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


spin = 0x0001;
oldState = False;
oldPos = 0;

# Lets go green
ringCoder.setRGBLED(0.99, 0.01, 0.99);

while(1):
        # you spin me round...
        if ((spin & 0xffff) == 0):
                spin = 0x0001
    
        ringCoder.setRingLEDS(spin)
        spin <<= 1

        # check button state
        bstate = ringCoder.getButtonState()
        if (bstate != oldState):
                print "Button state changed from", oldState, " to ", bstate
                oldState = bstate
    
        # check encoder position
        epos = ringCoder.getEncoderPosition()
        if (epos != oldPos):
                print "Encoder position changed from", oldPos, "to", epos
                oldPos = epos

        time.sleep(0.1)
