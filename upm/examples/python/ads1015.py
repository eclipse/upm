#!/usr/bin/env python
# Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION

# This example demonstrates how to use one the ADS1015 ADC on the Grove Joule
# Shield or the Sparkfun ADC Block for Edison with devices that output a small
# differential voltage (e.g. geophones, piezoelectric bands or pads,
# thermocouples).

from __future__ import print_function
from threading import Timer
from time import sleep
from upm import pyupm_ads1x15 as upm

def stop():
    global running
    running = False

def main():
    global running
    running = True
    fileName = './ads1015.data' # Output filename
    id = 0 # Sample number

    # Initialize and configure the ADS1015
    ads1015 = upm.ADS1015(0, 0x48)

    # Put the ADC into differential mode for pins A0 and A1
    ads1015.getSample(upm.ADS1X15.DIFF_0_1)

    # Set the gain based on expected VIN range to -/+ 2.048 V
    # Can be adjusted based on application to as low as -/+ 0.256 V, see API
    # documentation for details
    ads1015.setGain(upm.ADS1X15.GAIN_TWO)

    # Set the sample rate to 3300 samples per second (max) and turn on continuous
    # sampling
    ads1015.setSPS(upm.ADS1015.SPS_3300)
    ads1015.setContinuous(True)

    # Open the output file
    try:
        f = open(fileName, 'w')
    except OSError as e:
        print('Cannot open output file:', e)
        return

    # Setup a timer to stop logging after 10 seconds
    t = Timer(10, stop)
    t.start()

    # Read sensor and write to file every 1 ms
    while running:
        f.write(str(id) + ' %.7f' % ads1015.getLastSample() + '\n')
        id += 1
        sleep(0.001)

    # Close and exit
    f.close()
    print('Wrote', id, 'samples to file:', fileName)
    return

if __name__ == '__main__':
    main()

