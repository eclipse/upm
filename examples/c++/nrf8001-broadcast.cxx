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

#include <aci_setup.h>
#include <assert.h>
#include <iostream>
#include <lib_aci.h>
#include <signal.h>
#include <stdio.h>

#include "aci_evts.h"
#include "hal_aci_tl.h"
#include "nrf8001-broadcast.hpp"
#include "nrf8001.hpp"
#include "upm_utilities.h"

#ifdef SERVICES_PIPE_TYPE_MAPPING_CONTENT
static services_pipe_type_mapping_t services_pipe_type_mapping[NUMBER_OF_PIPES] =
SERVICES_PIPE_TYPE_MAPPING_CONTENT;
#else
#define NUMBER_OF_PIPES 0
static services_pipe_type_mapping_t* services_pipe_type_mapping = NULL;
#endif

/**
 * Store the setup for the nRF8001 in the flash of the AVR to save on RAM
 */
static hal_aci_data_t setup_msgs[NB_SETUP_MESSAGES] = SETUP_MESSAGES_CONTENT;

/**
 * aci_struct that will contain
 * total initial credits
 * current credit
 * current state of the aci (setup/standby/active/upm_delay)
 * open remote pipe pending
 * close remote pipe pending
 * Current pipe available bitmap
 * Current pipe closed bitmap
 * Current connection interval, slave latency and link supervision timeout
 * Current State of the the GATT client (Service Discovery)
 * Status of the bond (R) Peer address
 */
static struct aci_state_t aci_state;

/**
 * Temporary buffers for sending ACI commands
 */
static hal_aci_evt_t aci_data;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
    }
}

void
init_aci_setup()
{
    /**
   * Point ACI data structures to the the setup data that the nRFgo studio
   * generated for the nRF8001
   */
    // abort if this is NULL
    assert(services_pipe_type_mapping != NULL);

    aci_state.aci_setup_info.services_pipe_type_mapping = &services_pipe_type_mapping[0];

    aci_state.aci_setup_info.number_of_pipes = NUMBER_OF_PIPES;
    aci_state.aci_setup_info.setup_msgs = setup_msgs;
    aci_state.aci_setup_info.num_setup_msgs = NB_SETUP_MESSAGES;
}

int
main(int argc, char** argv)
{
    //! [Interesting]

    init_aci_setup();
    init_local_interfaces(&aci_state, 10, 8, 4);

    while (1) {
        static bool setup_required = false;
        if (lib_aci_event_get(&aci_state, &aci_data)) {
            aci_evt_t* aci_evt;
            aci_evt = &aci_data.evt;

            switch (aci_evt->evt_opcode) {
                /**
          As soon as you reset the nRF8001 you will get an ACI Device Started Event
          */
                case ACI_EVT_DEVICE_STARTED: {
                    aci_state.data_credit_available =
                    aci_evt->params.device_started.credit_available;
                    switch (aci_evt->params.device_started.device_mode) {
                        case ACI_DEVICE_SETUP:
                            /**
                  When the device is in the setup mode
                  */
                            printf("Evt Device Started: Setup\n");
                            setup_required = true;
                            break;

                        case ACI_DEVICE_STANDBY:
                            printf("Evt Device Started: Standby\n");
                            lib_aci_broadcast(10 /* in seconds */,
                                              0x0100 /* advertising interval 100ms */);
                            printf("Broadcasting started\n");
                            break;
                        default:
                            break;
                    }
                } break; // ACI Device Started Event

                case ACI_EVT_CMD_RSP:
                    if (ACI_STATUS_SUCCESS != aci_evt->params.cmd_rsp.cmd_status) {
                        printf("ACI_EVT_CMD_RSP\n");
                        while (1)
                            ;
                    }
                    break;

                case ACI_EVT_CONNECTED:
                    printf("ACI_EVT_CONNECTED\n");
                    break;

                case ACI_EVT_PIPE_STATUS:
                    printf("ACI_EVT_PIPE_STATUS\n");
                    break;

                case ACI_EVT_DISCONNECTED:
                    if (ACI_STATUS_ERROR_ADVT_TIMEOUT == aci_evt->params.disconnected.aci_status) {
                        printf("Broadcasting timed out\n");
                    } else {
                        printf("Evt Disconnected. Link Loss\n");
                    }
                    break;

                case ACI_EVT_DATA_RECEIVED:
                    printf("ACI_EVT_DATA_RECEIVED\n");
                    break;

                case ACI_EVT_HW_ERROR:
                    printf("ACI_EVT_HW_ERROR\n");
                    break;
                default:
                    break;
            }
        }

        if (setup_required) {
            if (SETUP_SUCCESS == do_aci_setup(&aci_state)) {
                setup_required = false;
            }
        }
        upm_delay_us(100);
    }

    close_local_interfaces(&aci_state);

    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}
