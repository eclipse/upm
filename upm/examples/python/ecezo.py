#!/usr/bin/env python
# Author: Jon Trulson <jtrulson@ics.com>
# Copyright (c) 2016 Intel Corporation.
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
from upm import pyupm_ecezo as sensorObj

def main():
    # Instantiate a ECEZO sensor on uart 0 at 9600 baud.
    sensor = sensorObj.ECEZO(0, 9600, False);

    # For I2C, assuming the device is configured for address 0x64 on
    # I2C bus 0, you could use something like:
    #
    # sensor = sensorObj.ECEZO(0, 0x64, True);

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

    while (1):
        sensor.update()

        print("EC "
              + str(sensor.getEC())
              + " uS/cm, TDS "
              + str(sensor.getTDS())
              + " mg/L, Salinity "
              + str(sensor.getSalinity())
              + " PSS-78, SG "
              + str(sensor.getSG()));
        time.sleep(5)

if __name__ == '__main__':
    main()
