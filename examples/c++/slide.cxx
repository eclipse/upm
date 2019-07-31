/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <stdint.h>
#include <stdio.h>

#include "slide.hpp"
#include "upm_utilities.h"

using namespace std;

int
main()
{
    //! [Interesting]
    upm::Slide slide(0);

    cout << slide.name() << endl;

    while (true) {
        float adc_value = slide.raw_value(); // Read raw value
        float volts = slide.voltage_value(); // Read voltage, board reference set at 5.0V
        fprintf(stdout, "%4d = %.2f V\n", (uint16_t) adc_value, volts);

        upm_delay_us(2500000); // Sleep for 2.5s
    }
    //! [Interesting]
    return 0;
}
