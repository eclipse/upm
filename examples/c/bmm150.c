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
#include "bmm150.h"

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

#if defined(CONFIG_BOARD_ARDUINO_101_SSS)
    // ARDUINO_101_SSS (ARC core) must use I2C
    // Instantiate a BMM150 instance using default i2c bus and address
    bmm150_context sensor = bmm150_init(BMM150_DEFAULT_I2C_BUS,
                                        BMM150_DEFAULT_ADDR, -1);
#elif defined(CONFIG_BOARD_ARDUINO_101)
    // ARDUINO_101 (Quark core) where you must use SPI
    // Instantiate a BMM150 instance using default SPI bus and pin 10 as CS
    bmm150_context sensor = bmm150_init(BMM150_DEFAULT_SPI_BUS,
                                        -1, 10);
#else
    // everything else use I2C by default
    // Instantiate a BMM150 instance using default i2c bus and address
    bmm150_context sensor = bmm150_init(BMM150_DEFAULT_I2C_BUS,
                                        BMM150_DEFAULT_ADDR, -1);
#endif

    if (!sensor)
    {
        printf("bmm150_init() failed.\n");
        return 1;
    }

    // now output data every 250 milliseconds
    while (shouldRun)
    {
        float x, y, z;

        if (bmm150_update(sensor))
        {
            printf("bmm150_update() failed\n");
            return 1;
        }

        bmm150_get_magnetometer(sensor, &x, &y, &z);
        printf("Magnetometer x: %f y: %f z: %f uT\n",
               x, y, z);

        upm_delay_ms(250);
    }

    printf("Exiting...\n");

    bmm150_close(sensor);

//! [Interesting]

    return 0;
}
