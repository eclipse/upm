/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
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
#include <stdio.h>
#include <signal.h>

#include "upm_utilities.h"
#include "bmpx8x.h"

bool shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}


int main(int argc, char **argv)
{
    signal(SIGINT, sig_handler);
//! [Interesting]

    // Instantiate a BMPX8X instance using default i2c bus and address
    bmpx8x_context sensor = bmpx8x_init(BMPX8X_DEFAULT_I2C_BUS,
                                        BMPX8X_DEFAULT_I2C_ADDR);

    if (!sensor)
    {
        printf("bmpx8x_init() failed.\n");
        return 1;
    }

    // Print the pressure, altitude, sea level, and
    // temperature values every 0.5 seconds
    while (shouldRun)
    {
        if (bmpx8x_update(sensor))
        {
            printf("bmpx8x_update() failed\n");
            bmpx8x_close(sensor);
            return 1;
        }

        // assume sea level pressure is 101325 Pa.
        float altitude = bmpx8x_get_altitude(sensor, 101325);
        int   sealevel = bmpx8x_get_sealevel_pressure(sensor, altitude);

        printf("Pressure: %d Pa, Temperature: %f C, "
               "Altitude %f m, Sea level %d Pa\n",
               bmpx8x_get_pressure(sensor),
               bmpx8x_get_temperature(sensor),
               altitude,
               sealevel);

        upm_delay_ms(500);
    }

    printf("Exiting...\n");
    bmpx8x_close(sensor);

//! [Interesting]
    return 0;
}
