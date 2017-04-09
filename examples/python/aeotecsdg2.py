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
        print("Turning switch off and sleeping for 5 seconds...")
        sensor.off()
        time.sleep(5)
        print("Exiting")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)

    defaultDev = "/dev/ttyACM0"
    if (len(sys.argv) > 1):
        defaultDev = sys.argv[1]
    print("Using device", defaultDev)

    # Instantiate an Aeotec Smart Dimmer Gen2 instance, on device node
    # 9.  You will almost certainly need to change this to reflect your
    # own network.  Use the ozwdump example to see what nodes are
    # available.
    sensor = sensorObj.AeotecSDG2(9)

    # The first thing to do is create options, then lock them when done.
    sensor.optionsCreate()
    sensor.optionsLock()

    # Next, initialize it.
    print("Initializing, this may take awhile depending on your ZWave network")

    sensor.init(defaultDev)
    print("Initialization complete")

    # turn light on
    print("Turning switch on, then sleeping for 5 secs")
    sensor.on();
    time.sleep(5);

    print("Querying data...")
    dim = False;
    while (True):
        # put on a light show...
        if (dim):
            sensor.setLevel(25)
        else:
            sensor.on()

        dim = not dim;

        sensor.update()

        print("Current Level:", end=' ')
        print(sensor.getLevel())

        print("Volts:", end=' ')
        print(sensor.getVolts(), end=' ')
        print("volts")

        print("Energy Consumption:", end=' ')
        print(sensor.getEnergy(), end=' ')
        print("kWh")

        print("Watts:", end=' ')
        print(sensor.getWatts())

        print("Current:", end=' ')
        print(sensor.getCurrent(), end=' ')
        print("amps")

        print()
        time.sleep(5)

if __name__ == '__main__':
    main()
