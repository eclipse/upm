/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2015 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/
#include <iostream>
#include <signal.h>

#include "upm_utilities.h"
#include "waterlevel.hpp"

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
    // The was tested with the Water Level Sensor
    // Instantiate a Water Level Sensor on digital pin D2
    upm::WaterLevel waterlevel(2);

    while (shouldRun) {
        if (waterlevel.isSubmerged())
            cout << "Sensor is submerged in liquid" << endl;
        else
            cout << "Liquid is below water level sensor" << endl;
        upm_delay(1);
    }

    //! [Interesting]
    cout << "Exiting" << endl;

    return 0;
}
