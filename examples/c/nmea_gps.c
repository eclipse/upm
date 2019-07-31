/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <signal.h>

#include "nmea_gps.h"
#include "upm_utilities.h"

bool shouldRun = true;

const size_t bufferLength = 256;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]

    // Instantiate a NMEA_GPS sensor on uart 0 at 9600 baud with enable
    // pin on D3.  If you do not need an enable pin, you can specify -1.
    nmea_gps_context sensor = nmea_gps_init(0, 9600, 3);

    if (!sensor)
    {
        printf("nmea_gps_init() failed.\n");
        return 1;
    }

    char buffer[bufferLength];
    int rv = 0;

    // loop, dumping NMEA data out as fast as it comes in
    while (shouldRun && nmea_gps_data_available(sensor, 5000))
    {
        if ((rv = nmea_gps_read(sensor, buffer, bufferLength)) >= 0)
        {
            int i;
            for (i=0; i<rv; i++)
                printf("%c", buffer[i]);
        }
    }

    if (shouldRun)
        printf("Timed out\n");

    //! [Interesting]

    printf("Exiting\n");

    nmea_gps_close(sensor);

    return 0;
}
