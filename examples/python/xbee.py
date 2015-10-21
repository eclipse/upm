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
import pyupm_xbee as sensorObj

# Instantiate a XBee Module on UART 0
sensor = sensorObj.XBee(0)

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

# Set the baud rate, 9600 baud is the default.
if (sensor.setBaudRate(9600)):
	print "Failed to set baud rate"
	sys.exit(0)


usageStr = ("Usage:\n"
"If an argument is supplied on the command line, that argument is\n"
"sent to the module and the response is printed out.\n\n"
"If no argument is used, then the firmware revision, serial number\n"
"and the current IP address (if set) are queried.\n\n")
print usageStr

# simple helper function to send a command and wait for a response
def sendCommand(sensor, cmd):
        # commands need to be terminated with a carriage return
        cmd += "\r"
	sensor.writeDataStr(cmd)

        resp = ""
	while sensor.dataAvailable(2000):
		resp += sensor.readDataStr(1024)

        if not resp:
                print "Timed out waiting for response"
        else:
                resp = sensor.stringCR2LF(resp)
                print "Returned (", len(resp), "bytes):"
                print resp


if (len(sys.argv) > 1):
        # enable command mode
        sensor.commandMode()
	print "Sending command line argument (" + sys.argv[1] + ")..."
	sendCommand(sensor, sys.argv[1])
else:
        # enable command mode
        sensor.commandMode()
        # query the verbose firmware revision
        print "Querying verbose firmware revision (ATVL)..."
        sendCommand(sensor, "ATVL")
        # query the number
        print "Querying Serial Number High (ATSH)..."
        sendCommand(sensor, "ATSH")
        print "Querying Serial Number Low (ATSL)..."
        sendCommand(sensor, "ATSL")

        print "Querying address, if set (ATMY)..."
        sendCommand(sensor, "ATMY");

        # For the XBee WiFi S6B
        # A comprehensive list of commands and command modes is
        # available from the datasheet at:
        # ftp1.digi.com/support/documentation/90002180_L.pdf
        
        # For the XBee S1
        # A comprehensive list of commands and command modes is
        # available from the datasheet at:
        # http://www.sparkfun.com/datasheets/Wireless/Zigbee/XBee-Datasheet.pdf
        
        # For the XBee WiFi module:
        # An example using AT commands to connect to an AP, with a
        # private Key using WPA2:
        
        # Connect to AP with SSID 'mySSID':
        #  ATIDmySSID
        
        # Provide the private key 'secret':
        #  ATPKsecret
        
        # Use WPA2 encryption
        #  ATEE2

