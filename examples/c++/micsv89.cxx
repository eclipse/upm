/*
 * Author: Marc Graham <marc@m2ag.net>
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

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "micsv89.h"

/*
 * An example for using the MICSV89 sensor library.
 * The MICSV89 comes in 4 variants, PWM and I2c
 * in 3.3 volts and 5 volts. This library only implements
 * the I2c version of the device.
 *
 * Device output is not valid until a warm up of 15 minutes
 * of operation.
 *
 * Additional linker flags: -lupm-micsv89
 */

using namespace std;

volatile int running = 1;

void
sig_handler(int signo)
{
    if (signo == SIGINT) {
        cout << "Exiting program." << endl;
        running = 0;
    }
}

int main()
{
    signal(SIGINT, sig_handler);

//! [Interesting]
    upm::MICSV89 *sensor = new upm::MICSV89(6);

    while(running)
    {
        sensor->update();
        while(!sensor->valid());
        cout << "co2: " << sensor->co2equ() << endl;
        cout << "short: " << sensor->vocshort() << endl;
        cout << "tvoc: " << sensor->tvoc() << endl;
        cout << "resistor: " << sensor->resistor() << endl;
        cout << "****************************" << endl;
        sleep(5);
    }

    delete sensor;
//! [Interesting]

    return MRAA_SUCCESS;
}
