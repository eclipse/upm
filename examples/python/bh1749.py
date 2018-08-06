# The MIT License (MIT)
#
# Author: Assam Boudjelthia
# Copyright (c) 2018 Rohm Semiconductor.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

import time, sys, signal, atexit
from upm import pyupm_bh1749


def main():
    bh1749_sensor = pyupm_bh1749.BH1749()

    bh1749_sensor.SensorInit(pyupm_bh1749.INT_JUDGE_1,
						pyupm_bh1749.MEAS_240MS,
						pyupm_bh1749.RGB_GAIN_1X,
						pyupm_bh1749.IR_GAIN_1X,
						pyupm_bh1749.RED)
    bh1749_sensor.SetThresholdHigh(511)

    # Prevent stack printing on CTRL^C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    def exitHandler():
        print("Exiting")
        sys.exit(0)

    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    sampleCounter = 10
    waitTime = bh1749_sensor.GetMeasurementTime()
    print("Color readings: ")
    while sampleCounter > 0:
        [r, g, b, ir, g2] = bh1749_sensor.GetMeasurements()
        print ("R: %d, y: %d, G: %d, IR: %d, G2: %d" % (r, g, b, ir, g2))
        
        time.sleep(waitTime / 1000.0)
        sampleCounter -= 1


if __name__ == '__main__':
    main()
