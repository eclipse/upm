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
#include "kxcjk1013.hpp"

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
    cout << fixed << setprecision(1);
    cout << x << '\t' << y << '\t' << z << "[m/s^2]" << endl;
}

int
main()
{
    signal(SIGINT, sig_handler);
    //! [Interesting]
    // Instantiate a KXCJK1013 Accelerometer Sensor on iio device 0
    accelerometer = new upm::KXCJK1013(0);
    // Available scales are 0.009582(2g), 0.019163(4g), and 0.038326(8g)
    accelerometer->setScale(0.019163);
    // Available sampling frequency are 0.781000, 1.563000, 3.125000, 6.250000, 12.500000, 25, 50,
    // 100, 200, 400, 800, and 1600
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
