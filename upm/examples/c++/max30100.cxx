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

#include "max30100.hpp"
#include "max30100_regs.h"
#include "upm_utilities.h"

using namespace upm;

int shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

// Example Callback handler
class mycallback : public Callback
{
  public:
    virtual void
    run(max30100_value samp)
    {
        std::cout << "My callback sample IR: " << samp.IR << " R: " << samp.R << std::endl;
    }
};

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);
    //! [Interesting]

    // Instantiate a MAX30100 instance using i2c bus 0
    upm::MAX30100 sensor(0);

    // Create an instance of the Callback class
    mycallback cb;

    // Read the temperature and version
    std::cout << "Temperature: " << sensor.temperature() << " C" << std::endl;
    std::cout << "Version: " << sensor.version() << std::endl;

    // Set high-res (50 Hz, 16-bit)
    sensor.high_res_enable(true);

    // Set to sample SpO2
    sensor.mode(MAX30100_MODE_SPO2_EN);

    // Read continuously, stepping up the LED current every second,
    // us GPIO 0 as the interrupt pin
    sensor.sample_continuous(0, false, &cb);
    for (int i = MAX30100_LED_CURRENT_0_0_MA; i <= MAX30100_LED_CURRENT_50_0_MA && shouldRun; i++) {
        // Toggle the LED current
        std::cout << "Setting LED current = " << i << std::endl;

        sensor.current((MAX30100_LED_CURRENT) i, (MAX30100_LED_CURRENT) i);

        upm_delay(1);
    }

    // Read individual samples
    for (int i = 0; i < 10; i++) {
        max30100_value val = sensor.sample();
        std::cout << "Single value IR: " << val.IR << " R: " << val.R << std::endl;
    }

    //! [Interesting]

    std::cout << "Exiting..." << std::endl;

    return 0;
}
