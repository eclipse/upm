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

#include "tmp006.hpp"
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
    std::cout << "Initializing test-application..." << std::endl;

    // please make sure that you have the right i2c address for your device
    // the correct range of addresses is 0x40 - 0x47
    // Instantiate an TMP006 instance on bus 1
    upm::TMP006 mySensor(1, TMP006_CONFIG_CR_DEF, TMP006_I2C_ADDRESS);

    // you can also get basic tmp007 functionality by initializing it as follows
    // upm::TMP006 mySensor(1, TMP006_CONFIG_CR_DEF, TMP007_I2C_ADDRESS);

    // activate periodic measurements
    mySensor.setActive();

    // update and print available values every second
    while (run) {
        // Print out temperature value in *C
        std::cout << "Temperature: " << mySensor.getTemperature(true) << " *C" << std::endl;

        upm_delay(1);
    }

    std::cout << "Exiting test-application..." << std::endl;

    //! [Interesting]

    return 0;
}
