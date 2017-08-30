/* Author: Norbert Wesp <nwesp@phytec.de>
 * Copyright (c) 2017 Phytec Messtechnik GmbH.
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
#include <signal.h>

#include "mag3110.hpp"
#include "upm_utilities.h"

using namespace upm;

bool run = true;

void
sig_handler(int sig)
{
    if (sig == SIGINT)
        run = false;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    upm::mag3110_data_t data;

    std::cout << "Initializing test-application..." << std::endl;

    // Instantiate an MAG3110 instance on bus 1
    upm::MAG3110 mySensor(1);

    // activate periodic measurements
    mySensor.setActive();

    // update and print available values every second
    while (run) {
        mySensor.getData(&data, true);
        std::cout << "x: " << data.x << std::endl
                  << "y: " << data.y << std::endl
                  << "z: " << data.z << std::endl
                  << "Status: " << data.status << std::endl
                  << "Die temperature: " << data.dtemp << std::endl;

        std::cout << std::endl;

        upm_delay(1);
    }

    std::cout << "Exiting test-application..." << std::endl;

    //! [Interesting]

    return 0;
}
