/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Sarah Knepper <sarah.knepper@intel.com>
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

int
main(int argc, char **argv)
{
//! [Interesting]

    // Create the temperature sensor object using AIO pin 0
    upm::GroveTemp* temp = new upm::GroveTemp(0);
    std::cout << temp->name() << std::endl;

    // Read the temperature ten times, printing both the Celsius and
    // equivalent Fahrenheit temperature, waiting one second between readings
    for (int i=0; i < 10; i++) {
        int celsius = temp->value();
        int fahrenheit = (int) (celsius * 9.0/5.0 + 32.0);
        printf("%d degrees Celsius, or %d degrees Fahrenheit\n",
                celsius, fahrenheit);
        sleep(1);
    }

    // Delete the temperature sensor object
    delete temp;
//! [Interesting]

    return 0;
}
