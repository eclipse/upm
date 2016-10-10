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
from upm import pyupm_h803x as sensorObj

def main():
    ## Exit handlers ##
    # This function stops python from printing a stacktrace when you hit control-C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    # This function lets you run code on exit
    def exitHandler():
        print("Exiting...")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    defaultDev = "/dev/ttyUSB0"

    # if an argument was specified, use it as the device instead
    if (len(sys.argv) > 1):
        defaultDev = sys.argv[1]

    print("Using device", defaultDev)
    print("Initializing...")

    # Instantiate an H803X instance, using MODBUS slave address 1, and
    # default comm parameters (9600, 8, N, 2)
    sensor = sensorObj.H803X(defaultDev, 1)

    # output the serial number and firmware revision
    print("Slave ID:", sensor.getSlaveID())

    print()

    # update and print available values every second
    while (1):
        # update our values from the sensor
        sensor.update()

        # H8035 / H8036
        print("Consumption (kWh):", sensor.getConsumption())
        print("Real Power (kW):", sensor.getRealPower())

        if (sensor.isH8036()):
            # The H8036 has much more data available...

            print("Reactive Power (kVAR):", sensor.getReactivePower())
            print("Apparent Power (kVA):", sensor.getApparentPower())
            print("Power Factor:", sensor.getPowerFactor())
            print("Volts Line to Line:", sensor.getVoltsLineToLine())
            print("Volts Line to Neutral:", sensor.getVoltsLineToNeutral())

            print("Current:", sensor.getCurrent())

            print("Real Power Phase A (kW):", sensor.getRealPowerPhaseA())
            print("Real Power Phase B (kW):", sensor.getRealPowerPhaseB())
            print("Real Power Phase C (kW):", sensor.getRealPowerPhaseC())

            print("Power Factor Phase A:", sensor.getPowerFactorPhaseA())
            print("Power Factor Phase B:", sensor.getPowerFactorPhaseB())
            print("Power Factor Phase C:", sensor.getPowerFactorPhaseC())

            print("Volts Phase A to B:", sensor.getVoltsPhaseAToB())
            print("Volts Phase B to C:", sensor.getVoltsPhaseBToC())
            print("Volts Phase A to C:", sensor.getVoltsPhaseAToC())
            print("Volts Phase A to Neutral: ", end=' ')
            print(sensor.getVoltsPhaseAToNeutral())
            print("Volts Phase B to Neutral: ", end=' ')
            print(sensor.getVoltsPhaseBToNeutral())
            print("Volts Phase C to Neutral: ", end=' ')
            print(sensor.getVoltsPhaseCToNeutral())

            print("Current Phase A:", sensor.getCurrentPhaseA())
            print("Current Phase B:", sensor.getCurrentPhaseB())
            print("Current Phase C:", sensor.getCurrentPhaseC())

            print("Avg Real Power (kW):", sensor.getAvgRealPower())
            print("Min Real Power (kW):", sensor.getMinRealPower())
            print("Max Real Power (kW):", sensor.getMaxRealPower())

        print()
        time.sleep(2)

if __name__ == '__main__':
    main()
