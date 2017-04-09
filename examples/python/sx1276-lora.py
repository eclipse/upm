#!/usr/bin/python
# Author: Jon Trulson <jtrulson@ics.com>
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

from __future__ import print_function
import time, sys, signal, atexit
from upm import pyupm_sx1276 as sensorObj

def main():
    # Instantiate an SX1276 using default parameters
    sensor = sensorObj.SX1276()

    ## Exit handlers ##
    # This stops python from printing a stacktrace when you hit control-C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    # This function lets you run code on exit
    def exitHandler():
        print("Exiting")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    print("Specify an argument to go into receive mode.  Default is transmit")

    # 915Mhz
    sensor.setChannel(915000000)

    # LORA configuration (rx and tx must be configured the same):
    # Tx output power = 14 dBm
    # LORA bandwidth = 125000 (can also be 250K and 500K)
    # LORA spreading factor = 7
    # LORA coding rate = 1 (4/5)
    # LORA preamble len = 8
    # LORA symbol timeout = 5
    # LORA fixed payload = false
    # LORA IQ inversion = false
    # LORA (rx) continuous Rx mode = true

    sensor.setTxConfig(sensor.MODEM_LORA, 14, 0, 125000,
                       7, 1, 8, False, True, False, 0, False)

    sensor.setRxConfig(sensor.MODEM_LORA, 125000, 7,
                       1, 0, 8, 5, False, 0, True, False, 0, False, True)

    count = 0

    while True:
        if (len(sys.argv) > 1):
            # receive mode
            print("Attempting to receive...")
            rv = sensor.setRx(3000)
            if (rv):
                print("setRx returned ", rv)
            else:
                print("Received Buffer: ", sensor.getRxBufferStr());
                # go back to sleep when done

                sensor.setSleep()
                time.sleep(.25)
        else:
            # transmit mode
            buffer = "Ping " + str(count)
            count += 1
            print("Sending..." + buffer)
            sensor.sendStr(buffer, 3000)
            sensor.setSleep();
            time.sleep(1);

if __name__ == '__main__':
    main()
