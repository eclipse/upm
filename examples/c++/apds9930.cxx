/*
 * Author: Lay, Kuan Loon <kuan.loon.lay@intel.com>
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
#include <iostream>
#include <signal.h>
#include "apds9930.hpp"

using namespace std;

int shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int
main()
{
    signal(SIGINT, sig_handler);
    //! [Interesting]
    // Instantiate a Digital Proximity and Ambient Light sensor on iio device 4
    upm::APDS9930* light_proximity = new upm::APDS9930(4);

    // Kernel driver implement sleep 5000-5100us after enable illuminance sensor
    light_proximity->enableIlluminance(true);

    // Kernel driver implement sleep 5000-5100us after enable proximity sensor
    light_proximity->enableProximity(true);

    // Tested this value works. Please change it on your platform
    usleep(120000);

    while (shouldRun) {
        float lux = light_proximity->getAmbient();
        cout << "Luminance value is " << lux << endl;
        float proximity = light_proximity->getProximity();
        cout << "Proximity value is " << proximity << endl;
        sleep(1);
    }
    light_proximity->enableProximity(false);
    light_proximity->enableIlluminance(false);
    //! [Interesting]

    cout << "Exiting" << endl;

    delete light_proximity;

    return 0;
}
