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
#include <signal.h>

#include "rn2903.h"
#include "upm_utilities.h"
#include "upm_platform.h"

int shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

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

    // For this example, we will just try to receive a packet
    // transmitted by the p2p-tx rn2903 example.  We reset the
    // device to defaults, and we do not make any adjustments to the
    // radio configuration.  You will probably want to do so for a
    // real life application.

    // The first thing to do is to suspend the LoRaWAN stack on the device.
    if (rn2903_mac_pause(sensor))
    {
        printf("Failed to pause the LoRaWAN stack\n");
        rn2903_close(sensor);
        return 1;
    }

    // We will use continuous mode (window_size 0), though the default
    // radio watch dog timer will expire every 15 seconds.  We will
    // just loop here.
    while (shouldRun)
    {
        printf("Waiting for packet...\n");
        RN2903_RESPONSE_T rv;
        rv = rn2903_radio_rx(sensor, 0);
        if (rv)
        {
            printf("rn2903_radio_rx() failed with code (%d)\n", rv);
        }
        else
        {
            const char *resp = rn2903_get_response(sensor);
            const char *payload = rn2903_get_radio_rx_payload(sensor);
            if (!payload)
                printf("Got response: '%s'\n", resp);
            else
                printf("Got payload: '%s'\n",
                       rn2903_from_hex(sensor, payload));
        }

        printf("\n");
    }

    printf("Exiting\n");

    rn2903_close(sensor);

//! [Interesting]

    return 0;
}
