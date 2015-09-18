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

int running = 0;
upm::NRF24L01 *comm = NULL;

uint8_t destAddress[5]     = {0x01, 0x01, 0x01, 0x01, 0x01};
uint8_t srcAddress[5]      = {0x01, 0x01, 0x01, 0x01, 0x02};

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
    uint32_t dummyData = 0;
    comm = new upm::NRF24L01(7, 8);
    comm->setSourceAddress ((uint8_t *) srcAddress);
    comm->setDestinationAddress ((uint8_t *) destAddress);
    comm->setPayload (MAX_BUFFER);
    comm->setChannel (99);
    comm->configure ();
    comm->setDataReceivedHandler (nrf_handler);

    signal(SIGINT, sig_handler);

    while (!running) {
        memcpy (comm->m_txBuffer, &dummyData, sizeof (dummyData));
        comm->send ();
        std::cout << "devi2 :: sending data ...." << dummyData << std::endl;
        usleep (3000000);
        dummyData += 3000;
    }

    std::cout << "exiting application" << std::endl;

    delete comm;
//! [Interesting]
    return 0;
}
