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
from upm import pyupm_ozw as sensorObj

def main():
    # This function lets you run code on exit
    def exitHandler():
        print("Exiting")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)

    defaultDev = "/dev/ttyACM0"
    if (len(sys.argv) > 1):
        defaultDev = sys.argv[1]
    print("Using device", defaultDev)

    # Instantiate an Aeotec DSB09104 instance, on device node 12.  You
    # will almost certainly need to change this to reflect your own
    # network.  Use the ozwdump example to see what nodes are available.
    sensor = sensorObj.AeotecDSB09104(12)

    # The first thing to do is create options, then lock them when done.
    sensor.optionsCreate()
    sensor.optionsLock()

    # Next, initialize it.
    print("Initializing, this may take awhile depending on your ZWave network")

    sensor.init(defaultDev)
    print("Initialization complete")

    print("Querying data...")

    while (True):
        sensor.update()

        print("Watts, Channel 1: %0.03f W" % sensor.getWattsC1())
        print("Watts, Channel 2: %0.03f W" % sensor.getWattsC2())
        print("Watts, Channel 3: %0.03f W" % sensor.getWattsC3())

        print("Energy, Channel 1: %0.03f kWh" % sensor.getEnergyC1())
        print("Energy, Channel 2: %0.03f kWh" % sensor.getEnergyC2())
        print("Energy, Channel 3: %0.03f kWh" % sensor.getEnergyC3())

        print("Battery Level: %d\n" % sensor.getBatteryLevel())
        time.sleep(3)

if __name__ == '__main__':
    main()
