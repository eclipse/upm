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

#include "max30100.h"
#include "upm_utilities.h"

bool shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

void my_sample_handler(max30100_value sample, void* arg)
{
    printf("My callback sample IR: %d R: %d\n",
            sample.IR, sample.R);
}

int main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    max30100_context* sensor = max30100_init(0);

    if (!sensor)
    {
        printf("max30100_init() failed\n");
        return -1;
    }

    /* Get the temperature */
    float temp;
    if (max30100_get_temperature(sensor, &temp) != UPM_SUCCESS)
    {
        printf("max30100_get_temperature failed\n");
        goto max30100_exit;
    }

    /* Get the version */
    uint16_t version;
    if (max30100_get_version(sensor, &version) != UPM_SUCCESS)
    {
        printf("max30100_get_version failed\n");
        goto max30100_exit;
    }

    printf("Temperature: %f C\n", temp);
    printf("Version: 0x%04x\n", version);

    /* Set high-res (50 Hz, 16-bit) */
    if (max30100_set_high_res(sensor, true) != UPM_SUCCESS)
    {
        printf("max30100_set_high_res failed\n");
        goto max30100_exit;
    }

    /* Set to sample SpO2 */
    if (max30100_set_mode(sensor, MAX30100_MODE_SPO2_EN) != UPM_SUCCESS)
    {
        printf("max30100_set_mode failed\n");
        goto max30100_exit;
    }

    /* Read continuously, stepping up the LED current every second,
     * us GPIO 0 as the interrupt pin */
    max30100_sample_continuous(sensor, 0, false, &my_sample_handler, sensor);
    for (int i = MAX30100_LED_CURRENT_0_0_MA;
         i <= MAX30100_LED_CURRENT_50_0_MA && shouldRun; i++)
    {
        /* Toggle the LED current */
        printf("Setting LED current = %d\n", i);

        if ( max30100_set_current(sensor, (MAX30100_LED_CURRENT)i,
                    (MAX30100_LED_CURRENT)i) != UPM_SUCCESS )
        {
            printf("max30100_set_current failed\n");
            goto max30100_exit;
        }

        upm_delay(1);
    }

    /* Read individual samples */
    for (int i = 0; i < 10; i++)
    {
        max30100_value samp;
        if (max30100_sample(sensor, &samp) != UPM_SUCCESS)
        {
            printf("max30100_sample failed\n");
            goto max30100_exit;
        }

        printf("Single value IR: %d R: %d\n", samp.IR, samp.R);
    }

max30100_exit:
    //! [Interesting]
    printf("Exiting\n");
    max30100_close(sensor);

    return 0;
}
