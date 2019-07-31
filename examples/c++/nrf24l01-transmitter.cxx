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
#include <string.h>

#include "nrf24l01.hpp"
#include "upm_utilities.h"

int running = 0;

uint8_t destAddress[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
uint8_t srcAddress[5] = { 0x01, 0x01, 0x01, 0x01, 0x02 };

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
        running = 1;
    }
}

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
    //! [Interesting]
    uint32_t dummyData = 0;
    upm::NRF24L01 comm(7, 8);
    mycb cb(&comm);

    comm.setSourceAddress((uint8_t*) srcAddress);
    comm.setDestinationAddress((uint8_t*) destAddress);
    comm.setPayload(MAX_BUFFER);
    comm.setChannel(99);
    comm.configure();
    comm.setDataReceivedHandler(&cb);

    signal(SIGINT, sig_handler);

    while (!running) {
        memcpy(comm.m_txBuffer, &dummyData, sizeof(dummyData));
        comm.send();
        std::cout << "devi2 :: sending data ...." << dummyData << std::endl;
        upm_delay_us(3000000);
        dummyData += 3000;
    }

    std::cout << "exiting application" << std::endl;

    //! [Interesting]
    return 0;
}
