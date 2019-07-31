/*
 * Author: Kiveisha Yevgeniy
 * Copyright (c) 2015-2016 Intel Corporation.
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

#include "rf22.hpp"

bool amWorking = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT) {
        amWorking = false;
    }
}
//! [Interesting]
int
main(int argc, char** argv)
{
    // SPI bus 0, CS pin 10, INTR pin 2
    upm::RF22 rf22(0, 10, 2);

    if (!rf22.init()) {
        std::cout << "RF22 init failed" << std::endl;
        return 0x1;
    }

    uint8_t data[] = "Hello World!";
    uint8_t buf[RF22_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    signal(SIGINT, sig_handler);
    while (amWorking) {
        std::cout << "Sending to rf22_server" << std::endl;
        // Send a message to rf22_server
        rf22.send(data, sizeof(data));
        rf22.waitPacketSent();
        // Now wait for a reply

        rf22.waitAvailableTimeout(1000000);

        if (rf22.recv(buf, &len)) {
            std::cout << "got response: " << (char*) buf << std::endl;
        } else {
            std::cout << "!!! NO RESPONSE !!!" << std::endl;
        }
    }

    std::cout << "Exit 'rfm22-client'" << std::endl;
    return 0;
}
//! [Interesting]
