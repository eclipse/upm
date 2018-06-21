#!/usr/bin/env python

# The MIT License (MIT)
#
# Author: Assam Boudjelthia
# Copyright (c) 2018 Rohm Semiconductor.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN

import time, sys, signal, atexit
from upm import pyupm_kxtj3


def main():
    kxtj3_sensor = pyupm_kxtj3.KXTJ3(0x0, 0x0f)

    kxtj3_sensor.SensorInit(pyupm_kxtj3.KXTJ3_ODR_25,
                            pyupm_kxtj3.HIGH_RES,
                            pyupm_kxtj3.KXTJ3_RANGE_16G_14)

    # Prevent stack printing on CTRL^C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    def exitHandler():
        print("Exiting")
        sys.exit(0)

    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    sampleCounter = 10
    waitTime = kxtj3_sensor.GetAccelerationSamplePeriod()
    print("Setting settings:\nODR: 25 Hz\nResolution: "
          "High\nAcceleration range: 16g with 14bits")
    print("Acceleration:")
    while sampleCounter > 0:
        [x, y, z] = kxtj3_sensor.GetAccelerationVector()

        print ("x: %0.02f, y: %0.02f, z: %0.02f" % (x, y, z))

        time.sleep(waitTime)
        sampleCounter -= 1


if __name__ == '__main__':
    main()
