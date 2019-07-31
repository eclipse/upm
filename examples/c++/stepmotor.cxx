/*
 * Authors: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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
#include <stdio.h>

#include "stepmotor.hpp"
#include "upm_utilities.h"

using namespace std;

int doWork = 1;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
        doWork = 0;
    }
}

int
main(int argc, char** argv)
{
    //! [Interesting]
    upm::StepMotor sensor(2, 3);

    while (doWork) {
        cout << "1 Revolution forward and back at 60 rpm" << endl;
        sensor.setSpeed(60);
        sensor.stepForward(200);
        upm_delay_us(1000000);
        sensor.stepBackward(200);
        upm_delay_us(1000000);

        cout << "1 Revolution forward and back at 150 rpm" << endl;
        sensor.setSpeed(150);
        sensor.stepForward(200);
        upm_delay_us(1000000);
        sensor.stepBackward(200);
        upm_delay_us(1000000);

        cout << "1 Revolution forward and back at 300 rpm" << endl;
        sensor.setSpeed(300);
        sensor.stepForward(200);
        upm_delay_us(1000000);
        sensor.stepBackward(200);
        upm_delay_us(1000000);
    }

    //! [Interesting]
    return 0;
}
