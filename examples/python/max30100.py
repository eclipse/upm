#!/usr/bin/env python
# Author: Noel Eck <noel.eck@intel.com>
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
from upm import pyupm_max30100

# Callback class derived from CXX Callback
class mycallback(pyupm_max30100.Callback):
    def __init__(self):
        self.count = 0
        pyupm_max30100.Callback.__init__(self)

    def run(self, samp):
        print("My callback sample IR: %d R: %d" % (samp.IR, samp.R))

def main():
    # Create an instance of the oximiter
    # I2C bus 0
    x = pyupm_max30100.MAX30100(0)

    print ('Oximeter sensor example...')

    # Create an instance of the mycallback class
    cb = mycallback().__disown__()

    # Read the temperature and version
    print ("Temperature: %d C" % x.temperature())
    print ("Version: 0x%04x" % x.version())

    # Set high-res (50 Hz, 16-bit)
    x.high_res_enable(True)

    # Set to sample SpO2
    x.mode(pyupm_max30100.MAX30100_MODE_SPO2_EN);

    # Read continuously, stepping up the LED current every second,
    # us GPIO 0 as the interrupt pin
    x.sample_continuous(0, False, cb)
    for i in range(16):
        print("Setting LED current = %d" % i)
        x.current(i, i)
        time.sleep(1)

    # Read individual samples
    for i in range(10):
        val = x.sample();
        print("Single value IR: %d R: %d " % (val.IR, val.R))

if __name__ == '__main__':
    main()
