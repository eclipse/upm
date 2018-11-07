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

# This example demonstrates how to use one of the ADS1115 ADCs on the
# DFRobot Joule Shield with devices that output a small differential
# voltage (e.g. geophones, piezoelectric bands or pads, thermocouples).

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
    fileName = './ads1115.data' # Output filename
    id = 0 # Sample number

    # Initialize and configure the ADS1115 for the SM-24 Geophone
    # There are two ADS1115 chips on the DFRobot Joule Shield on the same I2C bus
    #     - 0x48 gives access to pins A0 - A3
    #     - 0x49 gives access to pins A4 - A7
    ads1115 = upm.ADS1115(0, 0x48)

    # Put the ADC into differential mode for pins A0 and A1,
    # the SM-24 Geophone is connected to these pins
    ads1115.getSample(upm.ADS1X15.DIFF_0_1)

    # Set the gain based on expected VIN range to -/+ 2.048 V
    # Can be adjusted based on application to as low as -/+ 0.256 V, see API
    # documentation for details
    ads1115.setGain(upm.ADS1X15.GAIN_TWO)

    # Set the sample rate to 860 samples per second (max) and turn on continuous
    # sampling
    ads1115.setSPS(upm.ADS1115.SPS_860)
    ads1115.setContinuous(True)

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
        f.write(str(id) + ' %.7f' % ads1115.getLastSample() + '\n')
        id += 1
        sleep(0.001)

    # Close and exit
    f.close()
    print('Wrote', id, 'samples to file:', fileName)
    return

if __name__ == '__main__':
    main()

