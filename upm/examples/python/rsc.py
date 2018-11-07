#!/usr/bin/env python
# Author: Abhishek Malik <abhishek.malik@intel.com>
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
from upm import pyupm_rsc as rsc

# Since CI loads each python example, the following would fail if the types
# from rsc_regs.h are NOT exposed in the pyupm_rsc module
mode = rsc.NORMAL_MODE
dr = rsc.N_DR_20_SPS

def main():
    # Instantiate a Honeywell RSC Pressure sensor on the SPI bus 0
    rsc_sensor = rsc.RSC(0, 9, 8);

    ## Exit handlers ##
    # This function stops python from printing a stacktrace when you hit control-C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    # This function lets you run code on exit, including functions from abpdrrt005pg2a5
    def exitHandler():
        print("Exiting")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    # Read the value every second and detect the pressure
    print("Sensor Name: {0}".format(rsc_sensor.getSensorName()))
    print("Sensor Serial Number: {0}".format(rsc_sensor.getSensorSerialNumber()))
    while(1):
        print("Pressure {0}: {1}".format(rsc_sensor.getPressureUnit(), rsc_sensor.getPressure()))
        print("Temperature C: {0}".format(rsc_sensor.getTemperature()))
        time.sleep(1)

if __name__ == '__main__':
    main()
