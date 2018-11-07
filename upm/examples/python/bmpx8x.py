#!/usr/bin/env python
# Author: Zion Orent <zorent@ics.com>
# Author: Jon Trulson <jtrulson@ics.com>
# Copyright (c) 2014-2017 Intel Corporation.
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
from upm import pyupm_bmpx8x as upmBmpx8x

def main():
    # Load Barometer module on i2c using default values
    sensor = upmBmpx8x.BMPX8X(0);

    ## Exit handlers ##

    # This function stops python from printing a stacktrace when you hit
    # control-C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    # This function lets you run code on exit, including functions
    # from sensor
    def exitHandler():
        print("Exiting")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    # Print the pressure, altitude, sea level, and
    # temperature values every 0.1 seconds
    while(1):
        sensor.update()

        outputStr = ("Pressure: {0}"
        " Pa, Temperature: {1}"
        " C, Altitude: {2}"
        " m, Sea Level: {3} Pa".format(
        sensor.getPressure(),
        sensor.getTemperature(),
        sensor.getAltitude(),
        sensor.getSealevelPressure()))

        print(outputStr)
        time.sleep(.5)

if __name__ == '__main__':
    main()
