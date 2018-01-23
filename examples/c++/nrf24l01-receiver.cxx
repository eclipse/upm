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

#include <iostream>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf24l01.hpp"

int running = 0;

uint8_t local_address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
uint8_t broadcast_address[5] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
upm::NRF24L01 comm(7, 8);

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
class mycb : public virtual Callback
{
    public:
        mycb(upm::NRF24L01 *com) : _com(com) {}
        virtual void run()
        {
            if (_com != NULL)
                std::cout << "Reciever :: " << *((uint32_t*) &(_com->m_rxBuffer[0])) << std::endl;
            else
                std::cout << "Example callback!" << std::endl;
        }
    private:
        upm::NRF24L01* _com;
};

int
main(int argc, char** argv)
{
    mycb cb(&comm);

    comm.setSourceAddress((uint8_t*) local_address);
    comm.setDestinationAddress((uint8_t*) broadcast_address);
    comm.setPayload(MAX_BUFFER);
    comm.configure();
    comm.setSpeedRate(upm::NRF_250KBPS);
    comm.setChannel(99);
    comm.setDataReceivedHandler(&cb);

    signal(SIGINT, sig_handler);

    while (!running) {
        comm.pollListener();
    }

    std::cout << "exiting application" << std::endl;

    return 0;
}
//! [Interesting]
