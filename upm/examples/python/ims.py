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
from upm import pyupm_ims

def main():
    # Create an instance of the I2C Moisture Sensor
    # I2C bus 0, default address = 0x20
    ims = pyupm_ims.IMS(0)

    print ('I2C moisture sensor example...')
    while (1):
        try:
            print ('Version: %d light: 0x%04x moisture: 0x%04x temp: %3.2f C' \
                    % (ims.get_version(), ims.get_light(), ims.get_moisture(),
                    ims.get_temperature()))
            time.sleep(1)
        except KeyboardInterrupt:
            break

if __name__ == '__main__':
    main()
