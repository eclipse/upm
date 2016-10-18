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

import time
from upm import pyupm_md as upmmd

def main():
    I2C_BUS = upmmd.MD_I2C_BUS
    I2C_ADDR = upmmd.MD_DEFAULT_I2C_ADDR

    # Instantiate an I2C Motor Driver on I2C bus 0
    myMotorDriver = upmmd.MD(I2C_BUS, I2C_ADDR)

    # This example demonstrates using the MD to drive a stepper motor

    # configure it, for this example, we'll assume 200 steps per rev
    myMotorDriver.configStepper(200)

    # set for half a rotation
    myMotorDriver.setStepperSteps(100)

    # let it go - clockwise rotation, 10 RPM speed
    myMotorDriver.enableStepper(upmmd.MD_STEP_DIR_CW, 10)

    time.sleep(3)

    # Now do it backwards...
    myMotorDriver.setStepperSteps(100)
    myMotorDriver.enableStepper(upmmd.MD_STEP_DIR_CCW, 10)

    # now disable
    myMotorDriver.disableStepper()

if __name__ == '__main__':
    main()
