#!/usr/bin/python
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
from upm import pyupm_mma7361 as sensorObj

def main():
    # Instantiate a MMA7361 sensor on analog pins A0 (X), A1 (Y) A2
    # (Z), selftest pin on D2, sleep pin on D3 nd an analog reference
    # value of 5.0.  The freefall pin and the range pin are unused
    # (-1).
    sensor = sensorObj.MMA7361(0, 1, 2, 2, 3, -1, -1, 5.0)

    # 1.5g (true = 6g)
    sensor.setRange(False)

    ## Exit handlers ##
    # This function stops python from printing a stacktrace when you hit control-C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    # This function lets you run code on exit
    def exitHandler():
        print("Exiting")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    x = sensorObj.new_floatp()
    y = sensorObj.new_floatp()
    z = sensorObj.new_floatp()

    # Every 10th of a second, update and print values
    while (1):
        sensor.update()

        sensor.getAcceleration(x, y, z)
        print("Accelerometer x:", sensorObj.floatp_value(x), end=' ')
        print(" y:", sensorObj.floatp_value(y), end=' ')
        print(" z:", sensorObj.floatp_value(z))

        sensor.getVolts(x, y, z)
        print("Volts x:", sensorObj.floatp_value(x), end=' ')
        print(" y:", sensorObj.floatp_value(y), end=' ')
        print(" z:", sensorObj.floatp_value(z))

        print()
        time.sleep(.100)

if __name__ == '__main__':
    main()
