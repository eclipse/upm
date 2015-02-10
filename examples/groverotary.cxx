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
    // Instantiate a rotary sensor on analog pin A0
    upm::GroveRotary* knob = new upm::GroveRotary(0);

    // Print sensor name to confirm it initialized properly
    cout << knob->name() << endl;

    while(true) {
        float abs_value = knob->abs_value(); // Absolute raw value
        float abs_deg = knob->abs_deg();     // Absolute degrees
        float abs_rad = knob->abs_rad();     // Absolute radians
        float rel_value = knob->rel_value(); // Relative raw value
        float rel_deg = knob->rel_deg();     // Relative degrees
        float rel_rad = knob->rel_rad();     // Relative radians

        fprintf(stdout, "Absolute: %4d raw %5.2f deg = %3.2f rad Relative: %4d raw %5.2f deg %3.2f rad\n",
                (int16_t)abs_value, abs_deg, abs_rad, (int16_t)rel_value, rel_deg, rel_rad);

        usleep(2500000); // Sleep for 2.5s
    }
//! [Interesting]
    delete knob;
    return 0;
}
