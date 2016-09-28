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

#include "mma7361.h"
#include "upm_utilities.h"

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]

    // Instantiate a MMA7361 sensor on analog pins A0 (X), A1 (Y) A2
    // (Z), selftest pin on D2, sleep pin on D3 nd an analog reference
    // value of 5.0.  The freefall pin and the range pin are unused
    // (-1).
    mma7361_context sensor = mma7361_init(0, 1, 2, 2, 3, -1, -1, 5.0);

    if (!sensor)
    {
        printf("mma7361_init() failed.\n");
        return(1);
    }

    // 1.5g (true = 6g)
    mma7361_set_range(sensor, false);

    // Every 10th of a second, update and print values

    while (shouldRun)
    {
        mma7361_update(sensor);

        float x, y, z;

        mma7361_get_acceleration(sensor, &x, &y, &z);
        printf("Acceleration x = %f y = %f z = %f\n",
                x, y, z);

        mma7361_get_volts(sensor, &x, &y, &z);
        printf("Volts x = %f y = %f z = %f\n\n",
                x, y, z);

        upm_delay_ms(500);
    }

    //! [Interesting]

    printf("Exiting...\n");

    mma7361_close(sensor);

    return 0;
}
