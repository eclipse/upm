/*
 * Author: Jun Kato <i@junkato.jp>
 * Copyright (c) 2015 Jun Kato.
 *
 * Thanks to Seeed Studio for a working arduino sketch
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <signal.h>
#include <stdio.h>

#include "ultrasonic.hpp"
#include "upm_utilities.h"

bool running = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        running = false;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);
    //! [Interesting]
    // upm::UltraSonic *sonar = NULL;
    upm::UltraSonic sonar(2);
    while (running) {
        int width = sonar.getDistance();
        printf("Echo width = %d\n", width);
        printf("Distance inches = %f.2\n\n", width / 148.0);
        upm_delay(3);
    }
    //! [Interesting]
    printf("exiting application\n");
    return 0;
}
