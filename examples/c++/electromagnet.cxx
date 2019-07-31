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
#include <sys/time.h>
#include <time.h>

#include "electromagnet.hpp"

using namespace std;

int shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

float
get_time()
{
    return ((float) (clock())) / CLOCKS_PER_SEC;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // The was tested with the  Electromagnetic Module
    // Instantiate a  Electromagnet on digital pin D2
    upm::Electromagnet magnet(2);
    cout << "Starting up magnet...." << endl;
    magnet.off();

    bool magnetState = false;
    float time_passed = get_time();

    // Turn magnet on and off every 5 seconds
    while (shouldRun) {
        if ((get_time() - time_passed) > 5.0) {
            magnetState = !magnetState;
            if (magnetState)
                magnet.on();
            else
                magnet.off();
            cout << "Turning magnet " << ((magnetState) ? "on" : "off") << endl;
            time_passed = get_time();
        }
    }

    //! [Interesting]
    magnet.off();
    cout << "Exiting" << endl;

    return 0;
}
