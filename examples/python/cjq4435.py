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

from __future__ import division
import time
import pyupm_cjq4435 as upmCjq4435

# Instantiate a CJQ4435 MOSFET on a PWM capable digital pin D3
myMOSFETsensor = upmCjq4435.CJQ4435(3)


myMOSFETsensor.setPeriodMS(10)
myMOSFETsensor.enable(True)

# start with a duty cycle of 0.0 (off) and increment to 1.0 (on)
for i in range(11):
	myMOSFETsensor.setDutyCycle(i / 10)
	time.sleep(.1)
time.sleep(1)

# Now take it back down
#	 start with a duty cycle of 1.0 (on) and decrement to 0.0 (off)
for i in range(10, -1, -1):
	myMOSFETsensor.setDutyCycle(i / 10)
	time.sleep(.1)
time.sleep(1)
