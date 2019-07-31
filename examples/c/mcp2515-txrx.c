/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <upm_utilities.h>

#include "mcp2515.h"

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main(int argc, char **argv)
{
    signal(SIGINT, sig_handler);

    // by default we just receive packets
    bool do_tx = false;

    // if an argument is given, we will transmit packets
    if (argc > 1)
        do_tx = true;
//! [Interesting]

    // NOTE: This example assumes that only two devices are connected
    // to the CAN bus, and that both devices are running this example;
    // one in TX (transmit) mode, and one in RX (receive) mode.

    // If this is not the case, then in rx mode you may see other
    // packets from other devices, or not, depending on whether the
    // speed is correct for the network.  In tx mode, errors may be
    // generated on the bus, especially if the CAN bus speed does not
    // match the rest of the network.

    // You should start the receiver example first.  The transmitter
    // example will fail after about 5 seconds (timeout) of not being
    // able to transmit a message.

    // Instantiate a MCP2515 on SPI bus 0 using a hw CS pin (-1).

#if defined(CONFIG_BOARD_ARDUINO_101)
    // ARDUINO_101 (Quark core) where you must use SPI
    // Instantiate a MCP2515 instance using default SPI bus and pin 10 as CS
    mcp2515_context sensor = mcp2515_init(0, 10);
#else
    // Instantiate a MCP2515 on SPI bus 0 using a hw CS pin (-1).
    mcp2515_context sensor = mcp2515_init(0, -1);
#endif

    if (!sensor)
    {
        printf("mcp2515_init() failed\n");
        return 1;
    }

    // By default, after initialization, the baud rate is set to
    // 50Kbps, and the mode is NORMAL, so we don't need to set any of
    // that here.

    if (!do_tx)
        printf("RX mode, waiting for packets...\n");

    // our TX payload.  If transmitting, the first number will be
    // incremented (and rollover) on each transmission.
    uint8_t myPayload[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    while (shouldRun)
    {
        if (do_tx)
        {
            // lets build up a packet and try loading it.
            printf("Loading a packet of 8 numbers into a tx buffer...\n");
            if (mcp2515_load_tx_buffer(sensor, MCP2515_TX_BUFFER0, 0,
                                       false, false, myPayload, 8))
            {
                printf("mcp2515_load_tx_buffer() failed\n");
                return 1;
            }

            // now lets try to transmit it
            printf("Transmitting packet...\n");
            upm_result_t rv = 0;
            if ((rv = mcp2515_transmit_buffer(sensor, MCP2515_TX_BUFFER0,
                                              true)))
            {
                printf("mcp2515_transmit_buffer() failed with code %d\n", rv);
                return 1;
            }

            // increment the first number for each transmission
            myPayload[0]++;
            printf("Transmit successful\n\n");
            upm_delay_ms(500);
        }
        else
        {
            // RX mode
            // Look for a packet waiting for us in RXB0
            if (mcp2515_rx_status_msgs(sensor) == MCP2515_RXMSG_RXB0)
            {
                printf("Packet received in RXB0, decoding...\n");

                // now lets retrieve it
                MCP2515_MSG_T msg;
                if (mcp2515_get_rx_msg(sensor, MCP2515_RX_BUFFER0, &msg))
                {
                    printf("mcp2515_get_rx_msg() failed\n");
                    return 1;
                }
                mcp2515_print_msg(sensor, &msg);
                printf("\n");
            }

            upm_delay_ms(100);
        }

    }

    printf("Exiting...\n");

    mcp2515_close(sensor);
//! [Interesting]

    return 0;
}
