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
#include <math.h>
#include "mmc35240.h"

using namespace std;

int shouldRun = true;
upm::MMC35240* magnetometer;

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
    double azimuth;
    int level;
    magnetometer->extract3Axis(data, &x, &y, &z);
    /* calibrated level
     * UNRELIABLE = 0
     * ACCURACY_LOW = 1
     * ACCURACY_MEDIUM = 2
     * ACCURACY_HIGH = >=3
     */
    level = magnetometer->getCalibratedLevel();
    azimuth = 90 - atan(y/x) * 180 / M_PI;
    printf("[Calibrated Level:%d] [Azimuth:%d]    % .2f               % .2f               % .2f\n", level, (int)azimuth, x, y, z);
}

int
main()
{
    signal(SIGINT, sig_handler);
    //! [Interesting]
    // Instantiate a MMC35240 Magnetic Sensor on iio device 5
    magnetometer = new upm::MMC35240(5);

    magnetometer->setScale(0.001000);
    magnetometer->setSamplingFrequency(25.000000);
    magnetometer->enable3AxisChannel();
    magnetometer->installISR(data_callback, NULL);
    magnetometer->enableBuffer(16);

    while (shouldRun) {
        sleep(1);
    }
    magnetometer->disableBuffer();

    //! [Interesting]
    cout << "Exiting" << endl;

    delete magnetometer;

    return 0;
}
