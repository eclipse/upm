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

const size_t bufferLength = 128;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]

    // Instantiate a NMEA_GPS UBLOX based i2c sensor on i2c bus 0 at
    // address 0x42
    nmea_gps_context sensor = nmea_gps_init_ublox_i2c(0, 0x42);

    if (!sensor)
    {
        printf("nmea_gps_init_ublox_i2c() failed.\n");
        return 1;
    }

    char buffer[bufferLength];
    int rv = 0;

    // loop, dumping NMEA data out as fast as it comes in
    while (shouldRun)
    {
        if (!nmea_gps_data_available(sensor, 0))
            upm_delay_ms(500);
        else
        {
            if ((rv = nmea_gps_read(sensor, buffer, bufferLength)) >= 0)
            {
                int i;
                for (i=0; i<rv; i++)
                    printf("%c", buffer[i]);
            }
        }
    }

    //! [Interesting]

    printf("Exiting\n");

    nmea_gps_close(sensor);

    return 0;
}
