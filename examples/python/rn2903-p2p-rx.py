#!/usr/bin/env python
# Author: Jon Trulson <jtrulson@ics.com>
# Copyright (c) 2017 Intel Corporation.
#
# The MIT License
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
from upm import pyupm_rn2903 as sensorObj

def main():
    ## Exit handlers ##
    # This function stops python from printing a stacktrace when you
    # hit control-C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    # This function lets you run code on exit
    def exitHandler():
        print("Exiting")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    defaultDev = "/dev/ttyUSB0"
    if (len(sys.argv) > 1):
        defaultDev = sys.argv[1]
    print("Using device", defaultDev)

    # Instantiate a RN2903 sensor on defaultDev at 57600 baud.
    sensor = sensorObj.RN2903(defaultDev,
                              sensorObj.RN2903_DEFAULT_BAUDRATE)

    # To use an internal UART understood by MRAA, use the following
    # to inititialize rather than the above, which by default uses a
    # tty path.
    #
    # sensor = sensorObj.RN2903(0,
    #                           sensorObj.RN2903_DEFAULT_BAUDRATE)

    # enable debugging
    # sensor.setDebug(True)

    # get version
    if (sensor.command("sys get ver")):
        print("Failed to retrieve device version string")
        sys.exit(1)

    print("Firmware version: " + sensor.getResponse())

    print("Hardware EUI: " + sensor.getHardwareEUI())

    # For this example, we will just try to receive a packet
    # transmitted by the p2p-tx rn2903 example.  We reset the
    # device to defaults, and we do not make any adjustments to the
    # radio configuration.  You will probably want to do so for a
    # real life application.

    # The first thing to do is to suspend the LoRaWAN stack on the device.
    sensor.macPause();

    # We will use continuous mode (window_size 0), though the default
    # radio watch dog timer will expire every 15 seconds.  We will
    # just loop here.

    while (True):
        print("Waiting for packet...")
        rv = sensor.radioRx(0);
        if (rv):
            print("radioRx() failed with code " + str(rv))
        else:
            resp = sensor.getResponse();
            payload = sensor.getRadioRxPayload();
            if (not len(payload)):
                print("Got response: '", end='')
                print(resp, end="")
                print("'")
            else:
                print("Got payload: '", end='')
                print(sensor.fromHex(payload), end='')
                print("'")

        print()


if __name__ == '__main__':
    main()
