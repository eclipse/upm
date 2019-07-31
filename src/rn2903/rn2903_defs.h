/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// maximum buffer size
#define RN2903_MAX_BUFFER (512)

// size of hex encoded 64b EUI (IPV6 Extended Unique Identifier)
#define RN2903_MAX_HEX_EUI64  (16)

// default baudrate
#define RN2903_DEFAULT_BAUDRATE (57600)

// response wait times in milliseconds
#define RN2903_DEFAULT_RESP_DELAY   (1000)   // 1 second
#define RN2903_DEFAULT_RESP2_DELAY  (60000)  // 60 seconds

// This byte sequence must follow all commands.  All responses
// will also be followed by these bytes (\r\n - CR LF).
#define RN2903_PHRASE_TERM "\r\n"
#define RN2903_PHRASE_TERM_LEN (2)

// invalid parameter
#define RN2903_PHRASE_INV_PARAM "invalid_param"
// ok
#define RN2903_PHRASE_OK "ok"

    // RN2903_MAC_STATUS_BITS_T from "mac get status" cmd
    typedef enum {
        RN2903_MAC_STATUS_JOINED              = 0x0001,

        RN2903_MAC_STATUS_MAC_STATUS0         = 0x0002,
        RN2903_MAC_STATUS_MAC_STATUS1         = 0x0004,
        RN2903_MAC_STATUS_MAC_STATUS2         = 0x0008,
        _RN2903_MAC_STATUS_MAC_STATUS_MASK    = 7,
        _RN2903_MAC_STATUS_MAC_STATUS_SHIFT   = 1,

        RN2903_MAC_STATUS_AUTO_REPLY          = 0x0010,
        RN2903_MAC_STATUS_ADR                 = 0x0020,
        RN2903_MAC_STATUS_SILENT              = 0x0040,
        RN2903_MAC_STATUS_PAUSED              = 0x0080,
        RN2903_MAC_STATUS_RFU                 = 0x0100,
        RN2903_MAC_STATUS_LINK_CHK            = 0x0200,

        RN2903_MAC_STATUS_CHAN_UPD            = 0x0400,
        RN2903_MAC_STATUS_OUT_PWR_UPD         = 0x0800,
        RN2903_MAC_STATUS_NBREP_UPD           = 0x1000,
        RN2903_MAC_STATUS_PRESCALER_UPD       = 0x2000,
        RN2903_MAC_STATUS_SECOND_RX_UPD       = 0x4000,
        RN2903_MAC_STATUS_TX_TIMING_UPD       = 0x8000,
    } RN2903_MAC_STATUS_BITS_T;

    // RN2903_MAC_STATUS_MAC_STATUS values
    typedef enum {
        RN2903_MAC_STAT_IDLE                  = 0,
        RN2903_MAC_STAT_TX_IN_PROGESS         = 1,
        RN2903_MAC_STAT_BEFORE_RX_WIN1        = 2,
        RN2903_MAC_STAT_RX_WIN1_OPEN          = 3,
        RN2903_MAC_STAT_BETWEEN_RX_WIN1_WIN2  = 4,
        RN2903_MAC_STAT_RX_WIN2_OPEN          = 5,
        RN2903_MAC_STAT_ACK_TIMEOUT           = 6,
    } RN2903_MAC_STATUS_T;

    // Join types
    typedef enum {
        RN2903_JOIN_TYPE_OTAA                 = 0, // over-the-air-activation
        RN2903_JOIN_TYPE_ABP                  = 1, // activation-by
                                                   // personalization
    } RN2903_JOIN_TYPE_T;

    // Join status
    typedef enum {
        RN2903_JOIN_STATUS_ACCEPTED           = 0,
        RN2903_JOIN_STATUS_BAD_KEYS           = 1,
        RN2903_JOIN_STATUS_NO_CHAN            = 2,
        RN2903_JOIN_STATUS_SILENT             = 3,
        RN2903_JOIN_STATUS_BUSY               = 4,
        RN2903_JOIN_STATUS_MAC_PAUSED         = 5,
        RN2903_JOIN_STATUS_DENIED             = 6,
        RN2903_JOIN_STATUS_ALREADY_JOINED     = 7,
        RN2903_JOIN_STATUS_UPM_ERROR          = 8,
    } RN2903_JOIN_STATUS_T;

    // possible flow control methods
    typedef enum {
        RN2903_FLOW_CONTROL_NONE              = 0,
        RN2903_FLOW_CONTROL_HARD,          // hardware flow control
    } RN2903_FLOW_CONTROL_T;

    // MAC TX message types
    typedef enum {
        RN2903_MAC_MSG_TYPE_UNCONFIRMED       = 0,
        RN2903_MAC_MSG_TYPE_CONFIRMED         = 1,
    } RN2903_MAC_MSG_TYPE_T;

    // MAC TX status
    typedef enum {
        RN2903_MAC_TX_STATUS_TX_OK            = 0, // tx was sent successfully
        RN2903_MAC_TX_STATUS_NOT_JOINED       = 1,
        RN2903_MAC_TX_STATUS_NO_CHAN          = 2,
        RN2903_MAC_TX_STATUS_SILENT           = 3,
        RN2903_MAC_TX_STATUS_FC_NEED_REJOIN   = 4, // frame counter overflow
        RN2903_MAC_TX_STATUS_BUSY             = 5,
        RN2903_MAC_TX_STATUS_MAC_PAUSED       = 6,
        RN2903_MAC_TX_STATUS_BAD_DATA_LEN     = 7,
        RN2903_MAC_TX_STATUS_RX_RECEIVED      = 8, // received a packet
        RN2903_MAC_TX_STATUS_MAC_ERR          = 9, // error during tx
        RN2903_MAC_TX_STATUS_UPM_ERROR        = 10, // error during tx
    } RN2903_MAC_TX_STATUS_T;

    // last command status
    typedef enum {
        RN2903_RESPONSE_OK                    = 0, // "ok", or data
        RN2903_RESPONSE_INVALID_PARAM         = 1, // "invalid_param"
        RN2903_RESPONSE_TIMEOUT               = 3,
        RN2903_RESPONSE_UPM_ERROR             = 4,
    } RN2903_RESPONSE_T;

#ifdef __cplusplus
}
#endif
