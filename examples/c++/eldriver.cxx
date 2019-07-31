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

#include "eldriver.hpp"
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
    // This was tested with the El Driver Module
    // Instantiate an El Driver on digital pin D2
    upm::ElDriver eldriver(2);

    bool lightState = true;

    while (shouldRun) {
        if (lightState)
            eldriver.on();
        else
            eldriver.off();
        lightState = !lightState;
        upm_delay(1);
    }

    //! [Interesting]
    eldriver.off();
    cout << "Exiting" << endl;

    return 0;
}
