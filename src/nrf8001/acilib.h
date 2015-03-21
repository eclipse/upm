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
