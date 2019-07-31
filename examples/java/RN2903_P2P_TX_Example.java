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

public class RN2903_P2P_TX_Example
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

        // For this example, we will just try transmitting a packet over
        // LoRa.  We reset the device to defaults, and we do not make any
        // adjustments to the radio configuration.  You will probably want
        // to do so for a real life application.

        // The first thing to do is to suspend the LoRaWAN stack on the device.
        sensor.macPause();

        // the default radio watchdog timer is set for 15 seconds, so we
        // will send a packet every 10 seconds.  In reality, local
        // restrictions limit the amount of time on the air, so in a real
        // implementation, you would not want to send packets that
        // frequently.

        Integer count = 0;
        while (true)
        {
            String output = "Ping " + count.toString();
            count++;

            // All payloads must be hex encoded
            String payload = sensor.toHex(output);

            System.out.println("Transmitting a packet, data: '"
                + output
                + "' -> hex: '"
                + payload
                + "'");

            RN2903_RESPONSE_T rv = sensor.radioTx(payload);

            if (rv == RN2903_RESPONSE_T.RN2903_RESPONSE_OK)
                System.out.println("Transmit successful.");
            else
                System.out.println("Transmit failed with code "
                                   + rv.toString());

            System.out.println();

            Thread.sleep(10000);
        }

// ! [Interesting]
    }
}
