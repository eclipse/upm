#!/usr/bin/env python
from __future__ import print_function
# Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
# Copyright (c) 2017 Intel Corporation.
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
from upm import pyupm_htu21d as htu21d

def main():
    # Create an I2C sensor object
    sensor = htu21d.HTU21D(0)

    # Loop indefinitely, updating data and printing values
    while True:
        sensor.sampleData()
        print("Relative Humidity: %f %%" % sensor.getHumidity())
        print("Compensated RH: %f %%" % sensor.getCompRH())
        print("Temperature: %f C" % sensor.getTemperature())
        print("Dew Point: %f C" % sensor.getDewPoint())

        # Sleep for 1 s
        sleep(1)

if __name__ == '__main__':
    main()
