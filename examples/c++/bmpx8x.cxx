/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <iostream>
#include "bmpx8x.h"
#include <signal.h>

int doWork = 0;
upm::BMPX8X *sensor = NULL;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
        doWork = 1;
    }
}

int
main(int argc, char **argv)
{
    //! [Interesting]
    uint32_t presure = 0;
    float temperature = 0;
    float altitude = 0;
    uint32_t sealevel = 0;

    // Instantiate a BMPX8X sensor on I2C
    sensor = new upm::BMPX8X(0, ADDR);

    // Print the pressure, altitude, sea level, and
    // temperature values every 0.1 seconds
    while (!doWork) {
        presure = sensor->getPressure ();
        temperature = sensor->getTemperature ();
        altitude = sensor->getAltitude ();
        sealevel = sensor->getSealevelPressure ();

        std::cout << "pressure value = " <<
                    presure <<
                    ", altitude value = " <<
                    altitude <<
                    ", sealevel value = " <<
                    sealevel <<
                    ", temperature = " <<
                    temperature << std::endl;
        usleep (100000);
    }
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    delete sensor;

    return 0;
}
