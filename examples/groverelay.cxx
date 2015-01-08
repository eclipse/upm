/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
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
#include "grove.h"

int
main(int argc, char **argv)
{
    // This example uses GPIO 0
//! [Interesting]

    // Create the relay switch object using GPIO pin 0
    upm::GroveRelay* relay = new upm::GroveRelay(0);

    // Close and then open the relay switch 3 times,
    // waiting one second each time.  The LED on the relay switch
    // will light up when the switch is on (closed).
    // The switch will also make a noise between transitions.
    for ( int i = 0; i < 3; i++ ) {
        relay->on();
        if ( relay->isOn() ) 
            std::cout << relay->name() << " is on" << std::endl;
        sleep(1);
        relay->off();
        if ( relay->isOff() ) 
            std::cout << relay->name() << " is off" << std::endl;
        sleep(1);
    }

    // Delete the relay switch object
    delete relay;
//! [Interesting]

    return 0;
}
