#!/usr/bin/env python
# Author: Jon Trulson <jtrulson@ics.com>
# Copyright (c) 2017 Intel Corporation.
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

from __future__ import print_function
import time, sys, signal, atexit
from upm import pyupm_uartat as UARTAT

def main():
    ## Exit handlers ##
    # This function stops python from printing a stacktrace when you
    # hit control-C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    # This function lets you run code on exit, including functions from sensor
    def exitHandler():
        print("Exiting")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    defaultDev = "/dev/ttyUSB0"

    # if an argument was specified, use it as the device instead
    if (len(sys.argv) > 1):
        defaultDev = sys.argv[1]

    print("Using device:", defaultDev);

    # Instantiate a UARTAT sensor on defaultDev at 115200 baud.
    sensor = UARTAT.UARTAT(defaultDev, 115200)

    # This is a simplistic example that tries to configure the LE910,
    # and use it's built-in socket capabilities to connect to a
    # remote host, obtain a small piece of data, and return it.  It's
    # mainly intended to show you how you can use the various AT
    # commands supported by the LE910 to perform simple tasks.
    #
    # You must have a valid SIM card with an active data plan for
    # this example to do anything interesting.
    #
    # See the LE910 AT Commands reference for full information on
    # what is possible with this device.  The uartat driver is
    # intended to make it a little easier to control AT-style
    # devices, but is by no means a full-featured communication
    # infrastructure.  A "real" application will probably need to be
    # much more sophisticated with regard to parsing, doing retries,
    # etc.
    #
    # For experimenting with various AT commands, try using an
    # interactive terminal emulator like minicom or screen.

    # make sure we are in command mode
    if (not sensor.inCommandMode()):
        print("Not in command mode, switching...")
        sensor.commandMode("+++", 1000)

    # flter out CR's in responses by default
    sensor.filterCR(True)

    print("Configuring modem...")

    # discard any waiting characters
    sensor.drain()

    # reset modem
    sensor.command("ATZ\r")

    # turn off command echo, set verbosity to 1, enable data
    # connection mode
    sensor.command("ATE0 V1 +FCLASS=0\r")
    sensor.drain()

    # Now issue some commands and output the results.

    print("Modem and SIM information:")

    bufferLength = 256

    buffer = sensor.commandWithResponse("AT+ICCID\r", bufferLength)
    if (buffer):
        print("ICCID (SIM ID):", buffer)

    buffer = sensor.commandWithResponse("AT+CGSN=1\r", bufferLength)
    if (buffer):
        print("IMEI: ", buffer)

    # see if we are on the network....
    buffer = sensor.commandWithResponse("AT+CREG?\r", bufferLength)
    if (buffer):
        print(buffer)

        # look for "CGREG: 0,1" or "CGREG: 0,5"
        if (sensor.find(buffer, "CREG: 0,1") or
            sensor.find(buffer, "CREG: 0,5")):
            print("Connected to the cell data network.")

            # wait up to 5 seconds for responses now...
            sensor.setResponseWaitTime(5000)

            # setup PDP context (socket 1).  An ERROR repsonse is
            # possible if the PDP context is already set up.
            sensor.command("AT#SGACT=1,1\r")

            # setup a TCP socket to nist.gov and read the timestamp.

            print("Connecting to time-a.nist.gov, TCP port 13")

            # Wait up to 60 seconds to find the NO CARRIER
            # string, which will be present at the end, if the
            # connection succeeded and the requested data was
            # obtained.
            buffer = sensor.commandWaitFor("AT#SD=1,0,13,\"time-a.nist.gov\"\r",
                                           bufferLength, "\nNO CARRIER\n", 60000)
            if (buffer):
                # print out the response
                print("RESPONSE: ")
                print(buffer)
            else:
                print("No response.")

            # destroy PDP context
            sensor.setResponseWaitTime(250)
            sensor.command("AT#SGACT=1,0\r")
        else:
            print("You do not appear to be connected to the network...")
    else:
        print("Error executing query\n")

    # reset the modem
    sensor.command("ATZ\r")


if __name__ == '__main__':
    main()
