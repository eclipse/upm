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

#include "hka5.h"
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

    // Instantiate a HKA5 sensor on uart 0.  We don't use the set or
    // reset pins, so we pass -1 for them.
    hka5_context sensor = hka5_init(0, -1, -1);

    if (!sensor)
    {
        printf("hka5_init() failed.\n");
        return 1;
    }

    // update once every 2 seconds and output data
    while (shouldRun)
    {
        if (hka5_update(sensor) != UPM_SUCCESS)
        {
            printf("hka5_update() failed, exiting.\n");
            shouldRun = false;
        }

        printf("PM 1  : %d ug/m3\n", hka5_get_pm1(sensor));
        printf("PM 2.5: %d ug/m3\n", hka5_get_pm2_5(sensor));
        printf("PM 10 : %d ug/m3\n", hka5_get_pm10(sensor));
        printf("\n");

        upm_delay(2);
    }

    //! [Interesting]

    printf("Exiting\n");

    hka5_close(sensor);

    return 0;
}
