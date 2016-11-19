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

#include <upm_utilities.h>
#include <ecezo.h>

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

    // Instantiate a ECEZO sensor on uart 0 at 9600 baud.
    ecezo_context sensor = ecezo_uart_init(0, 9600);

    // For I2C, assuming the device is configured for address 0x64 on
    // I2C bus 0, you could use something like:
    //
    // ecezo_context sensor = ecezo_i2c_init(0, 0x64);

    if (!sensor)
    {
        printf("ecezo_init() failed.\n");
        return 1;
    }

    while (shouldRun)
    {
        // this will take about 1 second to complete
        if (ecezo_update(sensor))
        {
            printf("ecezo_update() failed\n");
        }
        else
        {
            printf("EC %f uS/cm, TDS %f mg/L, Salinity %f PSS-78, SG %f\n",
                   ecezo_get_ec(sensor),
                   ecezo_get_tds(sensor),
                   ecezo_get_salinity(sensor),
                   ecezo_get_sg(sensor));
        }

        upm_delay(5);
    }

    //! [Interesting]

    printf("Exiting\n");

    ecezo_close(sensor);

    return 0;
}
