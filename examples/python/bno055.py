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
from upm import pyupm_bno055 as sensorObj

def main():
    # Instantiate an BNO055 using default parameters (bus 0, addr
    # 0x28).  The default running mode is NDOF absolute orientation
    # mode.
    sensor = sensorObj.BNO055()

    ## Exit handlers ##
    # This function stops python from printing a stacktrace when you hit control-C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    # This function lets you run code on exit
    def exitHandler():
        print("Exiting...")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    mag = sensorObj.new_intp()
    acc = sensorObj.new_intp()
    gyr = sensorObj.new_intp()
    syst = sensorObj.new_intp()

    w = sensorObj.new_floatp()
    x = sensorObj.new_floatp()
    y = sensorObj.new_floatp()
    z = sensorObj.new_floatp()

    print("First we need to calibrate.  4 numbers will be output every")
    print("second for each sensor.  0 means uncalibrated, and 3 means")
    print("fully calibrated.")
    print("See the UPM documentation on this sensor for instructions on")
    print("what actions are required to calibrate.")
    print()

    while (not sensor.isFullyCalibrated()):
        sensor.getCalibrationStatus(mag, acc, gyr, syst)
        print("Magnetometer:", sensorObj.intp_value(mag), end=' ')
        print(" Accelerometer:", sensorObj.intp_value(acc), end=' ')
        print(" Gyroscope:", sensorObj.intp_value(gyr), end=' ')
        print(" System:", sensorObj.intp_value(syst), end=' ')
        time.sleep(1)

    print()
    print("Calibration complete.")
    print()

    # now output various fusion data every 250 milliseconds

    while (True):
        sensor.update()

        sensor.getEulerAngles(x, y, z)
        print("Euler: Heading:", sensorObj.floatp_value(x), end=' ')
        print(" Roll:", sensorObj.floatp_value(y), end=' ')
        print(" Pitch:", sensorObj.floatp_value(z), end=' ')
        print(" degrees")

        sensor.getQuaternions(w, x, y, z)
        print("Quaternion: W:", sensorObj.floatp_value(w), end=' ')
        print(" X:", sensorObj.floatp_value(x), end=' ')
        print(" Y:", sensorObj.floatp_value(y), end=' ')
        print(" Z:", sensorObj.floatp_value(z))

        sensor.getLinearAcceleration(x, y, z)
        print("Linear Acceleration: X:", sensorObj.floatp_value(x), end=' ')
        print(" Y:", sensorObj.floatp_value(y), end=' ')
        print(" Z:", sensorObj.floatp_value(z), end=' ')
        print(" m/s^2")

        sensor.getGravityVectors(x, y, z)
        print("Gravity Vector: X:", sensorObj.floatp_value(x), end=' ')
        print(" Y:", sensorObj.floatp_value(y), end=' ')
        print(" Z:", sensorObj.floatp_value(z), end=' ')
        print(" m/s^2")

        print()
        time.sleep(.25);

if __name__ == '__main__':
    main()
