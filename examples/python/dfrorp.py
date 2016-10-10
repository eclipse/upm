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
from upm import pyupm_dfrorp as sensorObj

def main():
    # Instantiate a DFRobot ORP sensor on analog pin A0 with an analog
    # reference voltage of 5.0.
    sensor = sensorObj.DFRORP(0, 5.0)

    # To calibrate:
    #
    # Disconnect the sensor probe (but leave the sensor interface board
    # connected).  Then run one of the examples while holding down the
    # 'calibrate' button on the device.  Read the ORP value reported
    # (it should be fairly small).
    #
    # This value is what you should supply to setCalibrationOffset().
    # Then reconnect the probe to the interface board and you should be
    # ready to go.
    #
    # DO NOT press the calibrate button on the interface board while
    # the probe is attached or you can permanently damage the probe.
    sensor.setCalibrationOffset(0.97);

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

    # Every second, update and print values
    while (True):
        sensor.update()

        print("ORP:", sensor.getORP(), "mV")

        print()

        time.sleep(1)

if __name__ == '__main__':
    main()
