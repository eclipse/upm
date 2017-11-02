#!/usr/bin/env python
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
from upm import pyupm_mcp2515 as MCP2515

def main():
    # Instantiate a MCP2515 on SPI bus 0 using a hw CS pin (-1).
    sensor = MCP2515.MCP2515(0, -1)

    ## Exit handlers ##
    # This function stops python from printing a stacktrace when you
    # hit control-C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    # This function lets you run code on exit, including functions from sensor
    def exitHandler():
        print("Exiting")
        sys.exit(0)

    # Register exit handlers
    atexit.register(exitHandler)
    signal.signal(signal.SIGINT, SIGINTHandler)

    # by default we just receive packets
    do_tx = False;
    if (len(sys.argv) > 1):
        do_tx = True

    # NOTE: This example assumes that only two devices are connected
    # to the CAN bus, and that both devices are running this example;
    # one in TX (transmit) mode, and one in RX (receive) mode.

    # If this is not the case, then in rx mode you may see other
    # packets from other devices, or not, depending on whether the
    # speed is correct for the network.  In tx mode, errors may be
    # generated on the bus, especially if the CAN bus speed does not
    # match the rest of the network.

    # You should start the receiver example first.  The transmitter
    # example will fail after about 5 seconds (timeout) of not being
    # able to transmit a message.

    # By default, after initialization, the baud rate is set to
    # 50Kbps, and the mode is NORMAL, so we don't need to set any of
    # that here.

    while (True):
        if (do_tx):
            print("Loading a packet of 8 numbers (0-7) into a TX buffer...")
            myPayload = "01234567";
            sensor.loadTXBuffer(MCP2515.MCP2515_TX_BUFFER0, 0, False,
                                False, myPayload);

            # now lets try to transmit it
            print("Transmitting packet...")
            sensor.transmitBuffer(MCP2515.MCP2515_TX_BUFFER0, True)

            print("Transmit successful")
            print()
            time.sleep(.5)
        else:
            # RX mode
            # Look for a packet waiting for us in RXB0
            if (sensor.rxStatusMsgs() == MCP2515.MCP2515_RXMSG_RXB0):
                print("Packet received in RXB0, decoding...")

                # now lets retrieve and print it
                sensor.getRXMsg(MCP2515.MCP2515_RX_BUFFER0)

                sensor.printMsg();
                print()

    print("Exiting...")

if __name__ == '__main__':
    main()
