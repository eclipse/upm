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

#include <string.h>
#include <unistd.h>
#include <iostream>
#include "nrf24l01.h"
#include <signal.h>
#include <stdio.h>
#include <vector>

int running = 0;
upm::NRF24L01 *sensor = NULL;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
        running = 1;
    }
}

void nrf_handler () {
}

int
main(int argc, char **argv)
{
    //! [Interesting]

    sensor = new upm::NRF24L01(7, 8);
    sensor->setBeaconingMode ();

    std::vector<std::string> msgs;

    msgs.push_back ("Hello World 1!!!");
    msgs.push_back ("Hello World 2!!!");
    msgs.push_back ("Hello World 3!!!");
    msgs.push_back ("Hello World 4!!!");
    msgs.push_back ("Hello World 5!!!");

    signal(SIGINT, sig_handler);

    while (!running) {
        for (std::vector<std::string>::iterator item = msgs.begin(); item != msgs.end(); ++item) {
            std::cout << "BROADCASTING " << (*item).c_str() << std::endl;

            for (int i = 0; i < 3; i++) {
                sensor->sendBeaconingMsg ((uint8_t*) (*item).c_str());
                usleep (1000000);
            }
        }
    }

    std::cout << "exiting application" << std::endl;

    msgs.clear();
    delete sensor;

    //! [Interesting]

    return 0;
}
