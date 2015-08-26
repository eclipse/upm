#!/usr/bin/python
# Author: Zion Orent <zorent@ics.com>
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
import pyupm_adafruitms1438 as upmAdafruitms1438


# Import header values
I2CBus = upmAdafruitms1438.ADAFRUITMS1438_I2C_BUS
I2CAddr = upmAdafruitms1438.ADAFRUITMS1438_DEFAULT_I2C_ADDR

M3Motor = upmAdafruitms1438.AdafruitMS1438.MOTOR_M3
MotorDirCW = upmAdafruitms1438.AdafruitMS1438.DIR_CW
MotorDirCCW = upmAdafruitms1438.AdafruitMS1438.DIR_CCW


# Instantiate an Adafruit MS 1438 on I2C bus 0
myMotorShield = upmAdafruitms1438.AdafruitMS1438(I2CBus, I2CAddr)


## Exit handlers ##
# This stops python from printing a stacktrace when you hit control-C
def SIGINTHandler(signum, frame):
	raise SystemExit

# This function lets you run code on exit,
# including functions from myMotorShield
def exitHandler():
	myMotorShield.disableMotor(M3Motor)
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


# Setup for use with a DC motor connected to the M3 port

# set a PWM period of 50Hz
myMotorShield.setPWMPeriod(50)

# disable first, to be safe
myMotorShield.disableMotor(M3Motor)

# set speed at 50%
myMotorShield.setMotorSpeed(M3Motor, 50)
myMotorShield.setMotorDirection(M3Motor, MotorDirCW)

print ("Spin M3 at half speed for 3 seconds, "
"then reverse for 3 seconds.")
myMotorShield.enableMotor(M3Motor)

time.sleep(3)

print "Reversing M3"
myMotorShield.setMotorDirection(M3Motor, MotorDirCCW)

time.sleep(3)

print "Stopping M3"

# exitHandler runs automatically
