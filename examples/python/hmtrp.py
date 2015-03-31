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
import pyupm_hmtrp as upmHmtrp

# Instantiate a HMTRP radio device on uart 0
my_HMTRP_Radio = upmHmtrp.HMTRP(0)


## Exit handlers ##
# This stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit,
# including functions from my_HMTRP_Radio
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


myCounter = 0

# normal read/write mode
bufferLength = 256
radioBuffer = upmHmtrp.charArray(bufferLength)

# make sure port is initialized properly. 9600 baud is the default.
if (not my_HMTRP_Radio.setupTty(upmHmtrp.cvar.int_B9600)):
	print "Failed to setup tty port parameters"
	sys.exit(0)


usageStr = ("Usage:\n"
"Pass a commandline argument (any argument) to this program\n"
"to query the radio configuration and output it.  NOTE: the\n"
"radio must be in CONFIG mode for this to work.\n\n"
"Running this program without arguments will simply transmit\n"
"'Hello World!' every second, and output any data received from\n"
"another radio.\n\n")
print usageStr

'''
By default, this radio simply transmits data sent via writeData()
and reads any available data via readData().

It can be placed into a configuration mode by grounding the
CONFIG pin on the module.  When this is done, the various
configuration query and config methods can be used.  In this
example, by default, we just read any data available fom the
device, and periodically transmit "Hello World".

If any argument was specified on the command line, do a simple
configuration query and output the results.  The radio must be in 
CONFIG mode for this to work.


Note that the first command-line argument should be "hmtry.py"
The data we want would be the second... if it exists
'''
if (len(sys.argv) > 1):
	# config mode
	freq = upmHmtrp.uint32Array(0)
	dataRate = upmHmtrp.uint32Array(0)
	rxBandwidth = upmHmtrp.uint16Array(0)
	modulation = upmHmtrp.uint8Array(0)
	txPower = upmHmtrp.uint8Array(0)
	uartBaud = upmHmtrp.uint32Array(0)

	if (my_HMTRP_Radio.getConfig(freq, dataRate, rxBandwidth,
	modulation, txPower, uartBaud)):
		print "Radio configuration:"
		outputStr = ("freq: {0} dataRate: {1} "
		"rxBandwidth: {2}Khz").format(freq.__getitem__(0),
		dataRate.__getitem__(0),
		rxBandwidth.__getitem__(0))
		print outputStr

		outputStr = "modulation: %d Khz txPower: %d uartBaud: %d" % (
		modulation.__getitem__(0), txPower.__getitem__(0),
		uartBaud.__getitem__(0))
		print outputStr
	else:
		errString = ("getConfig() failed.  Make sure the radio "
		"is in CONFIG mode.")
		print errString
else:
	print "Running in normal read/write mode."
	while (1):
		# we don't want the read to block in this example, so always
		# check to see if data is available first.
		if (my_HMTRP_Radio.dataAvailable()):
			rv = my_HMTRP_Radio.readData(radioBuffer, bufferLength)

			if (rv > 0):
				resultStr = "";
				for x in range(rv):
					resultStr += radioBuffer.__getitem__(x)
				print "Received:", resultStr

			if (rv < 0): # some sort of read error occured
				print "Port read error."
				sys.exit(0)
		myCounter += 1
		# every second, transmit "Hello World"
		if (myCounter > 10):
			msg = "Hello World!"

			print "Transmitting %s..." % msg

			# Adding 1 for NULL terminator.
			# Note that SWIG automatically adds a NULL terminator,
			# so no need to NULL-terminate ourselves.
			# Just increment the message length to include
			# the NULL that's already there
			my_HMTRP_Radio.writeData(msg, (len(msg) + 1))
			myCounter = 0
		time.sleep(.1)
