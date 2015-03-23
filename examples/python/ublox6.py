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
import pyupm_ublox6 as upmUblox6

# Instantiate a Ublox6 GPS device on uart 0.
myGPSSensor = upmUblox6.Ublox6(0)


## Exit handlers ##
# This function stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit, including functions from myGPSSensor
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


if (not myGPSSensor.setupTty(upmUblox6.cvar.int_B9600)):
	print "Failed to setup tty port parameters"
	sys.exit(0)

# Collect and output NMEA data.

# This device also supports numerous configuration options, which
# you can set with writeData().  Please refer to the Ublox-6 data
# sheet for further information on the formats of the data sent and
# received, and the various operating modes available.


bufferLength = 256
nmeaBuffer = upmUblox6.charArray(bufferLength)

def getGPSInfo():
	# we don't want the read to block in this example, so always
	# check to see if data is available first.
	if (myGPSSensor.dataAvailable()):
		rv = myGPSSensor.readData(nmeaBuffer, bufferLength)

		numlines= 0
		if (rv > 0):
			GPSData = ""
			# read only the number of characters
			# specified by myGPSSensor.readData
			for x in range(rv):
				GPSData += nmeaBuffer.__getitem__(x)
			sys.stdout.write(GPSData)

		if (rv < 0): # some sort of read error occured
			print "Port read error."
			sys.exit(0)

while (1):
	getGPSInfo()
	time.sleep(.1)
