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

M12Motor = upmAdafruitms1438.AdafruitMS1438.STEPMOTOR_M12
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
	myMotorShield.disableStepper(M12Motor)
	print "Exiting"
	sys.exit(0)

# Register exit handlers
atexit.register(exitHandler)
signal.signal(signal.SIGINT, SIGINTHandler)


# Setup for use with a stepper motor connected to the M1 & M2 ports

# set a PWM period of 50Hz

# disable first, to be safe
myMotorShield.disableStepper(M12Motor)

# configure for a NEMA-17, 200 steps per revolution
myMotorShield.stepConfig(M12Motor, 200)

# set speed at 10 RPM's
myMotorShield.setStepperSpeed(M12Motor, 10);
myMotorShield.setStepperDirection(M12Motor, MotorDirCW)

# enable
print "Enabling..."
myMotorShield.enableStepper(M12Motor)

print "Rotating 1 full revolution at 10 RPM speed."
myMotorShield.stepperSteps(M12Motor, 200)

print "Sleeping for 2 seconds..."
time.sleep(2)
print "Rotating 1/2 revolution in opposite direction at 10 RPM speed." 

myMotorShield.setStepperDirection(M12Motor, MotorDirCCW)
myMotorShield.stepperSteps(M12Motor, 100)

print "Disabling..."

# exitHandler runs automatically
