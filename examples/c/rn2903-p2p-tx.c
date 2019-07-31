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

bool shouldRun = true;

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

    // For this example, we will just try transmitting a packet over
    // LoRa.  We reset the device to defaults, and we do not make any
    // adjustments to the radio configuration.  You will probably want
    // to do so for a real life application.

    // The first thing to do is to suspend the LoRaWAN stack on the device.
    if (rn2903_mac_pause(sensor))
    {
        printf("Failed to pause the LoRaWAN stack\n");
        rn2903_close(sensor);
        return 1;
    }

    // the default radio watchdog timer is set for 15 seconds, so we
    // will send a packet every 10 seconds.  In reality, local
    // restrictions limit the amount of time on the air, so in a real
    // implementation, you would not want to send packets that
    // frequently.

    int count = 0;
    while (shouldRun)
    {
        char pingbuf[32] = {};
        snprintf(pingbuf, 32, "Ping %d", count++);
        // All payloads must be hex encoded
        const char *payload = rn2903_to_hex(sensor, pingbuf, strlen(pingbuf));

        printf("Transmitting a packet, data: '%s' -> hex: '%s'\n",
               pingbuf, payload);

        RN2903_RESPONSE_T rv;
        rv = rn2903_radio_tx(sensor, payload);

        if (rv == RN2903_RESPONSE_OK)
            printf("Transmit successful.\n");
        else
            printf("Transmit failed with code %d.\n", rv);

        printf("\n");
        upm_delay(10);
    }

    printf("Exiting\n");

    rn2903_close(sensor);

//! [Interesting]

    return 0;
}
