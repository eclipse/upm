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

public class RN2903_P2P_RX_Example
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

        // For this example, we will just try to receive a packet
        // transmitted by the p2p-tx rn2903 example.  We reset the
        // device to defaults, and we do not make any adjustments to the
        // radio configuration.  You will probably want to do so for a
        // real life application.

        // The first thing to do is to suspend the LoRaWAN stack on the device.
        sensor.macPause();

        // We will use continuous mode (window_size 0), though the default
        // radio watch dog timer will expire every 15 seconds.  We will
        // just loop here.

        while (true)
        {
            System.out.println("Waiting for packet...");
            RN2903_RESPONSE_T rv = sensor.radioRx(0);
            if (rv != RN2903_RESPONSE_T.RN2903_RESPONSE_OK)
            {
                System.out.println("radioRx() failed with code "
                                    + rv.toString());
            }
            else
            {
                String resp = sensor.getResponse();
                String payload = sensor.getRadioRxPayload();
                if (payload.length() == 0)
                    System.out.println("Got response: '" + resp + "'");
                else
                    System.out.println("Got payload: '"
                                + sensor.fromHex(payload)
                                + "'");
            }

            System.out.println();
        }

// ! [Interesting]
    }
}
