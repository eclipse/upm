/*
 * Author: William Penner <william.penner@intel.com>
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
#include <signal.h>

#include "mpl3115a2.h"

volatile int doWork = 0;

upm::MPL3115A2 *sensor = NULL;

void
sig_handler(int signo)
{
    if (signo == SIGINT) {
        printf("\nCtrl-C received.\n");
        doWork = 1;
    }
}

int
main(int argc, char **argv)
{
    // Register signal handler
    signal(SIGINT, sig_handler);

    //! [Interesting]
    float pressure    = 0.0;
    float temperature = 0.0;
    float altitude    = 0.0;
    float sealevel    = 0.0;

    sensor = new upm::MPL3115A2(0, MPL3115A2_I2C_ADDRESS);

    sensor->testSensor();

    while (!doWork) {
        temperature = sensor->getTemperature(true);
        pressure    = sensor->getPressure(false);
        altitude    = sensor->getAltitude();
        sealevel    = sensor->getSealevelPressure();

        std::cout << "pressure value = " <<
                    pressure <<
                    ", altitude value = " <<
                    altitude <<
                    ", sealevel value = " <<
                    sealevel <<
                    ", temperature = " <<
                    temperature << std::endl;
        usleep (500000);
    }
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    delete sensor;

    return 0;
}
