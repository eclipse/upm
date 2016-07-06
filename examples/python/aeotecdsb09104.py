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

import time, sys, signal, atexit
import pyupm_ozw as sensorObj

# This function lets you run code on exit
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)

defaultDev = "/dev/ttyACM0"
if (len(sys.argv) > 1):
        defaultDev = sys.argv[1]
print "Using device", defaultDev

# Instantiate an Aeotec DSB09104 instance, on device node 12.  You
# will almost certainly need to change this to reflect your own
# network.  Use the ozwdump example to see what nodes are available.
sensor = sensorObj.AeotecDSB09104(12)

# The first thing to do is create options, then lock them when done.
sensor.optionsCreate()
sensor.optionsLock()

# Next, initialize it.
print "Initializing, this may take awhile depending on your ZWave network"

sensor.init(defaultDev)
print "Initialization complete"

print "Querying data..."

while (True):
        sensor.update()

        print "Watts, Channel 1:",
        print sensor.getWattsC1(),
        print "W"

        print "Watts, Channel 2:",
        print sensor.getWattsC2(),
        print "W"

        print "Watts, Channel 3:",
        print sensor.getWattsC3(),
        print "W"

        print "Energy, Channel 1:",
        print sensor.getEnergyC1(),
        print "kWh"

        print "Energy, Channel 2:",
        print sensor.getEnergyC2(),
        print "kWh"

        print "Energy, Channel 3:",
        print sensor.getEnergyC3(),
        print "kWh"

        print "Battery Level:",
        print sensor.getBatteryLevel(),
        print "%"
        print
        time.sleep(3)
