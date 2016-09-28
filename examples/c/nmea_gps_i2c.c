/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
