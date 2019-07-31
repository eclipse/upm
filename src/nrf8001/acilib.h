/* Copyright (c) 2014, Nordic Semiconductor ASA
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

/**
 * @file
 *
 * @ingroup aci-lib
 *
 * @brief Internal prototype for acilib module.
 */

#ifndef _acilib_H_
#define _acilib_H_

#define MSG_SET_LOCAL_DATA_BASE_LEN              2
#define MSG_CONNECT_LEN                          5
#define MSG_BOND_LEN                             5
#define MSG_DISCONNECT_LEN                       2
#define MSG_BASEBAND_RESET_LEN                   1
#define MSG_WAKEUP_LEN                           1
#define MSG_SET_RADIO_TX_POWER_LEN               2
#define MSG_GET_DEVICE_ADDR_LEN                  1
#define MSG_SEND_DATA_BASE_LEN                   2
#define MSG_DATA_REQUEST_LEN                     2
#define MSG_OPEN_REMOTE_PIPE_LEN                 2
#define MSG_CLOSE_REMOTE_PIPE_LEN                2
#define MSG_DTM_CMD                              3
#define MSG_WRITE_DYNAMIC_DATA_BASE_LEN          2
#define MSG_SETUP_CMD_BASE_LEN                   1
#define MSG_ECHO_MSG_CMD_BASE_LEN                1
#define MSG_CHANGE_TIMING_LEN                    9
#define MSG_SET_APP_LATENCY_LEN                  4
#define MSG_CHANGE_TIMING_LEN_GAP_PPCP           1
#define MSG_DIRECT_CONNECT_LEN                   1
#define MSG_SET_KEY_REJECT_LEN                   2
#define MSG_SET_KEY_PASSKEY_LEN                  8
#define MSG_SET_KEY_OOB_LEN                      18
#define MSG_ACK_LEN                              2
#define MSG_NACK_LEN                             3
#define MSG_BROADCAST_LEN                        5
#define MSG_OPEN_ADV_PIPES_LEN                   9

#endif /* _acilib_H_ */
