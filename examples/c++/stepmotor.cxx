/*
 * Authors: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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
#include "stepmotor.hpp"

using namespace std;

int doWork = 1;
upm::StepMotor *sensor = NULL;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
        doWork = 0;
    }
}

int
main(int argc, char **argv)
{
//! [Interesting]
    sensor = new upm::StepMotor(2, 3);

    while (doWork) {
        cout << "1 Revolution forward and back at 60 rpm" << endl;
        sensor->setSpeed(60);
        sensor->stepForward(200);
        usleep(1000000);
        sensor->stepBackward(200);
        usleep(1000000);

        cout << "1 Revolution forward and back at 150 rpm" << endl;
        sensor->setSpeed(150);
        sensor->stepForward(200);
        usleep(1000000);
        sensor->stepBackward(200);
        usleep(1000000);

        cout << "1 Revolution forward and back at 300 rpm" << endl;
        sensor->setSpeed(300);
        sensor->stepForward(200);
        usleep (1000000);
        sensor->stepBackward(200);
        usleep (1000000);
    }

    delete sensor;
//! [Interesting]
    return 0;
}
