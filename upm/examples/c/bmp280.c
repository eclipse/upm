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
#include "bmp280.h"

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

    // Instantiate a BMP280 instance using default i2c bus and address
    bmp280_context sensor = bmp280_init(BMP280_DEFAULT_I2C_BUS,
                                        BMP280_DEFAULT_ADDR, -1);

    if (!sensor)
    {
        printf("bmp280_init() failed\n");
        return 1;
    }

    // For SPI, bus 0, you would pass -1 as the address, and a valid pin for CS:
    // bmp280_init(BMP280_DEFAULT_SPI_BUS,
    //             -1, 10)

    while (shouldRun)
    {
        // update our values from the sensor
        if (bmp280_update(sensor))
        {
            printf("bmp280_update() failed\n");
            bmp280_close(sensor);
            return 1;
        }

        printf("Compensation Temperature: %f C\n",
               bmp280_get_temperature(sensor));
        printf("Pressure: %f Pa\n", bmp280_get_pressure(sensor));
        printf("Computed Altitude: %f m\n\n", bmp280_get_altitude(sensor));

        upm_delay(1);
    }

    printf("Exiting...\n");

    bmp280_close(sensor);

//! [Interesting]
    return 0;
}
