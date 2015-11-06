/*
 * Author: Lay, Kuan Loon <kuan.loon.lay@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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
#include "kxcjk1013.h"

using namespace std;

int shouldRun = true;
upm::KXCJK1013* accelerometer;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

void
data_callback(char* data)
{
    float x, y, z;
    accelerometer->extract3Axis(data, &x, &y, &z);
    printf("%.1f               %.1f               %.1f\n", x, y, z);
    // usleep(100);
}

int
main()
{
    signal(SIGINT, sig_handler);
    //! [Interesting]
    // Instantiate a KXCJK1013 Accelerometer Sensor on iio device 0
    accelerometer = new upm::KXCJK1013(0);
    accelerometer->setScale(0.019163);
    accelerometer->setSamplingFrequency(25.0);
    accelerometer->enable3AxisChannel();
    accelerometer->installISR(data_callback, NULL);
    accelerometer->enableBuffer(16);

    while (shouldRun) {
        sleep(1);
    }
    accelerometer->disableBuffer();

    //! [Interesting]
    cout << "Exiting" << endl;

    delete accelerometer;

    return 0;
}
