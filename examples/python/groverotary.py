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
import pyupm_grove as grove

# New knob on AIO pin 0
knob = grove.GroveRotary(0)

# Loop indefinitely
while True:

    # Read values
    abs = knob.abs_value()
    absdeg = knob.abs_deg()
    absrad = knob.abs_rad()

    rel = knob.rel_value()
    reldeg = knob.rel_deg()
    relrad = knob.rel_rad()

    print "Abs values: %4d" % int(abs) , " raw %4d" % int(absdeg), "deg = %5.2f" % absrad , " rad ",
    print "Rel values: %4d" % int(rel) , " raw %4d" % int(reldeg), "deg = %5.2f" % relrad , " rad"

    # Sleep for 2.5 s
    sleep(2.5)
