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
#include <stdio.h>
#include <signal.h>

#include <upm_utilities.h>
#include "nunchuck.h"

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
    // Instantiate a nunchuck controller bus 3
    nunchuck_context sensor = nunchuck_init(3);

    if (!sensor)
    {
        printf("%s: nunchuck_init() failed\n", __FUNCTION__);
        return 1;
    }

    while (shouldRun)
    {
        if (nunchuck_update(sensor))
        {
            printf("%s: nunchuck_update() failed\n", __FUNCTION__);
            nunchuck_close(sensor);
            return 1;
        }

        int x, y, z;
        nunchuck_get_stick(sensor, &x, &y);
        printf("stickX: %d stickY: %d\n", x, y);

        nunchuck_get_acceleration(sensor, &x, &y, &z);
        printf("accelX: %d accelY: %d accelZ: %d\n", x, y, z);

        bool bc, bz;
        nunchuck_get_buttons(sensor, &bc, &bz);
        printf("button C: %s\n",
               ((bc) ? "pressed" : "not pressed"));
        printf("button Z: %s\n\n",
               ((bz) ? "pressed" : "not pressed"));

        upm_delay_ms(100);
    }

    nunchuck_close(sensor);

    //! [Interesting]
    return 0;
}
