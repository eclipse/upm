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

#include "collision.hpp"

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
    // The was tested with the Collision Sensor
    // Instantiate a Collision on digital pin D2
    upm::Collision collision(2);

    bool collisionState = false;
    cout << "No collision" << endl;
    while (shouldRun) {
        if (collision.isColliding() && !collisionState) {
            cout << "Collision!" << endl;
            collisionState = true;
        } else if (collisionState) {
            cout << "No collision" << endl;
            collisionState = false;
        }
    }

    //! [Interesting]
    cout << "Exiting" << endl;

    return 0;
}
