# Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
# Copyright (c) 2014 Intel Corporation.
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

from time import sleep
import pyupm_adxl345 as adxl345

# Create an I2C accelerometer object
adxl = adxl345.Adxl345(0)

# Loop indefinitely
while True:

    adxl.update() # Update the data
    raw = adxl.getRawValues() # Read raw sensor data
    force = adxl.getAcceleration() # Read acceleration force (g)
    print "Raw: %6d %6d %6d" % (raw[0], raw[1], raw[2])
    print "ForceX: %5.2f g" % (force[0])
    print "ForceY: %5.2f g" % (force[1])
    print "ForceZ: %5.2f g\n" % (force[2])

    # Sleep for 1 s
    sleep(1)
