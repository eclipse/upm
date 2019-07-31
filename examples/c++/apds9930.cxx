/*
 * Author: Lay, Kuan Loon <kuan.loon.lay@intel.com>
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

#include "apds9930.hpp"
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
main()
{
    signal(SIGINT, sig_handler);
    //! [Interesting]
    // Instantiate a Digital Proximity and Ambient Light sensor on iio device 4
    upm::APDS9930 light_proximity(4);

    // Kernel driver implement upm_delay 5000-5100us after enable illuminance
    // sensor
    light_proximity.enableIlluminance(true);

    // Kernel driver implement upm_delay 5000-5100us after enable proximity sensor
    light_proximity.enableProximity(true);

    // Tested this value works. Please change it on your platform
    upm_delay_us(120000);

    while (shouldRun) {
        float lux = light_proximity.getAmbient();
        cout << "Luminance value is " << lux << endl;
        float proximity = light_proximity.getProximity();
        cout << "Proximity value is " << proximity << endl;
        upm_delay(1);
    }
    light_proximity.enableProximity(false);
    light_proximity.enableIlluminance(false);
    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}
