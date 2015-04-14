/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include "ecs1030.h"

int is_running = 0;
upm::ECS1030 *sensor = NULL;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        is_running = 1;
    }
}

//! [Interesting]
int
main(int argc, char **argv)
{
    sensor = new upm::ECS1030(0);
    signal(SIGINT, sig_handler);

    while (!is_running) {
        std::cout << "I = " << sensor->getCurrency_A () << ", Power = " << sensor->getPower_A () << std::endl;
        std::cout << "I = " << sensor->getCurrency_B () << ", Power = " << sensor->getPower_B () << std::endl;
    }

    std::cout << "exiting application" << std::endl;

    delete sensor;

    return 0;
}
//! [Interesting]
