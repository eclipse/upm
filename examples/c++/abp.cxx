/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
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
#include "abp.hpp"

using namespace std;

int shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}


int main ()
{
    signal(SIGINT, sig_handler);

//! [Interesting]
    // Instantiate an ABP sensor on i2c bus 0
    upm::ABP* abp = new upm::ABP(0, ABP_DEFAULT_ADDRESS);
    while (shouldRun) {
        abp->update();
        cout << "Retrieved pressure: " << abp->getPressure() << endl;
        cout << "Retrieved Temperature: " << abp->getTemperature() << endl;

        sleep(1);
    }
//! [Interesting]

    cout << "Exiting..." << endl;

    delete abp;
    return 0;
}
