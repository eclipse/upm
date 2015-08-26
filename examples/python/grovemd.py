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

import time
import pyupm_grovemd as upmGrovemd

I2C_BUS = upmGrovemd.GROVEMD_I2C_BUS
I2C_ADDR = upmGrovemd.GROVEMD_DEFAULT_I2C_ADDR

# Instantiate an I2C Grove Motor Driver on I2C bus 0
myMotorDriver = upmGrovemd.GroveMD(I2C_BUS, I2C_ADDR)


# set direction to CW and set speed to 50%
print "Spin M1 and M2 at half speed for 3 seconds"
myMotorDriver.setMotorDirections(upmGrovemd.GroveMD.DIR_CW, upmGrovemd.GroveMD.DIR_CW)
myMotorDriver.setMotorSpeeds(127, 127)

time.sleep(3)
# counter clockwise
print "Reversing M1 and M2 for 3 seconds"
myMotorDriver.setMotorDirections(upmGrovemd.GroveMD.DIR_CCW,
upmGrovemd.GroveMD.DIR_CCW)
time.sleep(3)

print "Stopping motors"
myMotorDriver.setMotorSpeeds(0, 0)
