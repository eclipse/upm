#!/usr/bin/python
# Author: Jon Trulson <jtrulson@ics.com>
# Copyright (c) 2015 Intel Corporation.
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
import pyupm_mpu9150 as sensorObj

# Instantiate an MPU60X0 on I2C bus 0
sensor = sensorObj.MPU60X0()

## Exit handlers ##
# This function stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit
def exitHandler():
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)

sensor.init()

x = sensorObj.new_floatp()
y = sensorObj.new_floatp()
z = sensorObj.new_floatp()

while (1):
        sensor.update()
        sensor.getAccelerometer(x, y, z)
        print "Accelerometer: AX: ", sensorObj.floatp_value(x), 
        print " AY: ", sensorObj.floatp_value(y),
        print " AZ: ", sensorObj.floatp_value(z)

        sensor.getGyroscope(x, y, z)
        print "Gyroscope:     GX: ", sensorObj.floatp_value(x), 
        print " GY: ", sensorObj.floatp_value(y),
        print " GZ: ", sensorObj.floatp_value(z)

        print "Temperature:  ", sensor.getTemperature()
        print

	time.sleep(.5)
