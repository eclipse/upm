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
#include <signal.h>

#include "vdiv.h"
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

    // Instantiate a sensor on analog pin A0
    vdiv_context sensor = vdiv_init(0, 5);

    if (!sensor)
    {
        printf("vdiv_init() failed.\n");
        return(1);
    }

    // Every half a second, sample the sensor output
    while (shouldRun)
    {
        float raw_volts = 0.0, computed_volts = 0.0;

        vdiv_get_raw_volts(sensor, &raw_volts);
        vdiv_get_computed_volts(sensor, &computed_volts);

        printf("Divide SW: %d ADC voltage: %0.03f Sensor voltage: %0.03f\n",
                vdiv_get_divsw(sensor), raw_volts, computed_volts);

        upm_delay_ms(500);
    }

    //! [Interesting]

    printf("Exiting\n");

    vdiv_close(sensor);

    return 0;
}
