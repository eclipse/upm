#!/usr/bin/env python
# Author: Jon Trulson <jtrulson@ics.com>
# Copyright (c) 2017 Intel Corporation.
#
# The MIT License
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
from upm import pyupm_rn2903 as sensorObj

def main():
    ## Exit handlers ##
    # This function stops python from printing a stacktrace when you
    # hit control-C
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
    if (len(sys.argv) > 1):
        defaultDev = sys.argv[1]
    print("Using device", defaultDev)

    # Instantiate a RN2903 sensor on defaultDev at 57600 baud.
    sensor = sensorObj.RN2903(defaultDev,
                              sensorObj.RN2903_DEFAULT_BAUDRATE)

    # To use an internal UART understood by MRAA, use the following
    # to inititialize rather than the above, which by default uses a
    # tty path.
    #
    # sensor = sensorObj.RN2903(0,
    #                           sensorObj.RN2903_DEFAULT_BAUDRATE)

    # enable debugging
    # sensor.setDebug(True)

    # get version
    if (sensor.command("sys get ver")):
        print("Failed to retrieve device version string")
        sys.exit(1)

    print("Firmware version: " + sensor.getResponse())

    print("Hardware EUI: " + sensor.getHardwareEUI())

    # we can support two types of join, OTAA and ABP. Each requires
    # that certain parameters be set first.  We will only attempt ABP
    # joining with this example since it's the only one that can
    # succeed without actual configuration.  In both cases, if you
    # are actually attempting to join a real LoRaWAN network, you
    # must change the parameters below to match the network you are
    # attempting to join.

    # For OTAA, you need to supply valid Device EUI, Application EUI,
    # and Application key:
    #
    # sensor.setDeviceEUI("0011223344556677")
    # sensor.setApplicationEUI("0011223344556677")
    # sensor.setApplicationKey("01234567012345670123456701234567")
    #
    # rv = sensor.join(RN2903_JOIN_TYPE_OTAA)
    # A successful join will return RN2903_JOIN_STATUS_ACCEPTED (0).

    # Try an ABP join.  Note, these parameters are made up.  For a
    # real network, you will want to use the correct values
    # obviously.  For an ABP join, you need to supply the Device
    # Address, Network Session Key, and the Application Session Key.

    sensor.setDeviceAddr("00112233")
    sensor.setNetworkSessionKey("00112233001122330011223300112233")
    sensor.setApplicationSessionKey("00112233001122330011223300112233")

    rv = sensor.join(sensorObj.RN2903_JOIN_TYPE_ABP)

    if (rv == sensorObj.RN2903_JOIN_STATUS_ACCEPTED):
        print("Join successful.")

        # All transmit payloads must be hex encoded strings, so
        # pretend we have a temperature sensor that gave us a value
        # of 25.6 C, and we want to transmit it.
        faketemp = "25.6"
        print("Transmitting a packet...")

        trv = sensor.macTx(sensorObj.RN2903_MAC_MSG_TYPE_UNCONFIRMED,
                           1, # port number
                           sensor.toHex(faketemp))

        if (trv == sensorObj.RN2903_MAC_TX_STATUS_TX_OK):
            print("Transmit successful.")
        else:
            # check to see if we got a downlink packet
            if (trv == sensor.Obj.RN2903_MAC_TX_STATUS_RX_RECEIVED):
                print("Transmit successful, downlink packet received:", end=' ')
                print(sensor.getResponse())
            else:
                print("Transmit failed with code " + str(trv))
    else:
        print("Join failed with code " + str(rv))

if __name__ == '__main__':
    main()
