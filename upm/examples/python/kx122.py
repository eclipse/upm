#!/usr/bin/env python
# Author: Noel Eck <noel.eck@intel.com>
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
from upm import pyupm_kx122


def main():
    # Instantiate a KX122
    kx122_sensor = pyupm_kx122.KX122(0, -1, 24)

    # Reset the device
    kx122_sensor.softwareReset()

    # Initialize the device
    kx122_sensor.deviceInit(pyupm_kx122.KX122_ODR_50,
                            pyupm_kx122.HIGH_RES,
                            pyupm_kx122.KX122_RANGE_2G)

    # This function stops python from printing a stacktrace when you hit ctrl-C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    # Exit handler
    def exitHandler():
        print("Exiting")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    # Print the acceleration
    while True:
        [x, y, z] = kx122_sensor.getAccelerationData()

        print ("x: %0.02f, y: %0.02f, z: %0.02f" % (x, y, z))

        time.sleep(1)


if __name__ == '__main__':
    main()
