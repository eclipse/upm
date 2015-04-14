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
#include <iostream>
#include "nrf8001.h"
#include "nrf8001-helloworld.h"
#include <lib_aci.h>
#include <aci_setup.h>
#include <signal.h>
#include "uart_over_ble.h"

/*
 * You can use the nRF UART app in the Apple iOS app store and Google Play for Android 4.3 for Samsung Galaxy S4
 * with this helloworld application
 */

#ifdef SERVICES_PIPE_TYPE_MAPPING_CONTENT
    static services_pipe_type_mapping_t
        services_pipe_type_mapping[NUMBER_OF_PIPES] = SERVICES_PIPE_TYPE_MAPPING_CONTENT;
#else
    #define NUMBER_OF_PIPES 0
    static services_pipe_type_mapping_t * services_pipe_type_mapping = NULL;
#endif

/**
 * Store the setup for the nRF8001 in the flash of the AVR to save on RAM
 */
static hal_aci_data_t setup_msgs[NB_SETUP_MESSAGES] = SETUP_MESSAGES_CONTENT;

/**
 * aci_struct that will contain
 * total initial credits
 * current credit
 * current state of the aci (setup/standby/active/sleep)
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
static hal_aci_evt_t  aci_data;

/*
Timing change state variable
*/
static bool timing_change_done          = false;

/*
Used to test the UART TX characteristic notification
*/
static uart_over_ble_t uart_over_ble;
static uint8_t         uart_buffer[20];
static uint8_t         uart_buffer_len = 0;
static uint8_t         dummychar = 0;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
    }
}

void
init_aci_setup () {
    /**
     * Point ACI data structures to the the setup data that the nRFgo studio generated for the nRF8001
     */
    if (NULL != services_pipe_type_mapping) {
        aci_state.aci_setup_info.services_pipe_type_mapping = &services_pipe_type_mapping[0];
    } else {
        aci_state.aci_setup_info.services_pipe_type_mapping = NULL;
    }

    aci_state.aci_setup_info.number_of_pipes    = NUMBER_OF_PIPES;
    aci_state.aci_setup_info.setup_msgs         = setup_msgs;
    aci_state.aci_setup_info.num_setup_msgs     = NB_SETUP_MESSAGES;
}

void
uart_over_ble_init (void) {
    uart_over_ble.uart_rts_local = true;
}

bool
uart_tx (uint8_t *buffer, uint8_t buffer_len) {
    bool status = false;

    if  (lib_aci_is_pipe_available(&aci_state, PIPE_UART_OVER_BTLE_UART_TX_TX) &&
        (aci_state.data_credit_available >= 1)) {
        status = lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, buffer, buffer_len);
        if (status) {
            aci_state.data_credit_available--;
        }
    }

    return status;
}

bool
uart_process_control_point_rx(uint8_t *byte, uint8_t length) {
    bool status = false;
    aci_ll_conn_params_t *conn_params;

    if (lib_aci_is_pipe_available(&aci_state, PIPE_UART_OVER_BTLE_UART_CONTROL_POINT_TX) ) {
        switch (*byte) {
            /*
            Queues a ACI Disconnect to the nRF8001 when this packet is received.
            May cause some of the UART packets being sent to be dropped
            */
            case UART_OVER_BLE_DISCONNECT:
                /*
                Parameters:
                None
                */
                lib_aci_disconnect(&aci_state, ACI_REASON_TERMINATE);
                status = true;
                break;

            /*
            Queues an ACI Change Timing to the nRF8001
            */
            case UART_OVER_BLE_LINK_TIMING_REQ:
                /*
                Parameters:
                Connection interval min: 2 bytes
                Connection interval max: 2 bytes
                Slave latency:           2 bytes
                Timeout:                 2 bytes
                Same format as Peripheral Preferred Connection Parameters (See nRFgo studio -> nRF8001 Configuration -> GAP Settings
                Refer to the ACI Change Timing Request in the nRF8001 Product Specifications
                */
                conn_params = (aci_ll_conn_params_t *)(byte+1);
                lib_aci_change_timing( conn_params->min_conn_interval,
                                        conn_params->max_conn_interval,
                                        conn_params->slave_latency,
                                        conn_params->timeout_mult);
                status = true;
                break;

            /*
            Clears the RTS of the UART over BLE
            */
            case UART_OVER_BLE_TRANSMIT_STOP:
                /*
                Parameters:
                None
                */
                uart_over_ble.uart_rts_local = false;
                status = true;
                break;


            /*
            Set the RTS of the UART over BLE
            */
            case UART_OVER_BLE_TRANSMIT_OK:
                /*
                Parameters:
                None
                */
                uart_over_ble.uart_rts_local = true;
                status = true;
                break;
        }
    }

    return status;
}

int
main(int argc, char **argv)
{
    //! [Interesting]

    init_aci_setup ();
    init_local_interfaces (&aci_state, 10, 8, 4);

    while (1) {
        static bool setup_required = false;

        // We enter the if statement only when there is a ACI event available to be processed
        if (lib_aci_event_get(&aci_state, &aci_data)) {
            aci_evt_t * aci_evt;
            aci_evt = &aci_data.evt;
            switch(aci_evt->evt_opcode) {
                /**
                As soon as you reset the nRF8001 you will get an ACI Device Started Event
                */
                case ACI_EVT_DEVICE_STARTED: {
                    aci_state.data_credit_total = aci_evt->params.device_started.credit_available;
                    switch(aci_evt->params.device_started.device_mode) {
                        case ACI_DEVICE_SETUP:
                            /**
                            When the device is in the setup mode
                            */
                            printf ("Evt Device Started: Setup \n");
                            setup_required = true;
                            break;

                        case ACI_DEVICE_STANDBY:
                            printf ("Evt Device Started: Standby \n");
                            // Looking for an iPhone by sending radio advertisements
                            // When an iPhone connects to us we will get an ACI_EVT_CONNECTED event from the nRF8001
                            if (aci_evt->params.device_started.hw_error) {
                                usleep (20000); //Handle the HW error event correctly.
                            } else {
                                lib_aci_connect(0/* in seconds : 0 means forever */, 0x0050 /* advertising interval 50ms*/);
                                printf ("Advertising started \n");
                            }
                            break;
                    }
                }
                break; // ACI Device Started Event

                case ACI_EVT_CMD_RSP:
                    //If an ACI command response event comes with an error -> stop
                    if (ACI_STATUS_SUCCESS != aci_evt->params.cmd_rsp.cmd_status) {
                        //ACI ReadDynamicData and ACI WriteDynamicData will have status codes of
                        //TRANSACTION_CONTINUE and TRANSACTION_COMPLETE
                        //all other ACI commands will have status code of ACI_STATUS_SCUCCESS for a successful command
                        printf ("ACI_EVT_CMD_RSP \n");
                    }
                    if (ACI_CMD_GET_DEVICE_VERSION == aci_evt->params.cmd_rsp.cmd_opcode) {
                        //Store the version and configuration information of the nRF8001 in the Hardware Revision String Characteristic
                        lib_aci_set_local_data(&aci_state, PIPE_DEVICE_INFORMATION_HARDWARE_REVISION_STRING_SET,
                        (uint8_t *)&(aci_evt->params.cmd_rsp.params.get_device_version), sizeof(aci_evt_cmd_rsp_params_get_device_version_t));
                    }
                    break;

                case ACI_EVT_CONNECTED:
                    printf ("ACI_EVT_CONNECTED");
                    uart_over_ble_init ();
                    timing_change_done = false;
                    aci_state.data_credit_available = aci_state.data_credit_total;

                    /*
                    Get the device version of the nRF8001 and store it in the Hardware Revision String
                    */
                    lib_aci_device_version();
                    break;

                case ACI_EVT_PIPE_STATUS:
                    printf ("ACI_EVT_PIPE_STATUS \n");
                    if (lib_aci_is_pipe_available(&aci_state, PIPE_UART_OVER_BTLE_UART_TX_TX) && (false == timing_change_done)) {
                        lib_aci_change_timing_GAP_PPCP();   // change the timing on the link as specified in the nRFgo studio -> nRF8001 conf. -> GAP.
                                                            // Used to increase or decrease bandwidth
                        timing_change_done = true;

                        char hello[]="Hello World, works";
                        uart_tx((uint8_t *)&hello[0], strlen(hello));
                    }
                    break;

                case ACI_EVT_TIMING:
                    printf ("Evt link connection interval changed \n");
                    lib_aci_set_local_data(&aci_state,
                                PIPE_UART_OVER_BTLE_UART_LINK_TIMING_CURRENT_SET,
                                (uint8_t *)&(aci_evt->params.timing.conn_rf_interval), /* Byte aligned */
                                PIPE_UART_OVER_BTLE_UART_LINK_TIMING_CURRENT_SET_MAX_SIZE);
                    break;

                case ACI_EVT_DISCONNECTED:
                    printf ("ACI_EVT_DISCONNECTED \n");
                    lib_aci_connect(0/* in seconds  : 0 means forever */, 0x0050 /* advertising interval 50ms*/);
                    printf ("Advertising started \n");
                    break;

                case ACI_EVT_DATA_RECEIVED:
                    if (PIPE_UART_OVER_BTLE_UART_RX_RX == aci_evt->params.data_received.rx_data.pipe_number) {
                        for(int i=0; i<aci_evt->len - 2; i++) {
                          uart_buffer[i] = aci_evt->params.data_received.rx_data.aci_data[i];
                        }

                        uart_buffer_len = aci_evt->len - 2;
                        if (lib_aci_is_pipe_available(&aci_state, PIPE_UART_OVER_BTLE_UART_TX_TX)) {
                        }
                    }

                    if (PIPE_UART_OVER_BTLE_UART_CONTROL_POINT_RX == aci_evt->params.data_received.rx_data.pipe_number) {
                        //Subtract for Opcode and Pipe number
                        uart_process_control_point_rx(&aci_evt->params.data_received.rx_data.aci_data[0], aci_evt->len - 2);
                    }

                    printf ("Incomming data - %s\n", uart_buffer);
                    break;

                case ACI_EVT_DATA_CREDIT:
                    printf ("ACI_EVT_DATA_CREDIT \n");
                    aci_state.data_credit_available = aci_state.data_credit_available + aci_evt->params.data_credit.credit;
                    break;

                case ACI_EVT_PIPE_ERROR:
                    printf ("ACI_EVT_PIPE_ERROR \n");
                    //Increment the credit available as the data packet was not sent.
                    //The pipe error also represents the Attribute protocol Error Response sent from the peer and that should not be counted
                    //for the credit.
                    if (ACI_STATUS_ERROR_PEER_ATT_ERROR != aci_evt->params.pipe_error.error_code) {
                        aci_state.data_credit_available++;
                    }
                    break;

                case ACI_EVT_HW_ERROR:
                    printf ("ACI_EVT_HW_ERROR \n");
                    lib_aci_connect(0/* in seconds, 0 means forever */, 0x0050 /* advertising interval 50ms*/);
                    printf ("Advertising started \n");
                break;

            }
        }

        /* setup_required is set to true when the device starts up and enters setup mode.
        * It indicates that do_aci_setup() should be called. The flag should be cleared if
        * do_aci_setup() returns ACI_STATUS_TRANSACTION_COMPLETE.
        */
        if(setup_required) {
            if (SETUP_SUCCESS == do_aci_setup(&aci_state)) {
                setup_required = false;
            }
        }

        usleep (100);
    }

    close_local_interfaces (&aci_state);

    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}
