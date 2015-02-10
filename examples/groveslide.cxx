/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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
#include <iomanip>
#include "grove.h"

using namespace std;

int main ()
{
//! [Interesting]
    upm::GroveSlide* slide = new upm::GroveSlide(0);    // Instantiate new grove slide potentiometer on analog pin A0

    cout << slide->name() << endl;

    while(true) {
        float adc_value = slide->raw_value();       // Read raw value
        float volts = slide->voltage_value();    // Read voltage, board reference set at 5.0V
        fprintf(stdout, "%4d = %.2f V\n", (uint16_t)adc_value, volts);

        usleep(2500000);    // Sleep for 2.5s
    }
//! [Interesting]
    delete slide;
    return 0;
}
