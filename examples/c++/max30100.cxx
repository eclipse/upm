/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
