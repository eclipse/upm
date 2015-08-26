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

import time, sys, atexit
import pyupm_isd1820 as upmIsd1820

# Instantiate a ISD1820 on digital pins 2 (play) and 3 (record)
# This example was tested on the Grove Recorder.
myRecorder = upmIsd1820.ISD1820(2, 3)


doRecord = False

if len(sys.argv) > 1:
	doRecord = True


# This lets you run code on exit,
# including functions from myRecorder
def exitHandler():
	# turn off whatever we were doing.
	if (doRecord):
		myRecorder.record(False)
	else:
		myRecorder.play(False)
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)


# if an argument was specified (any argument), go into record mode,
# else playback a previously recorded sample

print "Supply any argument to the command line to record."
print "Running this example without arguments will play back any "
print "previously recorded sound."
print "There is approximately 10 seconds of recording time.\n"


# depending on what was selected, do it, and sleep for 15 seconds
if (doRecord):
	myRecorder.record(True)
else:
	myRecorder.play(True)
    
# There are about 10 seconds of recording/playback time, so we will
# sleep for a little extra time.
print "Sleeping for 15 seconds..."
time.sleep(15)


# exitHandler runs automatically
