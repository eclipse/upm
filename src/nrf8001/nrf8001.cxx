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

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include "nrf8001.h"

/**
 * Include the services_lock.h to put the setup in the OTP memory of the nRF8001.
 * This would mean that the setup cannot be changed once put in.
 * However this removes the need to do the setup of the nRF8001 on every reset.
*/

void
init_local_interfaces (aci_state_t* aci, uint8_t reqn, uint8_t rdyn, uint8_t rst) {
    /**
     * Tell the ACI library, the MCU to nRF8001 pin connections.
     * The Active pin is optional and can be marked UNUSED
     */
    aci->aci_pins.board_name                = BOARD_DEFAULT;    // See board.h for details REDBEARLAB_SHIELD_V1_1 or BOARD_DEFAULT
    aci->aci_pins.reqn_pin                  = reqn;             // SS for Nordic board, 9 for REDBEARLAB_SHIELD_V1_1
    aci->aci_pins.rdyn_pin                  = rdyn;             // 3 for Nordic board, 8 for REDBEARLAB_SHIELD_V1_1

    aci->aci_pins.reset_pin                 = rst;              // 4 for Nordic board, UNUSED for REDBEARLAB_SHIELD_V1_1
    aci->aci_pins.active_pin                = UNUSED;
    aci->aci_pins.optional_chip_sel_pin     = UNUSED;

    aci->aci_pins.interface_is_interrupt    = false;            // Interrupts still not available in Chipkit
    aci->aci_pins.interrupt_number          = 1;

    lib_aci_init (aci, false);
}

void
close_local_interfaces (aci_state_t* aci) {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_spi_stop(aci->aci_pins.m_spi);
    if (error != MRAA_SUCCESS) {

    }
    error = mraa_gpio_close (aci->aci_pins.m_rdy_ctx);
    if (error != MRAA_SUCCESS) {

    }
    error = mraa_gpio_close (aci->aci_pins.m_req_ctx);
    if (error != MRAA_SUCCESS) {

    }
    error = mraa_gpio_close (aci->aci_pins.m_rst_ctx);
    if (error != MRAA_SUCCESS) {

    }
}
