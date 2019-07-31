/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

import upm_rn2903.*;

public class RN2903_Example
{
    private static String defaultDev = "/dev/ttyUSB0";

    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        if (args.length > 0)
            defaultDev = args[0];

        System.out.println("Using device " + defaultDev);

        // Instantiate a RN2903 sensor on defaultDev at 57600 baud.
        RN2903 sensor = new RN2903(defaultDev,
                                   javaupm_rn2903.RN2903_DEFAULT_BAUDRATE);

        // To use an internal UART understood by MRAA, use the following
        // to inititialize rather than the above, which by default uses a
        // tty path.
        //
        // RN2903 sensor = new RN2903(defaultDev,
        //     upm_rn2903.javaupm_rn2903.RN2903_DEFAULT_BAUDRATE);

        // enable debugging
        // sensor.setDebug(true);

        // get version
        if (sensor.command("sys get ver")
            != RN2903_RESPONSE_T.RN2903_RESPONSE_OK)
        {
            System.out.println("Failed to retrieve device version string");
            System.exit(1);
        }
        System.out.println("Firmware version: " + sensor.getResponse());

        System.out.println("Hardware EUI: " + sensor.getHardwareEUI());

        // we can support two types of join, OTAA and ABP. Each requires
        // that certain parameters be set first.  We will only attempt ABP
        // joining with this example since it's the only one that can
        // succeed without actual configuration.  In both cases, if you
        // are actually attempting to join a real LoRaWAN network, you
        // must change the parameters below to match the network you are
        // attempting to join.

        // For OTAA, you need to supply valid Device EUI, Application EUI,
        // and Application key:
        //
        // sensor.setDeviceEUI("0011223344556677");
        // sensor.setApplicationEUI("0011223344556677");
        // sensor.setApplicationKey("01234567012345670123456701234567");
        //
        // RN2903_JOIN_STATUS_T rv = sensor.join(RN2903_JOIN_TYPE_OTAA);
        // A successful join will return RN2903_JOIN_STATUS_ACCEPTED (0).
        // cout << "JOIN: got rv " << int(rv) << endl;

        // Try an ABP join.  Note, these parameters are made up.  For a
        // real network, you will want to use the correct values
        // obviously.  For an ABP join, you need to supply the Device
        // Address, Network Session Key, and the Application Session Key.

        sensor.setDeviceAddr("00112233");
        sensor.setNetworkSessionKey("00112233001122330011223300112233");
        sensor.setApplicationSessionKey("00112233001122330011223300112233");

        RN2903_JOIN_STATUS_T rv =
            sensor.join(RN2903_JOIN_TYPE_T.RN2903_JOIN_TYPE_ABP);

        if (rv == RN2903_JOIN_STATUS_T.RN2903_JOIN_STATUS_ACCEPTED)
        {
            System.out.println("Join successful.");

            // All transmit payloads must be hex encoded strings, so
            // pretend we have a temperature sensor that gave us a value
            // of 25.6 C, and we want to transmit it.
            String faketemp = "25.6";
            System.out.println("Transmitting a packet...");

            RN2903_MAC_TX_STATUS_T trv =
                sensor.macTx(RN2903_MAC_MSG_TYPE_T.RN2903_MAC_MSG_TYPE_UNCONFIRMED,
                             1, // port number
                             sensor.toHex(faketemp));

            if (trv == RN2903_MAC_TX_STATUS_T.RN2903_MAC_TX_STATUS_TX_OK)
                System.out.println("Transmit successful.");
            else
            {
                // check to see if we got a downlink packet
                if (trv == RN2903_MAC_TX_STATUS_T.RN2903_MAC_TX_STATUS_RX_RECEIVED)
                {
                    System.out.println("Transmit successful, downlink packet received: "
                                + sensor.getResponse());
                }
                else
                {
                    System.out.println("Transmit failed with code " + trv);
                }
            }
        }
        else
        {
            System.out.println("Join failed with code " + rv);
        }


        System.out.println("Exiting...");
// ! [Interesting]
    }
}
