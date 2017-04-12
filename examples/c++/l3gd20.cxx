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

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <signal.h>
#include "l3gd20.hpp"

using namespace std;

int shouldRun = true;
upm::L3GD20* gyroscope;

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
    if (gyroscope->extract3Axis(data, &x, &y, &z)) {
        cout << fixed << setprecision(1);
        cout << x << '\t' << y << '\t' << z << "[rad/sec]" << endl;
    }
}

int
main()
{
    signal(SIGINT, sig_handler);
    //! [Interesting]
    // Instantiate a L3GD20 Gyroscope Sensor on iio device 3
    gyroscope = new upm::L3GD20(3);
    // Available scales are 0.000153(250dps), 0.000305(500dps), and 0.001222(2000dps)
    gyroscope->setScale(0.001222);
    // Available sampling frequency are 95, 190, 380, and 760
    gyroscope->setSamplingFrequency(95.0);
    gyroscope->enable3AxisChannel();
    gyroscope->installISR(data_callback, NULL);
    gyroscope->enableBuffer(16);

    while (shouldRun) {
        sleep(1);
    }
    gyroscope->disableBuffer();

    //! [Interesting]
    cout << "Exiting" << endl;

    delete gyroscope;

    return 0;
}
