/*  Copyright (c) 2013, Nordic Semiconductor ASA
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *  Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 *  Redistributions in binary form must reproduce the above copyright notice, this
 *  list of conditions and the following disclaimer in the documentation and/or
 *  other materials provided with the distribution.
 *
 *  Neither the name of Nordic Semiconductor ASA nor the names of its
 *  contributors may be used to endorse or promote products derived from
 *  this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * @brief BLE specific macros
 * @ingroup nrf8001
 */

#ifndef UART_OVER_BLE_H__
#define UART_OVER_BLE_H__

/**
 * @def UART_OVER_BLE_DISCONNECT
 * @brief
 * Command to queue a ACI Disconnect to the nRF8001
 */
#define UART_OVER_BLE_DISCONNECT      (0x01)


/**
 * @def UART_OVER_BLE_LINK_TIMING_REQ
 * @brief
 * Command to queue a ACI Change Timing to the nRF8001
 */
#define UART_OVER_BLE_LINK_TIMING_REQ (0x02)

/**
 * @def UART_OVER_BLE_TRANSMIT_STOP
 * @brief
 * Command to stop sending UART over BLE packets
 */
#define UART_OVER_BLE_TRANSMIT_STOP   (0x03)


/**
 * @def UART_OVER_BLE_TRANSMIT_OK
 * @brief
 * Command to allow sending UART over BLE packets
 */
#define UART_OVER_BLE_TRANSMIT_OK     (0x04)

/**
 * @struct uart_over_ble_t
 * @brief State of the UART RTS over Bluetooth Low Energy(BLE)
 */
typedef struct
{
    uint8_t uart_rts_local;  /**< State of the local UART RTS  */
    uint8_t uart_rts_remote; /**< State of the remote UART RTS */
} uart_over_ble_t;

#endif // UART_OVER_BLE_H__
