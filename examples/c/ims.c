/*
 * Author: Noel Eck <noel.eck@intel.com>
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

#include "ims.h"
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
    ims_context* sensor = ims_init(0, IMS_ADDRESS_DEFAULT);

    if (!sensor)
    {
        printf("ims_init() failed\n");
        return 1;
    }

    // Every second, sample the sensor outputs
    while (shouldRun)
    {
        uint16_t version, light, moisture, temp;

        if (ims_get_version(sensor, &version) == UPM_SUCCESS &&
            ims_get_light(sensor, &light) == UPM_SUCCESS &&
            ims_get_moisture(sensor, &moisture) == UPM_SUCCESS &&
            ims_get_temperature(sensor, &temp) == UPM_SUCCESS)
            printf("Version: %d light: 0x%04x moisture: 0x%04x temp: %3.2f C\n",
                    version, light, moisture, temp/10.0);
        else
            break;

        upm_delay(1);
    }

    //! [Interesting]
    printf("Exiting\n");
    ims_close(sensor);

    return 0;
}
