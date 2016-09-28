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
#include <stdio.h>
#include <signal.h>

#include "ds18b20.h"
#include "upm_utilities.h"

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

    printf("Initializing...\n");

    // Instantiate an DS18B20 instance using the uart 0
    ds18b20_context sensor = ds18b20_init(0);

    if (!sensor)
    {
        printf("ds18b20_init() failed.\n");
        return(1);
    }

    printf("Found %d device(s)\n\n", ds18b20_devices_found(sensor));

    // update and print available values every second
    while (shouldRun)
    {
        // update our values for all sensors
        ds18b20_update(sensor, -1);

        int i;
        for (i=0; i<ds18b20_devices_found(sensor); i++)
        {
            printf("Device %02d: Temperature: %f C\n",
                    i, ds18b20_get_temperature(sensor, i));
        }
        printf("\n");

        upm_delay(2);
    }

    printf("Exiting...\n");

    ds18b20_close(sensor);
    //! [Interesting]

    return 0;
}
