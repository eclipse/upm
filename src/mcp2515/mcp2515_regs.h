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
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// maximum number of bytes we can send/receive
#define MCP2515_MAX_PAYLOAD_DATA (8)

// This includes SIDH, SIDL, EID8, EID0, DLC, and 8 payload bytes.
// This represents the non-control components of the RX and TX
// buffers.
#define MCP2515_MAX_PKT_DATA (13)
// Now we break them out for clarity.  These are offsets into a
// transmitted or received packet.
#define MCP2515_PKT_SIDH     (0)
#define MCP2515_PKT_SIDL     (1)
#define MCP2515_PKT_EID8     (2)
#define MCP2515_PKT_EID0     (3)
#define MCP2515_PKT_DLC      (4)
#define MCP2515_PKT_D0       (5)
#define MCP2515_PKT_D1       (6)
#define MCP2515_PKT_D2       (7)
#define MCP2515_PKT_D3       (8)
#define MCP2515_PKT_D4       (9)
#define MCP2515_PKT_D5       (10)
#define MCP2515_PKT_D6       (11)
#define MCP2515_PKT_D7       (12)

// The number of bytes that make up an ID (SIDH, SIDL, EID8, EID0)
#define MCP2515_MAX_ID_BYTES (4)

    // 4 byte SIDH, SIDL, SID8, SID0 id.  This is used only for
    // converting back and forth from an integer and 4-byte device
    // representation of a standard or extended ID.
    typedef union {
        uint8_t data[MCP2515_MAX_ID_BYTES];
        struct {
            uint8_t SIDH;
            uint8_t SIDL;
            uint8_t EID8;
            uint8_t EID0;
        };
    } MCP2515_ID_T;

    // 13 byte packet starting at SIDH.  This is used to represent
    // packets for transmission and reception.
    typedef union {
        uint8_t data[MCP2515_MAX_PKT_DATA];
        struct {
            uint8_t SIDH; // Id data
            uint8_t SIDL;
            uint8_t EID8;
            uint8_t EID0;
            uint8_t DLC;  // DLC (data length)
            uint8_t D0;   // start of 8 byte (max) payload
            uint8_t D1;
            uint8_t D2;
            uint8_t D3;
            uint8_t D4;
            uint8_t D5;
            uint8_t D6;
            uint8_t D7;
        };
    } MCP2515_PKT_T;

    // A received message.  This includes the packet, and some decoded
    // data (rtr, ext, id, etc.
    typedef struct {
        int id;
        bool rtr;
        bool ext;
        int filter_num;
        int len;
        MCP2515_PKT_T pkt;
    } MCP2515_MSG_T;

    // Registers
    typedef enum {
        // 5 RX filters, each composed of SIDH, SIDL, EID8, EID0.  We
        // only specify the leading (SIDH) address here.

        // first two filters are associated with rxb0
        MCP2515_REG_FILTER0              = 0x00,
        MCP2515_REG_FILTER1              = 0x04,
        // remaining 4 filters are associated with rxb1
        MCP2515_REG_FILTER2              = 0x08,
        MCP2515_REG_FILTER3              = 0x10,
        MCP2515_REG_FILTER4              = 0x14,
        MCP2515_REG_FILTER5              = 0x18,

        // 2 RX filter mask registers, like above, we only specify the
        // SIDH address

        // associated with rxb0
        MCP2515_REG_MASK0                = 0x20,
        // associated with rxb1
        MCP2515_REG_MASK1                = 0x24,

        MCP2515_REG_BFPCTL               = 0x0c,
        MCP2515_REG_TXRTSCTRL            = 0x0d, // tx ready-to-send

        // CANSTAT and CANCTRL are aliased in several locations to
        // make it easier to get maximum data using burst reads.  In
        // reality, they are the same register, but we list them all
        // anyway for completeness.

        // CANSTAT and aliases
        MCP2515_REG_CANSTAT              = 0x0e,
        MCP2515_REG_CANSTAT_0            = 0x0e, // same as CANSTAT

        MCP2515_REG_CANSTAT_1            = 0x1e, // rest of the aliases
        MCP2515_REG_CANSTAT_2            = 0x2e,
        MCP2515_REG_CANSTAT_3            = 0x3e,
        MCP2515_REG_CANSTAT_4            = 0x4e,
        MCP2515_REG_CANSTAT_5            = 0x5e,
        MCP2515_REG_CANSTAT_6            = 0x6e,
        MCP2515_REG_CANSTAT_7            = 0x7e,

        // CANCTRL and aliases
        MCP2515_REG_CANCTRL              = 0x0f,
        MCP2515_REG_CANCTRL_0            = 0x0f, // same as CANCTRL

        MCP2515_REG_CANCTRL_1            = 0x1f, // rest of the aliases
        MCP2515_REG_CANCTRL_2            = 0x2f,
        MCP2515_REG_CANCTRL_3            = 0x3f,
        MCP2515_REG_CANCTRL_4            = 0x4f,
        MCP2515_REG_CANCTRL_5            = 0x5f,
        MCP2515_REG_CANCTRL_6            = 0x6f,
        MCP2515_REG_CANCTRL_7            = 0x7f,

        MCP2515_REG_TEC                  = 0x1c, // tx error count
        MCP2515_REG_REC                  = 0x1d, // rx error count

        MCP2515_REG_CNF3                 = 0x28, // configuration bytes
        MCP2515_REG_CNF2                 = 0x29,
        MCP2515_REG_CNF1                 = 0x2a,

        MCP2515_REG_CANINTE              = 0x2b, // intr enables
        MCP2515_REG_CANINTF              = 0x2c, // intr flags

        MCP2515_REG_EFLG                 = 0x2a, // error flags

        // Start of the buffer reg ranges for tx and rx buffers.
        // There are 3 transmit buffers and 2 rx buffers.  You can
        // read and write 16 bytes (0x0f) starting at these locations
        // to set and/get buffer control bits, message ids, msg
        // content, and CANSTAT and CANCTRL regs in one bus transaction.

        // See the datasheet page 61
        MCP2515_REG_TXB0CTRL             = 0x30, // tx buffer 0 ctrl
        MCP2515_REG_TXB1CTRL             = 0x40, // tx buffer 1 ctrl
        MCP2515_REG_TXB2CTRL             = 0x50, // tx buffer 2 ctrl

        MCP2515_REG_RXB0CTRL             = 0x60, // rx buffer 0 ctrl
        MCP2515_REG_RXB1CTRL             = 0x70 // rx buffer 1 ctrl
    } MCP2515_REG_T;

    // MCP2515_REG_CANCTRL bits
    typedef enum {
        MCP2515_CANCTRL_CLKPRE0          = 0x01,
        MCP2515_CANCTRL_CLKPRE1          = 0x02,
        _MCP2515_CANCTRL_CLKPRE_MASK     = 3,
        _MCP2515_CANCTRL_CLKPRE_SHIFT    = 0,

        MCP2515_CANCTRL_CLKEN            = 0x04, //enable CLKOUT pin

        MCP2515_CANCTRL_OSM              = 0x08, // one shot mode

        MCP2515_CANCTRL_ABAT             = 0x10, // abort all pending tx

        MCP2515_CANCTRL_REQOP0           = 0x20, // request new operating mode
        MCP2515_CANCTRL_REQOP1           = 0x40,
        MCP2515_CANCTRL_REQOP2           = 0x80,
        _MCP2515_CANCTRL_REQOP_MASK      = 7,
        _MCP2515_CANCTRL_REQOP_SHIFT     = 5,
    } MCP2515_CANCTRL_BITS_T;

    // MCP2515_CLKPRE values
    typedef enum {
        MCP2515_CLKPRE_DIV1              = 0, // sysclk/1
        MCP2515_CLKPRE_DIV2              = 1, // sysclk/2
        MCP2515_CLKPRE_DIV4              = 2, // sysclk/4
        MCP2515_CLKPRE_DIV8              = 3  // sysclk/8
    } MCP2515_CLKPRE_T;

    // MCP2515_OPMODE values.  These are the same for the CANCTRL_REQOP and
    // CANSTAT_OPMODE bitfields.
    typedef enum {
        MCP2515_OPMODE_NORMAL            = 0,
        MCP2515_OPMODE_SLEEP             = 1,
        MCP2515_OPMODE_LOOPBACK          = 2,
        MCP2515_OPMODE_LISTENONLY        = 3,
        MCP2515_OPMODE_CONFIG            = 4
    } MCP2515_OPMODE_T;

    // MCP2515_REG_CANSTAT bits
    typedef enum {
        // 0x01 reserved

        MCP2515_CANSTAT_ICOD0            = 0x02, // intr flag code
        MCP2515_CANSTAT_ICOD1            = 0x04,
        MCP2515_CANSTAT_ICOD2            = 0x08,
        _MCP2515_CANSTAT_ICOD_MASK       = 7,
        _MCP2515_CANSTAT_ICOD_SHIFT      = 1,

        // 0x10 reserved

        MCP2515_CANSTAT_OPMODE0          = 0x20, // request new operating mode
        MCP2515_CANSTAT_OPMODE1          = 0x40,
        MCP2515_CANSTAT_OPMODE2          = 0x80,
        _MCP2515_CANSTAT_OPMODE_MASK     = 7,
        _MCP2515_CANSTAT_OPMODE_SHIFT    = 5,
    } MCP2515_CANSTAT_BITS_T;

    // MCP2515_REG_BFPCTL bits, Buffer Pin Control
    typedef enum {
        MCP2515_BFPCTL_B0BFM             = 0x01, // opmode bit
        MCP2515_BFPCTL_B1BFM             = 0x02,

        MCP2515_BFPCTL_B0BFE             = 0x04, // func enable bit
        MCP2515_BFPCTL_B1BFE             = 0x08,

        MCP2515_BFPCTL_B0BFS             = 0x10, // pin state (output mode only)
        MCP2515_BFPCTL_B1BFS             = 0x20,

        // 0x40-0x080 reserved
    } MCP2515_BFPCTL_BITS_T;

    // MCP2515_REG_TXRTSCTRL bits, TX RTS pin control
    typedef enum {
        MCP2515_TXRTSCTRL_B0RTSM          = 0x01, // pin mode
        MCP2515_TXRTSCTRL_B1RTSM          = 0x02,
        MCP2515_TXRTSCTRL_B2RTSM          = 0x04,

        MCP2515_TXRTSCTRL_B0RTS           = 0x08, // pin state when in
                                                  // input mode
        MCP2515_TXRTSCTRL_B1RTS           = 0x10,
        MCP2515_TXRTSCTRL_B2RTS           = 0x20,

        // 0x40-0x80 reserved
    } MCP2515_TXRTSCTRL_BITS_T;

    // MCP2515_REG_EFLG bits, Error flags
    typedef enum {
        MCP2515_EFLG_EWARN               = 0x01, // error warning (TEC/REC > 96)
        MCP2515_EFLG_RXWAR               = 0x02, // rx warning (REC > 96)
        MCP2515_EFLG_TXWAR               = 0x04, // tx warning (REC > 96)

        MCP2515_EFLG_RXEP                = 0x08, // rx error-passive
        MCP2515_EFLG_TXEP                = 0x10, // tx error-passive

        MCP2515_EFLG_TXBO                = 0x20, // tx bus off

        MCP2515_EFLG_RX0OVR              = 0x40, // rx buf 0 overflow
        MCP2515_EFLG_RX1OVR              = 0x80, // rx buf 1 overflow
    } MCP2515_EFLG_BITS_T;

    // MCP2515_REG_CANINTE and MCP2515_REG_CANINTF bits, interrupt
    // enables and flags.  We use the same enum here as the two regs
    // have the same bits.
    typedef enum {
        MCP2515_CANINT_RX0I              = 0x01, // rx buf 0 full
        MCP2515_CANINT_RX1I              = 0x02, // rx buf 1 full

        MCP2515_CANINT_TX0I              = 0x04, // tx buf 1 empty
        MCP2515_CANINT_TX1I              = 0x08, // tx buf 2 empty
        MCP2515_CANINT_TX2I              = 0x10, // tx buf 3 empty

        MCP2515_CANINT_ERRI              = 0x20, // error intr
        MCP2515_CANINT_WAKI              = 0x40, // wakeup intr

        MCP2515_CANINT_MERR              = 0x80  // msg error
    } MCP2515_CANINT_BITS_T;

    // MCP2515_REG_TXBCTRL, TX control for tx buffers 0 (0x30), 1
    // (0x40), and 2 (0x50)
    typedef enum {
        MCP2515_TXBCTRL_TXP0              = 0x01, // message priority
        MCP2515_TXBCTRL_TXP1              = 0x02,
        _MCP2515_TXBCTRL_TXP_MASK         = 3,
        _MCP2515_TXBCTRL_TXP_SHIFT        = 0,

        // 0x04 reserved

        MCP2515_TXBCTRL_TXREQ             = 0x08, // tx request
        MCP2515_TXBCTRL_TXERR             = 0x10, // tx error detected
        MCP2515_TXBCTRL_MLOA              = 0x20, // msg lost arbitration
        MCP2515_TXBCTRL_ABTF              = 0x40  // msg aborted flag

        // 0x80 reserved
    } MCP2515_TXBCTRL_BITS_T;

    // MCP2515_TXBCTRL_TXP priority values
    typedef enum {
        MCP2515_TXP_LOWEST                = 0,
        MCP2515_TXP_LOW                   = 1,
        MCP2515_TXP_HIGH                  = 2,
        MCP2515_TXP_HIGHEST               = 3
    } MCP2515_TXP_T;

    // MCP2515_TXBDLC bits, (addresses 0x35, 0x45, 0x55)
    typedef enum {
        MCP2515_TXBDLC_DLC0               = 0x01, // data length code
        MCP2515_TXBDLC_DLC1               = 0x02, // max is 8 bytes
        MCP2515_TXBDLC_DLC2               = 0x04,
        MCP2515_TXBDLC_DLC3               = 0x08,
        _MCP2515_TXBDLC_MASK              = 15,
        _MCP2515_TXBDLC_SHIFT             = 0,

        // 0x10-0x20 reserved

        MCP2515_TXBDLC_RTR                = 0x40, // remote
                                                  // transmission
                                                  // request bit

        // 0x80 reserved
    } MCP2515_TXBDLC_BITS_T;

    // MCP2515_REG_RXB0CTRL, RX control for rx buffer 0 (0x60)
    typedef enum {
        MCP2515_RXB0CTRL_FILHIT            = 0x01, // which filter was hit

        MCP2515_RXB0CTRL_BUKT1             = 0x02, // readonly version of BUKT
        MCP2515_RXB0CTRL_BUKT              = 0x04, // rollover enable (to rx1)

        MCP2515_RXB0CTRL_RXRTR             = 0x08, // RTR request

        // 0x10 reserved

        MCP2515_RXB0CTRL_RXMODE0           = 0x20, // rx buf mode bits
        MCP2515_RXB0CTRL_RXMODE1           = 0x40,
        _MCP2515_RXB0CTRL_RXMODE_MASK      = 3,
        _MCP2515_RXB0CTRL_RXMODE_SHIFT     = 5

        // 0x80 reserved
    } MCP2515_RXB0CTRL_BITS_T;

    // These values determine whether the filters are used and what
    // kinds of messages to accept.  They are the same for both rx
    // buffers.
    typedef enum {
        MCP2515_RXMODE_ANY_FILTER          = 0, // std/ext based on filter
        MCP2515_RXMODE_STANDARD_FILTER     = 1, // standard only, filter
        MCP2515_RXMODE_EXTENDED_FILTER     = 2, // extended only, filter
        MCP2515_RXMODE_ANY_NOFILTER        = 3  // any, no filter
    } MCP2515_RXMODE_T;

    // MCP2515_REG_RXB1CTRL, RX control for rx buffer 1 (0x70)
    typedef enum {
        MCP2515_RXB1CTRL_FILHIT0           = 0x01, // which filter was hit
        MCP2515_RXB1CTRL_FILHIT1           = 0x02, // for a rollover, this can
        MCP2515_RXB1CTRL_FILHIT2           = 0x04, // be filter 0 or 1 too.
        _MCP2515_RXB1CTRL_FILHIT_MASK      = 7,
        _MCP2515_RXB1CTRL_FILHIT_SHIFT     = 0,

        MCP2515_RXB1CTRL_RXRTR             = 0x08, // RTR request

        // 0x10 reserved

        MCP2515_RXB1CTRL_RXMODE0           = 0x20, // rx buf mode bits
        MCP2515_RXB1CTRL_RXMODE1           = 0x40,
        _MCP2515_RXB1CTRL_RXMODE_MASK      = 3,
        _MCP2515_RXB1CTRL_RXMODE_SHIFT     = 5

        // 0x80 reserved
    } MCP2515_RXB1CTRL_BITS_T;

    // MCP2515_RXB1CTRL_FILHIT values for rx buffer 1
    typedef enum {
        MCP2515_FILHIT_F0                  = 0, // filter 0 hit (only if bukt)
        MCP2515_FILHIT_F1                  = 1, // filter 1 hit (only if bukt)
        MCP2515_FILHIT_F2                  = 2, // filter 2 hit
        MCP2515_FILHIT_F3                  = 3,
        MCP2515_FILHIT_F4                  = 4,
        MCP2515_FILHIT_F5                  = 5
    } MCP2515_FILHIT_T;

    // Valid CANBUS speeds.  These are precomputed and represented in
    // a table in mcp2515.c.  It is a requirement that the enum values
    // below correspond to the indexes in the speed table.  Keep them
    // in sync!  Add any new values to the end of this enum, and to
    // the corresponding table in mcp2515.c.
    //
    // These values are valid only for 16Mhz clocks.
    typedef enum {
        MCP2515_SPEED_5KBPS                = 0,
        MCP2515_SPEED_10KBPS               = 1,
        MCP2515_SPEED_20KBPS               = 2,
        MCP2515_SPEED_25KBPS               = 3,
        MCP2515_SPEED_31_25KBPS            = 4, // 31.25kbps
        MCP2515_SPEED_33KBPS               = 5,
        MCP2515_SPEED_40KBPS               = 6,
        MCP2515_SPEED_50KBPS               = 7,
        MCP2515_SPEED_80KBPS               = 8,
        MCP2515_SPEED_83KBPS               = 9,

        MCP2515_SPEED_95KBPS               = 10,
        MCP2515_SPEED_100KBPS              = 11,
        MCP2515_SPEED_125KBPS              = 12,
        MCP2515_SPEED_200KBPS              = 13,
        MCP2515_SPEED_250KBPS              = 14,
        MCP2515_SPEED_500KBPS              = 15,
        MCP2515_SPEED_666KBPS              = 16,
        MCP2515_SPEED_1000KBPS             = 17
    } MCP2515_SPEED_T;

    // MCP2515_RXBDLC bits, (addresses 0x65, 0x75)
    typedef enum {
        MCP2515_RXBDLC_DLC0               = 0x01, // data length code
        MCP2515_RXBDLC_DLC1               = 0x02, // max is 8 bytes
        MCP2515_RXBDLC_DLC2               = 0x04,
        MCP2515_RXBDLC_DLC3               = 0x08,
        _MCP2515_RXBDLC_MASK              = 15,
        _MCP2515_RXBDLC_SHIFT             = 0,

        // 0x10-0x20 reserved

        MCP2515_RXBDLC_RTR                = 0x40, // remote
                                                  // transmission
                                                  // request bit, if
                                                  // extended id

        // 0x80 reserved
    } MCP2515_RXBDLC_BITS_T;

    // SPI commands - these are commands sent to the device to start
    // certain operations.  The datasheet specifies that after CS is
    // active, the first byte must be the command byte.  It is not
    // possible to send multiple commands in a single transaction
    // (a CS On/CS Off sequence).
    typedef enum {
        MCP2515_CMD_RESET                  = 0xc0,

        MCP2515_CMD_READ                   = 0x03,

        // These commands allows the specification of a read buffer.
        // The real command is 0x90. The lower nibble is 0mn0,
        // where m and n specify the 4 locations to start reading
        // from.  In this list, we will enumerate all of them.
        //
        // n  m  cmd        real hex addr
        // ------------------------------
        // 0  0  RXB0SIDH   0x61
        // 0  1  RXB0D0     0x66
        // 1  0  RXB1SIDH   0x71
        // 1  1  RXB1D0     0x76
        MCP2515_CMD_READ_RXBUF_RXB0SIDH    = 0x90,
        MCP2515_CMD_READ_RXBUF_RXB0D0      = 0x92,
        MCP2515_CMD_READ_RXBUF_RXB1SIDH    = 0x94,
        MCP2515_CMD_READ_RXBUF_RXB1D0      = 0x96,

        MCP2515_CMD_WRITE                  = 0x02,

        // This command allows speedier loading of tx buffer contents.
        // The lower nibble is 0b0abc, where abc specifies 6 location
        // addresses to start writing to.  This works similarly to the
        // READ_RXBUF_* commands.
        //
        // a  b  c   cmd        real hex addr
        // ----------------------------------
        // 0  0  0   TXB0SIDH   0x31
        // 0  0  1   TXB0D0     0x36
        // 0  1  0   TXB1SIDH   0x41
        // 0  1  1   TXB1D0     0x46
        // 1  0  0   TXB2SIDH   0x51
        // 1  0  1   TXB2D0     0x56
        MCP2515_CMD_LOAD_TXBUF_TXB0SIDH    = 0x40,
        MCP2515_CMD_LOAD_TXBUF_TXB0D0      = 0x41,
        MCP2515_CMD_LOAD_TXBUF_TXB1SIDH    = 0x42,
        MCP2515_CMD_LOAD_TXBUF_TXB1D0      = 0x43,
        MCP2515_CMD_LOAD_TXBUF_TXB2SIDH    = 0x44,
        MCP2515_CMD_LOAD_TXBUF_TXB2D0      = 0x45,

        // Request to Send.  This is a quick way to request the
        // sending of tx buffers without having to do a
        // read/modify/write register sequence.  The lower 3 bits
        // indicate which buffer(s) to setup for transmission.
        //
        // 0x8n Where n is 0b0xxx. bit 0 is for txb 0, bit 1 for txb 1
        // and bit 2 for txb 2.
        MCP2515_CMD_RTS                    = 0x80,
        MCP2515_CMD_RTS_BUFFER0            = 0x81,
        MCP2515_CMD_RTS_BUFFER1            = 0x82,
        MCP2515_CMD_RTS_BUFFER2            = 0x84,

        // return status bits for rx and tx. See MCP2515_RDSTATUS_BITS_T.
        MCP2515_CMD_READ_STATUS            = 0xa0,

        // returns information on which filters have been matched.
        // See MCP2515_RXSTATUS_BITS_T.
        MCP2515_CMD_RX_STATUS              = 0xb0,

        // This command allows for certain registers to have certain
        // bit(s) modified w/o going through a read/modify/write cycle
        // of the whole register.  See the datasheet - only certain
        // registers can be used with this command.
        //
        // The format for this command is: cmd, addr, mask, value
        MCP2515_CMD_BIT_MODIFY             = 0x05
    } MCP2515_CMD_T;

    // This bitfield is the contents of the byte returned from a
    // CMD_READ_STATUS command.
    typedef enum {
        MCP2515_RDSTATUS_RX0IF              = 0x01, // rx buf 0 full IF
        MCP2515_RDSTATUS_RX1IF              = 0x02, // rx buf 1 full IF
        MCP2515_RDSTATUS_TXB0REQ            = 0x04, // TX buf 0 req bit
        MCP2515_RDSTATUS_TXB0IF             = 0x08, // TX buf 0 empty IF
        MCP2515_RDSTATUS_TXB1REQ            = 0x10, // TX buf 1 req bit
        MCP2515_RDSTATUS_TXB1IF             = 0x20, // TX buf 1 empty IF
        MCP2515_RDSTATUS_TXB2REQ            = 0x40, // TX buf 2 req bit
        MCP2515_RDSTATUS_TXB2IF             = 0x80  // TX buf 2 empty IF
    } MCP2515_RDSTATUS_BITS_T;

    // This bitfield is the contents of the byte returned from a
    // CMD_READ_RX_STATUS command.
    typedef enum {
        MCP2515_RXSTATUS_FILTERMATCH0       = 0x01,
        MCP2515_RXSTATUS_FILTERMATCH1       = 0x02,
        MCP2515_RXSTATUS_FILTERMATCH2       = 0x04,
        _MCP2515_RXSTATUS_FILTERMATCH_MASK  = 7,
        _MCP2515_RXSTATUS_FILTERMATCH_SHIFT = 0,

        MCP2515_RXSTATUS_MSGTYPE0           = 0x08,
        MCP2515_RXSTATUS_MSGTYPE1           = 0x10,
        _MCP2515_RXSTATUS_MSGTYPE_MASK      = 3,
        _MCP2515_RXSTATUS_MSGTYPE_SHIFT     = 3,

        // 0x20 is not mentioned in the DS

        MCP2515_RXSTATUS_RXMSG0             = 0x40,
        MCP2515_RXSTATUS_RXMSG1             = 0x80,
        _MCP2515_RXSTATUS_RXMSG_MASK        = 3,
        _MCP2515_RXSTATUS_RXMSG_SHIFT       = 6
    } MCP2515_RXSTATUS_BITS_T;

    // MCP2515_RXSTATUS_FILTERMATCH values
    typedef enum {
        MCP2515_FILTERMATCH_RXF0            = 0,
        MCP2515_FILTERMATCH_RXF1            = 1,
        MCP2515_FILTERMATCH_RXF2            = 2,
        MCP2515_FILTERMATCH_RXF3            = 3,
        MCP2515_FILTERMATCH_RXF4            = 4,
        MCP2515_FILTERMATCH_RXF5            = 5,
        MCP2515_FILTERMATCH_RXF0_ROLLOVER   = 6,
        MCP2515_FILTERMATCH_RXF1_ROLLOVER   = 7
    } MCP2515_FILTERMATCH_T;

    // MCP2515_RXSTATUS_MSGTYPE values
    typedef enum {
        MCP2515_MSGTYPE_STD                 = 0, // standard
        MCP2515_MSGTYPE_STDRF               = 1, // standard remote frame
        MCP2515_MSGTYPE_EXT                 = 2, // extended
        MCP2515_MSGTYPE_EXTRF               = 3  // extended remote frame
    } MCP2515_MSGTYPE_T;

    // MCP2515_RXSTATUS_RXMSG values
    typedef enum {
        MCP2515_RXMSG_NONE                  = 0, // no msg
        MCP2515_RXMSG_RXB0                  = 1, // msg in rxb0
        MCP2515_RXMSG_RXB1                  = 2, // msg in rxb1
        MCP2515_RXMSG_BOTH                  = 3  // msg in both
    } MCP2515_RXMSG_T;

    // we spell out the SIDL registers as they contain the EXIDE bit.
    // The others (SIDL, EID0 and EID8 just contain their respective
    // ID bits.
    typedef enum {
        MCP2515_SIDL_EID16                  = 0x01,
        MCP2515_SIDL_EID17                  = 0x02,

        // 0x04 reserved

        MCP2515_SIDL_EXIDE                  = 0x08,

        // 0x10 reserved

        MCP2515_SIDL_SID0                   = 0x20,
        MCP2515_SIDL_SID1                   = 0x40,
        MCP2515_SIDL_SID2                   = 0x80
    } MCP2515_SIDL_T;

    // An enum to specify one of the 3 TX buffers
    typedef enum {
        MCP2515_TX_BUFFER0                  = 0,
        MCP2515_TX_BUFFER1                  = 1,
        MCP2515_TX_BUFFER2                  = 2,
        // special value indicating no buffers
        MCP2515_TX_NO_BUFFERS               = 255
    } MCP2515_TX_BUFFER_T;

    // An enum to specify one of the 2 rx buffers
    typedef enum {
        MCP2515_RX_BUFFER0                  = 0,
        MCP2515_RX_BUFFER1                  = 1
    } MCP2515_RX_BUFFER_T;

    // An enum to specify one of the 6 rx filters
    typedef enum {
        MCP2515_RX_FILTER0                  = 0,
        MCP2515_RX_FILTER1                  = 1,
        MCP2515_RX_FILTER2                  = 2,
        MCP2515_RX_FILTER3                  = 3,
        MCP2515_RX_FILTER4                  = 4,
        MCP2515_RX_FILTER5                  = 5
    } MCP2515_RX_FILTER_T;

    // An enum to specify one of the 2 rx masks
    typedef enum {
        MCP2515_RX_MASK0                    = 0,
        MCP2515_RX_MASK1                    = 1
    } MCP2515_RX_MASK_T;


#ifdef __cplusplus
}
#endif
