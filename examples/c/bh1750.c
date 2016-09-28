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

#include "bh1750.h"
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

    // Instantiate a BH1750 sensor on default I2C bus (0), using the
    // default I2C address (0x23), and setting the mode to highest
    // resolution, lowest power mode.
    bh1750_context sensor = bh1750_init(BH1750_DEFAULT_I2C_BUS,
            BH1750_DEFAULT_I2C_ADDR,
            BH1750_OPMODE_H2_ONCE);

    if (!sensor)
    {
        printf("bh1750_init() failed.\n");
        return 1;
    }

    // Every second, sample the BH1750 and output the measured lux value

    while (shouldRun)
    {
        float lux;

        bh1750_get_lux(sensor, &lux);
        printf("Detected Light Level (lux): %f\n", lux);
        upm_delay(1);
    }

    //! [Interesting]

    printf("Exiting\n");

    bh1750_close(sensor);

    return 0;
}
