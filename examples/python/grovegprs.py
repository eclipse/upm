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
import pyupm_grovegprs as sensorObj

# Instantiate a GroveGPRS Module on UART 0
sensor = sensorObj.GroveGPRS(0)

## Exit handlers ##
# This stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)

# Set the baud rate, 19200 baud is the default.
if (sensor.setBaudRate(19200)):
	print "Failed to set baud rate"
	sys.exit(0)


usageStr = ("Usage:\n"
"If an argument is supplied on the command line, that argument is\n"
"sent to the module and the response is printed out.\n\n"
"If no argument is used, then the manufacturer and the current\n"
"saved profiles are queried and the results printed out.\n\n")
print usageStr

# simple helper function to send a command and wait for a response
def sendCommand(sensor, cmd):
        # commands need to be terminated with a carriage return
        cmd += "\r";
	sensor.writeDataStr(cmd)

	# wait up to 1 second
	if (sensor.dataAvailable(1000)):
                print "Returned: ",
		print sensor.readDataStr(1024)
	else:
		print "Timed out waiting for response"


if (len(sys.argv) > 1):
	print "Sending command line argument (" + sys.argv[1] + ")..."
	sendCommand(sensor, sys.argv[1])
else:
        # query the module manufacturer
        print "Querying module manufacturer (AT+CGMI)..."
        sendCommand(sensor, "AT+CGMI");

        time.sleep(1);

        # query the saved profiles
        print "Querying the saved profiles (AT&V)..."
        sendCommand(sensor, "AT&V");

        # A comprehensive list is available from the datasheet at:
        # http://www.seeedstudio.com/wiki/images/7/72/AT_Commands_v1.11.pdf
