#!/usr/bin/python
# Author: Jon Trulson <jtrulson@ics.com>
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
from upm import pyupm_t3311 as sensorObj

def main():
    ## Exit handlers ##
    # This function stops python from printing a stacktrace when you hit control-C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    # This function lets you run code on exit
    def exitHandler():
        print("Exiting")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    defaultDev = "/dev/ttyUSB0"

    # if an argument was specified, use it as the device instead
    if (len(sys.argv) > 1):
        defaultDev = sys.argv[1]

    print("Initializing...")

    # Instantiate an T3311 instance, using MODBUS slave address 1, and
    # default comm parameters (9600, 8, N, 2)
    sensor = sensorObj.T3311(defaultDev, 1)

    # output the serial number and firmware revision
    print("Serial Number:", sensor.getSerialNumber())
    print("Firmware Revision: {0}.{1}".format(sensor.getFirmwareMajor(),
                                              sensor.getFirmwareMinor()))
    print()

    # update and print available values every second
    while (1):
        # update our values from the sensor
        sensor.update()

        # we show both C and F for temperature
        print("Temperature:", sensor.getTemperature(), "C /", end=' ')
        print(sensor.getTemperature(True), "F")

        print("Humidity:", sensor.getHumidity(), "%")

        # this value depends on the sensor configuration -- by default
        # it is the dew point temperature
        print("Computed Value:", sensor.getComputedValue())

        # with FW revisions > 2.44, extended computed data is available
        if (sensor.extendedDataAvailable()):
            print("Dew Point Temperature:", sensor.getDewPointTemperature(), end=' ')
            print("C /", sensor.getDewPointTemperature(True), "F")

            print("Absolute Humidity:", sensor.getAbsoluteHumidity(), "g/m3")

            print("Specific Humidity:", sensor.getSpecificHumidity(), end=' ')
            print("g/kg")

            print("Mixing Ratio:", sensor.getMixingRatio(), "g/kg")

            print("Specific Enthalpy:", sensor.getSpecificEnthalpy(), end=' ')
            print("kJ/kg")

        print()
        time.sleep(1)

if __name__ == '__main__':
    main()
