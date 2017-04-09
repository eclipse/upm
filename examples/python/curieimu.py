#!/usr/bin/python

# Author: Ron Evans (@deadprogram)
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
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE

from __future__ import print_function
import mraa
print (mraa.getVersion())

# open connection to Firmata
mraa.addSubplatform(mraa.GENERIC_FIRMATA, "/dev/ttyACM0")

import time, sys, signal, atexit
from upm import pyupm_curieimu as curieimu

def main():
    sensor = curieimu.CurieImu()

    ## Exit handlers ##
    def SIGINTHandler(signum, frame):
        raise SystemExit

    def exitHandler():
        print("Exiting")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    while(1):
        sensor.updateAccel();

        outputStr = "acc: gX {0} - gY {1} - gZ {2}".format(
        sensor.getAccelX(), sensor.getAccelY(),
        sensor.getAccelZ())
        print(outputStr)

        time.sleep(1)

if __name__ == '__main__':
    main()
