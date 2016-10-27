/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "upm_utilities.h"
#include "mma7660.h"

int shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int main(int argc, char **argv)
{
    signal(SIGINT, sig_handler);

//! [Interesting]
    // Instantiate an MMA7660 on I2C bus 0

    mma7660_context accel = mma7660_init(MMA7660_DEFAULT_I2C_BUS,
                                         MMA7660_DEFAULT_I2C_ADDR);

    if (!accel)
    {
        printf("mma7660_init() failed\n");
        return 1;
    }

    // place device in standby mode so we can write registers
    mma7660_set_mode_standby(accel);

    // enable 64 samples per second
    mma7660_set_sample_rate(accel, MMA7660_AUTOSLEEP_64);

    // place device into active mode
    mma7660_set_mode_active(accel);

    while (shouldRun)
    {
        float ax, ay, az;

        mma7660_get_acceleration(accel, &ax, &ay, &az);
        printf("Acceleration: x = %f y = %f z = %f\n\n",
               ax, ay, az);

        upm_delay_ms(500);
    }

    printf("Exiting...\n");

    mma7660_close(accel);

//! [Interesting]
    return 0;
}
