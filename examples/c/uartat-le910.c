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

#include <unistd.h>

#include "uartat.h"
#include "upm_utilities.h"

const size_t bufferLength = 256;

int main(int argc, char **argv)
{
//! [Interesting]

    char *defaultDev = "/dev/ttyUSB0";
    if (argc > 1)
        defaultDev = argv[1];

    printf("Using device: %s\n", defaultDev);
    // Instantiate a UARTAT sensor on defaultDev at 115200 baud.
    uartat_context sensor = uartat_init_tty(defaultDev, 115200);

    if (!sensor)
    {
        printf("uartat_init_tty() failed.\n");
        return 1;
    }

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
    // much more sophisticated, and custom designed for the desired
    // task.
    //
    // For experimenting with various AT commands, try using an
    // interactive terminal emulator like minicom or screen.

    char buffer[bufferLength];

    // make sure we are in command mode
    if (!uartat_in_command_mode(sensor))
    {
        printf("Not in command mode, switching...\n");
        uartat_command_mode(sensor, "+++", 1000);
    }

    // flter out CR's in responses by default
    uartat_filter_cr(sensor, true);

    printf("Configuring modem...\n");

    // discard any waiting characters
    uartat_drain(sensor);

    // reset modem
    uartat_command(sensor, "ATZ\r");

    // turn off command echo, set verbosity to 1, enable data
    // connection mode
    uartat_command(sensor, "ATE0 V1 +FCLASS=0\r");
    uartat_drain(sensor);

    // Now issue some commands and output the results.

    printf("Modem and SIM information:\n");
    if (uartat_command_with_response(sensor, "AT+ICCID\r",
                                      buffer, bufferLength))
        printf("ICCID (SIM ID): %s\n", buffer);

    if (uartat_command_with_response(sensor, "AT+CGSN=1\r",
                                      buffer, bufferLength))
        printf("IMEI: %s\n", buffer);

    // see if we are on the network....
    if (uartat_command_with_response(sensor, "AT+CREG?\r",
                                      buffer, bufferLength))
    {
        printf("%s\n", buffer);
        // look for "CGREG: 0,1" or "CGREG: 0,5"
        if (uartat_find(sensor, buffer, "CREG: 0,1") ||
            uartat_find(sensor, buffer, "CREG: 0,5"))
        {
            printf("Connected to the cell data network.\n");

            // wait up to 5 seconds for responses now...
            uartat_set_response_wait_time(sensor, 5000);

            // setup PDP context (socket 1).  An ERROR response is
            // possible if the PDP context is already set up.
            uartat_command(sensor, "AT#SGACT=1,1\r");

            // setup a TCP socket to nist.gov and read the timestamp.

            printf("Connecting to time-a.nist.gov, TCP port 13\n");

            // Wait up to 60 seconds to find the NO CARRIER
            // string, which will be present at the end, if the
            // connection succeeded and the requested data was
            // obtained.
            if (uartat_command_waitfor(sensor,
                                       "AT#SD=1,0,13,\"time-a.nist.gov\"\r",
                                       buffer, bufferLength,
                                       "\nNO CARRIER\n", 60000))
            {
                // print out the response
                printf("RESPONSE:\r%s\r", buffer);
            }
            else
            {
                printf("No response.\n");
            }

            // destroy PDP context
            uartat_set_response_wait_time(sensor, 250);
            uartat_command(sensor, "AT#SGACT=1,0\r");
        }
        else
        {
            printf("You do not appear to be connected to the network...\n");
        }
    }
    else
    {
        printf("Error executing query\n");
    }

    // reset the modem
    uartat_command(sensor, "ATZ\r");

    printf("Exiting\n");

    uartat_close(sensor);

//! [Interesting]

    return 0;
}
