/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>

#include "adc121c021.hpp"
#include "upm_utilities.h"

using namespace std;

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
    // Instantiate an ADC121C021 on I2C bus 0

    upm::ADC121C021 adc(ADC121C021_I2C_BUS, ADC121C021_DEFAULT_I2C_ADDR);

    // An analog sensor, such as a Grove light sensor,
    // must be attached to the adc
    // Prints the value and corresponding voltage every 50 milliseconds
    while (shouldRun) {
        uint16_t val = adc.value();
        cout << "ADC value: " << val << " Volts = " << adc.valueToVolts(val) << endl;
        upm_delay_us(50000);
    }
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
