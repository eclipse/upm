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
import pyupm_zfm20 as upmZfm20

# Instantiate a ZFM20 Fingerprint reader on UART 0
myFingerprintSensor = upmZfm20.ZFM20(0)


## Exit handlers ##
# This stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit,
# including functions from myFingerprintSensor
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


# make sure port is initialized properly.  57600 baud is the default.
if (not myFingerprintSensor.setupTty(upmZfm20.cvar.int_B57600)):
	print "Failed to setup tty port parameters"
	sys.exit(1)

# how many valid stored templates (fingerprints) do we have?
print "Total stored templates: %d" % myFingerprintSensor.getNumTemplates()
print " "

# now spin waiting for a fingerprint to successfully image
print "Waiting for finger print..."

while (myFingerprintSensor.generateImage() == upmZfm20.ZFM20.ERR_NO_FINGER):
	pass

# in theory, we have an image
print "Image captured, converting..."

rv = myFingerprintSensor.image2Tz(1)
if (rv != upmZfm20.ZFM20.ERR_OK):
	print "Image conversion failed with error code %d" % rv
	sys.exit(1)

print "Image conversion succeeded."
print "Searching database..."

myid = upmZfm20.uint16Array(0)
myid.__setitem__(0, 0)
myscore = upmZfm20.uint16Array(0)
myscore.__setitem__(0, 0)

# we search for a print matching slot 1, where we stored our last
# converted fingerprint
rv = myFingerprintSensor.search(1, myid, myscore)
if (rv != upmZfm20.ZFM20.ERR_OK):
	if (rv == upmZfm20.ZFM20.ERR_FP_NOTFOUND):
		print "Finger Print not found"
		sys.exit(0)
	else:
		print "Search failed with error code %d" % rv
		sys.exit(1)

print "Fingerprint found!"
print "ID: %d, Score: %d" % (myid.__getitem__(0), myscore.__getitem__(0))
