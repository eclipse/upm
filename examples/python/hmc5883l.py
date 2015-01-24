# Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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

from time import sleep
import pyupm_hmc5883l as hmc5883l

# Create an I2C compass object and set declination
hmc = hmc5883l.Hmc5883l(0)
hmc.set_declination(0.2749)

# Loop indefinitely
while True:

    hmc.update() # Update the data
    pos = hmc.coordinates() # Read raw coordinates
    hdg = hmc.heading() # Read heading
    dir = hmc.direction() # Read direction

    # Print values
    print "Coor: %5d %5d %5d" % (pos[0], pos[1], pos[2])
    print "Heading: %5.2f" % (hdg)
    print "Direction: %3.2f\n" % (dir)

    # Sleep for 1 s
    sleep(1)
