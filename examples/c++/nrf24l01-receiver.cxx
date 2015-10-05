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

uint8_t local_address[5]     = {0x01, 0x01, 0x01, 0x01, 0x01};
uint8_t broadcast_address[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
        running = 1;
    }
}

//! [Interesting]
void nrf_handler () {
    std::cout << "Reciever :: " << *((uint32_t *)&(comm->m_rxBuffer[0])) << std::endl;
}

int
main(int argc, char **argv)
{

    comm = new upm::NRF24L01(7, 8);
    comm->setSourceAddress ((uint8_t *) local_address);
    comm->setDestinationAddress ((uint8_t *) broadcast_address);
    comm->setPayload (MAX_BUFFER);
    comm->configure ();
    comm->setSpeedRate (upm::NRF_250KBPS);
    comm->setChannel (99);
    comm->setDataReceivedHandler (nrf_handler);

    signal(SIGINT, sig_handler);

    while (!running) {
        comm->pollListener ();
    }

    std::cout << "exiting application" << std::endl;

    delete comm;
    return 0;
}
//! [Interesting]
