/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

import upm_uartat.UARTAT;

public class LE910_Example
{
    private static String defaultDev = "/dev/ttyUSB0";
    static private final int bufferLength = 256;

    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]
        if (args.length > 0)
            defaultDev = args[0];

        System.out.println("Using device " + defaultDev);
        System.out.println("Initializing...");

        // Instantiate a UARTAT sensor on defaultDev at 115200 baud.
        UARTAT sensor = new UARTAT(defaultDev, 115200);

        // This is a simplistic example that tries to configure the LE910,
        // and use it's built-in socket capabilities to connect to a
        // remote host, obtain a small piece of data, and return it.  It's
        // mainly intended to show you how you can use the various AT
        // commands supported by the LE910 to perform simple tasks.
        //
        // You must have a valid SIM card with an active data plan for
        // this example to do anything interesting.
        //
        // See the LE910 AT Commands reference for full information on
        // what is possible with this device.  The uartat driver is
        // intended to make it a little easier to control AT-style
        // devices, but is by no means a full-featured communication
        // infrastructure.  A "real" application will probably need to be
        // much more sophisticated with regard to parsing, doing retries,
        // etc.
        //
        // For experimenting with various AT commands, try using an
        // interactive terminal emulator like minicom or screen.

        // make sure we are in command mode
        if (!sensor.inCommandMode())
        {
            System.out.println("Not in command mode, switching...");
            sensor.commandMode("+++", 1000);
        }

        // flter out CR's in responses by default
        sensor.filterCR(true);

        System.out.println("Configuring modem...");

        // discard any waiting characters
        sensor.drain();

        // reset modem
        sensor.command("ATZ\r");

        // turn off command echo, set verbosity to 1, enable data
        // connection mode
        sensor.command("ATE0 V1 +FCLASS=0\r");
        sensor.drain();

        // Now issue some commands and output the results.

        System.out.println("Modem and SIM information:");

        String buffer;

        buffer = sensor.commandWithResponse("AT+ICCID\r", bufferLength);
        if (buffer.length() > 0)
        {
            System.out.println("ICCID (SIM ID): " + buffer);
        }

        buffer = sensor.commandWithResponse("AT+CGSN=1\r", bufferLength);
        if (buffer.length() > 0)
        {
            System.out.println("IMEI: " + buffer);
        }

        // see if we are on the network....
        buffer = sensor.commandWithResponse("AT+CREG?\r", bufferLength);
        if (buffer.length() > 0)
        {
            System.out.println(buffer);

            // look for "CGREG: 0,1" or "CGREG: 0,5"
            if (sensor.find(buffer, "CREG: 0,1") ||
                sensor.find(buffer, "CREG: 0,5"))
            {
                System.out.println("Connected to the cell data network.");

                // wait up to 5 seconds for responses now...
                sensor.setResponseWaitTime(5000);

                // setup PDP context (socket 1).  An ERROR repsonse is
                // possible if the PDP context is already set up.
                sensor.command("AT#SGACT=1,1\r");

                // setup a TCP socket to nist.gov and read the timestamp.

                System.out.println("Connecting to time-a.nist.gov, TCP port 13");

                // Wait up to 60 seconds to find the NO CARRIER
                // string, which will be present at the end, if the
                // connection succeeded and the requested data was
                // obtained.
                buffer =
                    sensor.commandWaitFor("AT#SD=1,0,13,\"time-a.nist.gov\"\r",
                                          bufferLength, "\nNO CARRIER\n", 60000);
                if (buffer.length() > 0)
                {
                    // print out the response
                    System.out.println("RESPONSE: ");
                    System.out.println(buffer);
                }
                else
                {
                    System.out.println("No response.");
                }

                // destroy PDP context
                sensor.setResponseWaitTime(250);
                sensor.command("AT#SGACT=1,0\r");
            }
            else
            {
                System.out.println("You do not appear to be connected to the network...");
            }
        }
        else
        {
            System.out.println("Error executing query\n");
        }

        // reset the modem
        sensor.command("ATZ\r");

        System.out.println("Exiting");

// ! [Interesting]
    }
}
