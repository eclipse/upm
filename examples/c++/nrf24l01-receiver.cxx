/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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
