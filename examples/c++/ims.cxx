/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2016 Intel Corporation.
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

#include "ims.hpp"
#include "ims_defs.h"
#include "upm_utilities.h"

using namespace upm;

int shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);
    //! [Interesting]

    // Instantiate a IMS instance using i2c bus 0 and default address
    upm::IMS sensor(0);

    int i2c_addr_cur = IMS_ADDRESS_DEFAULT + 1;
    while (shouldRun) {
        std::cout << std::hex << "Version: 0x" << sensor.get_version() << " light: 0x"
                  << sensor.get_light() << " moisture: 0x" << sensor.get_moisture()
                  << " temp: " << sensor.get_temperature() << " C" << std::endl;

        // Change the address and continue
        if (i2c_addr_cur >= 128)
            i2c_addr_cur = 1;
        std::cout << "Changing device address to 0x" << i2c_addr_cur << std::endl;
        sensor.reset_i2c_address(i2c_addr_cur++);
        upm_delay(1);
    }
    //! [Interesting]

    std::cout << "Exiting..." << std::endl;

    return 0;
}
