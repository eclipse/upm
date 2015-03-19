/* Copyright (c) 2014, Nordic Semiconductor ASA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/** 
 * @file
 * @brief Interface for hal_aci_tl.
 * @ingroup nrf8001
 */

/**
@{

@brief Module for the ACI Transport Layer interface
@details This module is responsible for sending and receiving messages over the ACI interface of the nRF8001 chip.
 The hal_aci_tl_send_cmd() can be called directly to send ACI commands.


The RDYN line is hooked to an interrupt on the MCU when the level is low.
The SPI master clocks in the interrupt context.
The ACI Command is taken from the head of the command queue is sent over the SPI
and the received ACI event is placed in the tail of the event queue.

*/

#ifndef HAL_ACI_TL_H__
#define HAL_ACI_TL_H__

#include "hal_platform.h"
#include "aci.h"
#include "boards.h"

#include <mraa/aio.h>
#include <mraa/gpio.h>
#include <mraa/spi.h>

#ifndef HAL_ACI_MAX_LENGTH
#define HAL_ACI_MAX_LENGTH 31
#endif

/************************************************************************/
/* Unused nRF8001 pin                                                    */
/************************************************************************/
#define UNUSED          255

/** Data type for ACI commands and events */
typedef struct {
  uint8_t status_byte;
  uint8_t buffer[HAL_ACI_MAX_LENGTH+1];
} _aci_packed_ hal_aci_data_t;

ACI_ASSERT_SIZE(hal_aci_data_t, HAL_ACI_MAX_LENGTH + 2);

/** Datatype for ACI pins and interface (polling/interrupt)*/
typedef struct aci_pins_t
{
    mraa_spi_context        m_spi;
    mraa_gpio_context       m_rdy_ctx;
    mraa_gpio_context       m_req_ctx;
    mraa_gpio_context       m_rst_ctx;

    uint8_t board_name;             //Optional : Use BOARD_DEFAULT if you do not know. See boards.h
    uint8_t reqn_pin;               //Required
    uint8_t rdyn_pin;               //Required
    uint8_t mosi_pin;               //Required
    uint8_t miso_pin;               //Required
    uint8_t sck_pin;                //Required

    uint8_t spi_clock_divider;      //Required : Clock divider on the SPI clock : nRF8001 supports a maximum clock of 3MHz

    uint8_t reset_pin;              //Recommended but optional - Set it to UNUSED when not connected
    uint8_t active_pin;             //Optional - Set it to UNUSED when not connected
    uint8_t optional_chip_sel_pin;  //Optional - Used only when the reqn line is required to be separate from the SPI chip select. Eg. Arduino DUE

    bool    interface_is_interrupt; //Required - true = Uses interrupt on RDYN pin. false - Uses polling on RDYN pin

    uint8_t interrupt_number;       //Required when using interrupts, otherwise ignored
} aci_pins_t;

/** @brief ACI Transport Layer initialization.
 *  @details
 *  This function initializes the transport layer, including configuring the SPI, creating
 *  message queues for Commands and Events and setting up interrupt if required.
 *  @param a_pins Pins on the MCU used to connect to the nRF8001
 *  @param bool True if debug printing should be enabled on the Serial.
 */
void hal_aci_tl_init(aci_pins_t *a_pins, bool debug);

/** @brief Sends an ACI command to the radio.
 *  @details
 *  This function sends an ACI command to the radio. This queue up the message to send and
 *  lower the request line. When the device lowers the ready line, @ref m_aci_spi_transfer()
 *  will send the data.
 *  @param aci_buffer Pointer to the message to send.
 *  @return True if the data was successfully queued for sending,
 *  false if there is no more space to store messages to send.
 */
bool hal_aci_tl_send(hal_aci_data_t *aci_buffer);

/** @brief Process pending transactions.
 *  @details
 *  The library code takes care of calling this function to check if the nRF8001 RDYN line indicates a
 *  pending transaction. It will send a pending message if there is one and return any receive message
 *  that was pending.
 *  @return Points to data buffer for received data. Length byte in buffer is 0 if no data received.
 */
hal_aci_data_t * hal_aci_tl_poll_get(void);

/** @brief Get an ACI event from the event queue
 *  @details
 *  Call this function from the main context to get an event from the ACI event queue
 *  This is called by lib_aci_event_get
 */
bool hal_aci_tl_event_get(hal_aci_data_t *p_aci_data);

/** @brief Peek an ACI event from the event queue
 *  @details
 *  Call this function from the main context to peek an event from the ACI event queue.
 *  This is called by lib_aci_event_peek
 */
bool hal_aci_tl_event_peek(hal_aci_data_t *p_aci_data);

/** @brief Enable debug printing of all ACI commands sent and ACI events received
 *  @details
 *  when the enable parameter is true. The debug printing is enabled on the Serial.
 *  When the enable parameter is false. The debug printing is disabled on the Serial.
 *  By default the debug printing is disabled.
 */
void hal_aci_tl_debug_print(bool enable);


/** @brief Pin reset the nRF8001
 *  @details
 *  The reset line of the nF8001 needs to kept low for 200 ns.
 *  Redbearlab shield v1.1 and v2012.07 are exceptions as they
 *  have a Power ON Reset circuit that works differently.
 *  The function handles the exceptions based on the board_name in aci_pins_t
 */
void hal_aci_tl_pin_reset(void);

/** @brief Return full status of transmit queue
 *  @details
 *
 */
 bool hal_aci_tl_rx_q_full(void);

 /** @brief Return empty status of receive queue
 *  @details
 *
 */
 bool hal_aci_tl_rx_q_empty(void);

/** @brief Return full status of receive queue
 *  @details
 *
 */
 bool hal_aci_tl_tx_q_full(void);

 /** @brief Return empty status of transmit queue
 *  @details
 *
 */
 bool hal_aci_tl_tx_q_empty(void);

/** @brief Flush the ACI command Queue and the ACI Event Queue
 *  @details
 *  Call this function in the main thread
 */
void hal_aci_tl_q_flush(void);

#endif // HAL_ACI_TL_H__
/** @} */
