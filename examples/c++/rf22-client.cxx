/*
 * Author: Kiveisha Yevgeniy
 * Copyright (c) 2015-2016 Intel Corporation.
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
