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

#include "mcp2515.h"

int main(int argc, char **argv)
{
//! [Interesting]

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

    printf("Setting loopback mode...\n");

    // set the mode to loopback mode.  In loopback mode, transmitted
    // packets are sent directly to an appropriate receive buffer
    // without actually going out onto the CAN bus.
    if (mcp2515_set_opmode(sensor, MCP2515_OPMODE_LOOPBACK))
    {
        printf("mcp2515_set_opmode(loopback) failed.\n");
        return 1;
    }

    // lets build up a packet and try loading it.
    uint8_t myPayload[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    printf("Loading a packet of 8 numbers (0-7) into a TX buffer...\n");
    if (mcp2515_load_tx_buffer(sensor, MCP2515_TX_BUFFER0, 0, false, false,
                               myPayload, 8))
    {
        printf("mcp2515_load_tx_buffer() failed\n");
        return 1;
    }

    // now lets try to transmit it
    printf("Transmitting packet...\n");
    upm_result_t rv = 0;
    if ((rv = mcp2515_transmit_buffer(sensor, MCP2515_TX_BUFFER0, true)))
    {
        printf("mcp2515_transmit_buffer() failed with code %d\n", rv);
        return 1;
    }

    printf("Transmit successful\n");

    // There should now be a packet waiting for us in RXB0
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
    }
    else
    {
        printf("No packet found, how strange.\n");
    }

    printf("Exiting...\n");

    mcp2515_close(sensor);
//! [Interesting]

    return 0;
}
