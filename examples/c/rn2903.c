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
#include <stdio.h>
#include <string.h>

#include "rn2903.h"
#include "upm_utilities.h"
#include "upm_platform.h"

#if defined(UPM_PLATFORM_ZEPHYR) && !defined(CONFIG_STDOUT_CONSOLE)
# define printf printk
#endif

int main(int argc, char **argv)
{
//! [Interesting]

    char *defaultDev = "/dev/ttyUSB0";
    if (argc > 1)
        defaultDev = argv[1];

    printf("Using device: %s\n", defaultDev);

    // Instantiate a RN2903 sensor on defaultDev at 57600 baud.
#if defined(UPM_PLATFORM_ZEPHYR)
    rn2903_context sensor = rn2903_init(0, RN2903_DEFAULT_BAUDRATE);
#else
    rn2903_context sensor = rn2903_init_tty(defaultDev,
                                            RN2903_DEFAULT_BAUDRATE);
#endif

    // To use an internal UART understood by MRAA, use the following
    // to inititialize rather than the above, which by default uses a
    // tty path.
    //
    // rn2903_context sensor = rn2903_init(0, RN2903_DEFAULT_BAUDRATE);

    if (!sensor)
    {
        printf("rn2903_init_tty() failed.\n");
        return 1;
    }

    // enable debugging
    // rn2903_set_debug(sensor, true);

    // get version
    if (rn2903_command(sensor, "sys get ver"))
    {
        printf("Failed to retrieve device version string\n");
        rn2903_close(sensor);
        return 1;
    }
    printf("Firmware version: %s\n", rn2903_get_response(sensor));

    printf("Hardware EUI: %s\n", rn2903_get_hardware_eui(sensor));

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
    // rn2903_set_device_eui(sensor, "0011223344556677");
    // rn2903_set_application_eui(sensor, "0011223344556677");
    // rn2903_set_application_key(sensor, "01234567012345670123456701234567");
    //
    // RN2903_JOIN_STATUS_T rv = rn2903_join(sensor, RN2903_JOIN_TYPE_OTAA);
    // A successful join will return RN2903_JOIN_STATUS_ACCEPTED (0).
    // printf("JOIN: got rv %d\n", rv);

    // Try an ABP join.  Note, these parameters are made up.  For a
    // real network, you will want to use the correct values
    // obviously.  For an ABP join, you need to supply the Device
    // Address, Network Session Key, and the Application Session Key.

    rn2903_set_device_addr(sensor, "00112233");
    rn2903_set_network_session_key(sensor, "00112233001122330011223300112233");
    rn2903_set_application_session_key(sensor,
                                       "00112233001122330011223300112233");
    RN2903_JOIN_STATUS_T rv = rn2903_join(sensor, RN2903_JOIN_TYPE_ABP);
    if (rv == RN2903_JOIN_STATUS_ACCEPTED)
    {
        printf("Join successful.\n");

        // All transmit payloads must be hex encoded strings, so
        // pretend we have a temperature sensor that gave us a value
        // of 25.6 C, and we want to transmit it.
        const char *faketemp = "25.6";
        printf("Transmitting a packet....\n");

        RN2903_MAC_TX_STATUS_T trv;
        trv = rn2903_mac_tx(sensor, RN2903_MAC_MSG_TYPE_UNCONFIRMED,
                            1, // port number
                            rn2903_to_hex(sensor, faketemp, strlen(faketemp)));

        if (trv == RN2903_MAC_TX_STATUS_TX_OK)
            printf("Transmit successful.\n");
        else
        {
            // check to see if we got a downlink packet
            if (trv == RN2903_MAC_TX_STATUS_RX_RECEIVED)
            {
                printf("Transmit successful, downlink packet received: %s\n",
                       rn2903_get_response(sensor));
            }
            else
            {
                printf("Transmit failed with code %d.\n", trv);
            }
        }
    }
    else
    {
        printf("Join failed with code %d.\n", rv);
    }

    printf("Exiting\n");

    rn2903_close(sensor);

//! [Interesting]

    return 0;
}
