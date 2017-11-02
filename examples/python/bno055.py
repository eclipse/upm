#!/usr/bin/env python
# Author: Jon Trulson <jtrulson@ics.com>
# Copyright (c) 2016-2017 Intel Corporation.
#
# The MIT License
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
from upm import pyupm_bno055 as sensorObj

def main():
    # Instantiate an BNO055 using default parameters (bus 0, addr
    # 0x28).  The default running mode is NDOF absolute orientation
    # mode.
    sensor = sensorObj.BNO055()

    ## Exit handlers ##
    # This function stops python from printing a stacktrace when you
    # hit control-C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    # This function lets you run code on exit
    def exitHandler():
        print("Exiting...")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    print("First we need to calibrate.  4 numbers will be output every")
    print("second for each sensor.  0 means uncalibrated, and 3 means")
    print("fully calibrated.")
    print("See the UPM documentation on this sensor for instructions on")
    print("what actions are required to calibrate.")
    print()

    while (not sensor.isFullyCalibrated()):
        intData = sensor.getCalibrationStatus()
        print("Magnetometer:", intData[0], end=' ')
        print(" Accelerometer:", intData[1], end=' ')
        print(" Gyroscope:", intData[2], end=' ')
        print(" System:", intData[3])
        time.sleep(1)

    print()
    print("Calibration complete.")
    print()

# example - read calibration data, sleep and then write it
#    print("Reading calibration data....")
#    byteData = sensor.readCalibrationData()
#    print("Read data successfully.")
#    print("Writing calibration data...")
#    time.sleep(1)
#    sensor.writeCalibrationData(byteData)
#    print("Success!")
#    time.sleep(3)

    # now output various fusion data every 250 milliseconds

    while (True):
        sensor.update()

        floatData = sensor.getEulerAngles()
        print("Euler: Heading:", floatData[0], end=' ')
        print(" Roll:", floatData[1], end=' ')
        print(" Pitch:", floatData[2], end=' ')
        print(" degrees")

        floatData = sensor.getQuaternions()
        print("Quaternion: W:", floatData[0], end=' ')
        print(" X:", floatData[1], end=' ')
        print(" Y:", floatData[2], end=' ')
        print(" Z:", floatData[3])

        floatData = sensor.getLinearAcceleration()
        print("Linear Acceleration: X:", floatData[0], end=' ')
        print(" Y:", floatData[1], end=' ')
        print(" Z:", floatData[2], end=' ')
        print(" m/s^2")

        floatData = sensor.getGravityVectors()
        print("Gravity Vector: X:", floatData[0], end=' ')
        print(" Y:", floatData[1], end=' ')
        print(" Z:", floatData[2], end=' ')
        print(" m/s^2")

        print()
        time.sleep(.25);

if __name__ == '__main__':
    main()
