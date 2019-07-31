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
#include <string.h>
#include <assert.h>

#include "mcp2515.h"

#include <upm_platform.h>
#include <upm_utilities.h>

// in ms
#define MCP2515_MAX_TIMEOUT (5000)

// static array of cnf1-3 value for setting speed.  These values were
// taken from the Seeed code at:
// https://github.com/Seeed-Studio/CAN_BUS_Shield/

struct _mcp2515_config {
    uint8_t cnf1;
    uint8_t cnf2;
    uint8_t cnf3;
};

// NOTE: These indexes must match the enum MCP2515_SPEED_T values in
// mcp2515_regs.h!  Add any new values to the end, with a corresponding
// enum value in MCP2515_SPEED_T.
static struct _mcp2515_config mcp2515_configs[] = {
    { 0x3f, 0xff, 0x87 }, // 0 - 5kbps
    { 0x1f, 0xff, 0x87 }, // 1 - 10kbps
    { 0x0f, 0xff, 0x87 }, // 2 - 20kbps
    { 0x0f, 0xba, 0x07 }, // 3 - 25kbps
    { 0x0f, 0xf1, 0x85 }, // 4 - 31k25kbps
    { 0x09, 0xbe, 0x07 }, // 5 - 33kbps
    { 0x07, 0xff, 0x87 }, // 6 - 40kbps
    { 0x07, 0xfa, 0x87 }, // 7 - 50kbps
    { 0x03, 0xff, 0x87 }, // 8 - 80kbps
    { 0x03, 0xbe, 0x07 }, // 9 - 83kbps

    { 0x03, 0xad, 0x07 }, // 10 - 95kbps
    { 0x03, 0xfa, 0x87 }, // 11 - 100kbps
    { 0x03, 0xf0, 0x86 }, // 12 - 125kbps
    { 0x01, 0xfa, 0x87 }, // 13 - 200kbps
    { 0x41, 0xf1, 0x85 }, // 14 - 250kbps
    { 0x00, 0xf0, 0x86 }, // 15 - 500kbps
    { 0x00, 0xa0, 0x04 }, // 16 - 666kbps
    { 0x00, 0xd0, 0x82 }, // 17 - 1000kbps
};

// For SPI, these are our CS on/off functions, if needed
static void mcp2515_cs_on(const mcp2515_context dev)
{
    assert(dev != NULL);

    if (dev->gpio)
        mraa_gpio_write(dev->gpio, 0);
}

static void mcp2515_cs_off(const mcp2515_context dev)
{
    assert(dev != NULL);

    if (dev->gpio)
        mraa_gpio_write(dev->gpio, 1);
}

// convert an integer id into the 4-byte breakout format used by the
// device.
static void mcp2515_int_to_id(const mcp2515_context dev, int id, bool ext,
                              bool is_filter, MCP2515_ID_T *did)
{
    assert(dev != NULL);
    assert(did != NULL);

    // mask off all but the lower 29 bits
    id &= 0x1fffffff;

    did->SIDH = (id >> 3) & 0xff;
    did->SIDL = ((id & 0x07) << 5);
    did->EID8 = 0;
    did->EID0 = 0;

    // This is a sort of special case (ext vs. filter).  When using
    // this function to create a filter, but the msg type is standard,
    // the filtering/masking engine can use the first 16 bits of
    // extended filter data (EID8, EID0) to compare against the first
    // 16 bits of the data payload (D0 and D1).  For a standard id,
    // the extended bits 16 and 17 are ignored, so we only include
    // these (along with the extended bit) on true extended ids
    // regardless of filtering.
    if (ext || is_filter)
    {
        if (ext)
        {
            // add ext bits 17 and 16
            did->SIDL |= ((id & 0x18000000) >> 27) & 0x03;
            // set the extended id bit
            did->SIDL |= MCP2515_SIDL_EXIDE;
        }
        // now fill in the extended bits
        did->EID8 = (id >> 19) & 0xff;
        did->EID0 = (id >> 11) & 0xff;
    }
}

// convert the 4-byte breakout format used by the device into an integer id
static int mcp2515_id_to_int(const mcp2515_context dev, bool *ext,
                             MCP2515_ID_T *did)
{
    assert(dev != NULL);
    assert(did != NULL);

    int id = 0;
    *ext = false;

    id |= (did->SIDH << 3);
    id |= (did->SIDL >> 5) & 0x07;

    // is it an extended id?
    if (did->SIDL & MCP2515_SIDL_EXIDE)
    {
        *ext = true;
        id |= ((did->SIDL & 0x03) << 27);

        id |= (did->EID8 << 19);
        id |= (did->EID0 << 11);
    }

    return id;
}

// init...
mcp2515_context mcp2515_init(int bus, int cs_pin)
{
    mcp2515_context dev =
        (mcp2515_context)malloc(sizeof(struct _mcp2515_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _mcp2515_context));

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        mcp2515_close(dev);
        return NULL;
    }

    if (!(dev->spi = mraa_spi_init(bus)))
    {
        printf("%s: mraa_spi_init() failed.\n", __FUNCTION__);
        mcp2515_close(dev);
        return NULL;
    }

    // Only create cs context if we are actually using a valid pin.
    // A hardware controlled pin should specify cs as -1.
    if (cs_pin >= 0)
    {
        if (!(dev->gpio = mraa_gpio_init(cs_pin)))
        {
            printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
            mcp2515_close(dev);
            return NULL;
        }

        mraa_gpio_dir(dev->gpio, MRAA_GPIO_OUT);
        mcp2515_cs_off(dev);
    }

    if (mraa_spi_mode(dev->spi, MRAA_SPI_MODE0))
    {
        printf("%s: mraa_spi_mode() failed.\n", __FUNCTION__);
        mcp2515_close(dev);
        return NULL;
    }

    if (mraa_spi_frequency(dev->spi, 10000000))
    {
        printf("%s: mraa_spi_frequency() failed.\n", __FUNCTION__);
        mcp2515_close(dev);
        return NULL;
    }

    // reset
    if (mcp2515_reset(dev))
    {
        printf("%s: mcp2515_reset() failed.\n", __FUNCTION__);
        mcp2515_close(dev);
        return NULL;
    }

    // make sure the mode is config (should be after a reset), set a
    // default speed, and then set normal mode.
    if (mcp2515_set_opmode(dev, MCP2515_OPMODE_CONFIG))
    {
        printf("%s: mcp2515_set_opmode(config) failed.\n", __FUNCTION__);
        mcp2515_close(dev);
        return NULL;
    }

    if (mcp2515_set_speed(dev, MCP2515_SPEED_50KBPS))
    {
        printf("%s: mcp2515_set_speed() failed.\n", __FUNCTION__);
        mcp2515_close(dev);
        return NULL;
    }

    // clear all filters and masks
    if (mcp2515_set_filter(dev, MCP2515_RX_FILTER0, false, 0)
        || mcp2515_set_filter(dev, MCP2515_RX_FILTER1, false, 0)
        || mcp2515_set_filter(dev, MCP2515_RX_FILTER2, false, 0)
        || mcp2515_set_filter(dev, MCP2515_RX_FILTER3, false, 0)
        || mcp2515_set_filter(dev, MCP2515_RX_FILTER4, false, 0)
        || mcp2515_set_filter(dev, MCP2515_RX_FILTER5, false, 0))
    {
        printf("%s: mcp2515_set_filter() failed.\n", __FUNCTION__);
        mcp2515_close(dev);
        return NULL;
    }

    if (mcp2515_set_mask(dev, MCP2515_RX_MASK0, false, 0)
        || mcp2515_set_mask(dev, MCP2515_RX_MASK1, false, 0))
    {
        printf("%s: mcp2515_set_mask() failed.\n", __FUNCTION__);
        mcp2515_close(dev);
        return NULL;
    }

    // set the mode for any filter.  With the above settings, all
    // packets will be received since all of the mode bits are 0, so
    // the filter bits are ignored.
    if (mcp2515_set_rx_buffer_mode(dev, MCP2515_RX_BUFFER0,
                                   MCP2515_RXMODE_ANY_FILTER)
        || mcp2515_set_rx_buffer_mode(dev, MCP2515_RX_BUFFER1,
                                      MCP2515_RXMODE_ANY_FILTER))
    {
        printf("%s: mcp2515_set_rx_buffer_mode() failed.\n", __FUNCTION__);
        mcp2515_close(dev);
        return NULL;
    }

    // turn off all interrupt enables
    if (mcp2515_set_intr_enables(dev, 0))
    {
        printf("%s: mcp2515_set_intr_enables() failed.\n", __FUNCTION__);
        mcp2515_close(dev);
        return NULL;
    }

    // switch to normal operating mode
    if (mcp2515_set_opmode(dev, MCP2515_OPMODE_NORMAL))
    {
        printf("%s: mcp2515_set_opmode(normal) failed.\n", __FUNCTION__);
        mcp2515_close(dev);
        return NULL;
    }

    return dev;
}

void mcp2515_close(mcp2515_context dev)
{
    assert(dev != NULL);

    mcp2515_uninstall_isr(dev);

    if (dev->spi)
        mraa_spi_stop(dev->spi);

    if (dev->gpio)
        mraa_gpio_close(dev->gpio);

    free(dev);
}

// bus read and write functions
upm_result_t mcp2515_bus_read(const mcp2515_context dev, uint8_t cmd,
                              uint8_t *args, unsigned int arglen,
                              uint8_t *data, uint8_t len)
{
    int buflen = len + 1 + arglen;
    uint8_t sbuf[buflen];

    memset((void *)sbuf, 0, buflen);

    int index = 0;
    sbuf[index++] = cmd;

    if (args && arglen)
    {
        for (unsigned int i=0; i<arglen; i++)
            sbuf[index++] = args[i];
    }

    mcp2515_cs_on(dev);

    if (mraa_spi_transfer_buf(dev->spi, sbuf, sbuf, buflen))
    {
        mcp2515_cs_off(dev);
        printf("%s: mraa_spi_transfer_buf() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }
    mcp2515_cs_off(dev);

    // now copy it into user buffer
    for (int i=0; i<len; i++)
        data[i] = sbuf[i + 1 + arglen];

    return UPM_SUCCESS;
}

upm_result_t mcp2515_bus_write(const mcp2515_context dev, uint8_t cmd,
                               uint8_t *data, uint8_t len)
{
    assert(dev != NULL);

    uint8_t sbuf[len + 1];
    memset((void *)sbuf, 0, len + 1);
    sbuf[0] = cmd;

    // copy in the data to write...
    if (data && len)
    {
        for (int i=0; i<len; i++)
            sbuf[i + 1] = data[i];
    }

    mcp2515_cs_on(dev);

    if (mraa_spi_transfer_buf(dev->spi, sbuf, sbuf, len + 1))
    {
        mcp2515_cs_off(dev);
        printf("%s: mraa_spi_transfer_buf() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }
    mcp2515_cs_off(dev);

    return UPM_SUCCESS;
}

upm_result_t mcp2515_write_reg(const mcp2515_context dev, uint8_t reg,
                               uint8_t value)
{
    assert(dev != NULL);

    uint8_t arg[2]; // reg addr, value

    arg[0] = reg;
    arg[1] = value;

    return mcp2515_bus_write(dev, MCP2515_CMD_WRITE, arg, 2);
}

upm_result_t mcp2515_write_regs(const mcp2515_context dev, uint8_t reg,
                                uint8_t *buffer, int len)
{
    assert(dev != NULL);
    assert(buffer != NULL);

    int writeLen = len + 1;
    uint8_t arg[1+len]; // reg addr, data...

    arg[0] = reg;

    for (int i=0; i<len; i++)
        arg[i+1] = buffer[i];

    return mcp2515_bus_write(dev, MCP2515_CMD_WRITE, arg, writeLen);
}

upm_result_t mcp2515_read_reg(const mcp2515_context dev, uint8_t reg,
                              uint8_t *value)
{
    assert(dev != NULL);

    uint8_t arg = reg;

    return mcp2515_bus_read(dev, MCP2515_CMD_READ, &arg, 1, value, 1);
}

upm_result_t mcp2515_reset(const mcp2515_context dev)
{
    assert(dev != NULL);

    upm_result_t rv = mcp2515_bus_write(dev, MCP2515_CMD_RESET, NULL, 0);
    upm_delay_ms(100);

    return rv;
}

upm_result_t mcp2515_bit_modify(const mcp2515_context dev, uint8_t addr,
                                uint8_t mask, uint8_t value)
{
    assert(dev != NULL);

    // build the command and send it
    uint8_t args[3]; // addr, mask, value
    args[0] = addr;
    args[1] = mask;
    args[2] = value;

    return mcp2515_bus_write(dev, MCP2515_CMD_BIT_MODIFY, args, 3);
}

upm_result_t mcp2515_set_opmode(const mcp2515_context dev,
                                MCP2515_OPMODE_T opmode)
{
    assert(dev != NULL);

    // make it so
    upm_result_t rv =
        mcp2515_bit_modify(dev, MCP2515_REG_CANCTRL,
                           (_MCP2515_CANCTRL_REQOP_MASK
                            << _MCP2515_CANCTRL_REQOP_SHIFT),
                           (opmode
                            << _MCP2515_CANCTRL_REQOP_SHIFT));

    if (rv)
    {
        printf("%s: mcp2515_bit_modify() failed\n", __FUNCTION__);
        return rv;
    }

    // now spin until the mode is set (there can be a delay if the
    // chip is busy sending or receiving packets, or, secretly
    // communicating with our alien overlords).
    // We set an upper limit of 5 seconds before we decide to timeout.
    const uint8_t shiftmask = (_MCP2515_CANSTAT_OPMODE_MASK
                               << _MCP2515_CANSTAT_OPMODE_SHIFT);

    bool done = false;
    upm_clock_t clock = upm_clock_init();

    do
    {
        uint8_t reg;

        rv = mcp2515_read_reg(dev, MCP2515_REG_CANSTAT, &reg);
        if (rv)
        {
            printf("%s: mcp2515_bus_read() failed\n", __FUNCTION__);
            return rv;
        }

        if ((reg & shiftmask) == (opmode << _MCP2515_CANSTAT_OPMODE_SHIFT))
            done = true;
        else
            upm_delay_ms(10);

    } while (!done && (upm_elapsed_ms(&clock) < MCP2515_MAX_TIMEOUT));

    if (!done)
        return UPM_ERROR_TIMED_OUT;
    else
        return UPM_SUCCESS;
}

upm_result_t mcp2515_set_speed(const mcp2515_context dev,
                               MCP2515_SPEED_T speed)
{
    assert(dev != NULL);

    upm_result_t rv;

    if ((rv = mcp2515_write_reg(dev, MCP2515_REG_CNF1,
                                mcp2515_configs[speed].cnf1)))
        return rv;

    if ((rv = mcp2515_write_reg(dev, MCP2515_REG_CNF2,
                                mcp2515_configs[speed].cnf2)))
        return rv;

    if ((rv = mcp2515_write_reg(dev, MCP2515_REG_CNF3,
                                mcp2515_configs[speed].cnf3)))
        return rv;

    return UPM_SUCCESS;
}

upm_result_t mcp2515_load_tx_buffer(const mcp2515_context dev,
                                    MCP2515_TX_BUFFER_T bufnum,
                                    int id, bool ext, bool rtr,
                                    uint8_t *payload, int len)
{
    assert(dev != NULL);

    // The first thing to do is make sure that the tx buffer is
    // actually available (ie: does not already contain a packet
    // awaiting transmit).

    if (!mcp2515_tx_buffer_is_free(dev, bufnum))
    {
        printf("%s: buffer is busy\n", __FUNCTION__);
        return UPM_ERROR_NO_RESOURCES;
    }

    uint8_t cmd = 0;
    switch (bufnum)
    {
    case MCP2515_TX_BUFFER0:
        cmd = MCP2515_CMD_LOAD_TXBUF_TXB0SIDH;
        break;

    case MCP2515_TX_BUFFER1:
        cmd = MCP2515_CMD_LOAD_TXBUF_TXB1SIDH;
        break;

    case MCP2515_TX_BUFFER2:
        cmd = MCP2515_CMD_LOAD_TXBUF_TXB2SIDH;
        break;

    default:
        printf("%s: invalid buffer specified\n", __FUNCTION__);
        return UPM_ERROR_INVALID_PARAMETER;
    }

    // start preparing for the load
    MCP2515_ID_T idBuf;
    MCP2515_PKT_T packet;

    memset(&packet, 0, sizeof(MCP2515_PKT_T));

    // first add the id converted to the 4-byte id the device requires
    // mask off all but the lower 29 bits
    id &= 0x1fffffff;
    mcp2515_int_to_id(dev, id, ext, false, &idBuf);

    // copy in the [device]id bytes, then fill in the DLC reg.
    packet.SIDH = idBuf.SIDH;
    packet.SIDL = idBuf.SIDL;
    packet.EID8 = idBuf.EID8;
    packet.EID0 = idBuf.EID0;

    // DLC register
    if (len > MCP2515_MAX_PAYLOAD_DATA)
        len = MCP2515_MAX_PAYLOAD_DATA;
    if (len < 0)
        len = 0;

    packet.DLC = (len & _MCP2515_TXBDLC_MASK) << _MCP2515_TXBDLC_SHIFT;

    if (rtr)
        packet.DLC |= MCP2515_TXBDLC_RTR;

    // now the payload
    for (int i=0; i<len; i++)
        packet.data[i+MCP2515_PKT_D0] = payload[i];

    // load the buffer
    if (mcp2515_bus_write(dev, cmd, packet.data, MCP2515_MAX_PKT_DATA))
    {
        printf("%s: mcp2515_bus_write() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t mcp2515_transmit_buffer(const mcp2515_context dev,
                                     MCP2515_TX_BUFFER_T bufnum,
                                     bool wait)
{
    assert(dev != NULL);

    uint8_t waitreg = 0;
    uint8_t rtscmd = 0;
    switch (bufnum)
    {
    case MCP2515_TX_BUFFER0:
        waitreg = MCP2515_REG_TXB0CTRL;
        rtscmd = MCP2515_CMD_RTS_BUFFER0;
        break;

    case MCP2515_TX_BUFFER1:
        waitreg = MCP2515_REG_TXB1CTRL;
        rtscmd = MCP2515_CMD_RTS_BUFFER1;
        break;

    case MCP2515_TX_BUFFER2:
        waitreg = MCP2515_REG_TXB2CTRL;
        rtscmd = MCP2515_CMD_RTS_BUFFER2;
        break;

    default:
        return UPM_ERROR_INVALID_PARAMETER;
    }

    // now transmit the requested buffer
    if (mcp2515_bus_write(dev, rtscmd, NULL, 0))
        return UPM_ERROR_OPERATION_FAILED;

    // if we are not waiting for transmission to complete, we are done
    // here.
    if (!wait)
        return UPM_SUCCESS;

    // now spin with timeout waiting for it to be transmitted
    upm_clock_t clock =  upm_clock_init();
    bool done = false;

    do
    {
        uint8_t txbctrl = 0;

        if (mcp2515_read_reg(dev, waitreg, &txbctrl))
            return UPM_ERROR_OPERATION_FAILED;

        if (!(txbctrl & MCP2515_TXBCTRL_TXREQ))
            done = true;

        upm_delay_ms(1);
    } while (!done && (upm_elapsed_ms(&clock) < MCP2515_MAX_TIMEOUT));

    if (!done)
        return UPM_ERROR_TIMED_OUT;
    else
        return UPM_SUCCESS;
}

bool mcp2515_tx_buffer_is_free(const mcp2515_context dev,
                               MCP2515_TX_BUFFER_T bufnum)
{
    assert(dev != NULL);

    uint8_t chkreg = 0;
    switch (bufnum)
    {
    case MCP2515_TX_BUFFER0:
        chkreg = MCP2515_REG_TXB0CTRL;
        break;

    case MCP2515_TX_BUFFER1:
        chkreg = MCP2515_REG_TXB1CTRL;
        break;

    case MCP2515_TX_BUFFER2:
        chkreg = MCP2515_REG_TXB2CTRL;
        break;

    default:
        return false;
    }

    uint8_t txbctrl = 0;

    if (mcp2515_read_reg(dev, chkreg, &txbctrl))
        return false;

    if (!(txbctrl & MCP2515_TXBCTRL_TXREQ))
        return true;
    else
        return false;
}

MCP2515_TX_BUFFER_T mcp2515_find_free_tx_buffer(const mcp2515_context dev)
{
    assert(dev != NULL);

    // check each of the tx buffers and return one if it's free.
    if (mcp2515_tx_buffer_is_free(dev, MCP2515_TX_BUFFER0))
        return MCP2515_TX_BUFFER0;

    if (mcp2515_tx_buffer_is_free(dev, MCP2515_TX_BUFFER1))
        return MCP2515_TX_BUFFER1;

    if (mcp2515_tx_buffer_is_free(dev, MCP2515_TX_BUFFER2))
        return MCP2515_TX_BUFFER2;

    return MCP2515_TX_NO_BUFFERS;
}

// can only be set in CONFIG mode
upm_result_t mcp2515_set_filter(const mcp2515_context dev,
                                MCP2515_RX_FILTER_T filter, bool ext, int id)
{
    assert(dev != NULL);

    MCP2515_ID_T idBuf;
    mcp2515_int_to_id(dev, id, ext, true, &idBuf);

    uint8_t reg = 0;

    switch (filter)
    {
    case MCP2515_RX_FILTER0:
        reg = MCP2515_REG_FILTER0;
        break;

    case MCP2515_RX_FILTER1:
        reg = MCP2515_REG_FILTER1;
        break;

    case MCP2515_RX_FILTER2:
        reg = MCP2515_REG_FILTER2;
        break;

    case MCP2515_RX_FILTER3:
        reg = MCP2515_REG_FILTER3;
        break;

    case MCP2515_RX_FILTER4:
        reg = MCP2515_REG_FILTER4;
        break;

    case MCP2515_RX_FILTER5:
        reg = MCP2515_REG_FILTER5;
        break;

    default:
        return UPM_ERROR_INVALID_PARAMETER;
    }

    return mcp2515_write_regs(dev, reg, idBuf.data, MCP2515_MAX_ID_BYTES);
}

// can only be set in CONFIG mode
upm_result_t mcp2515_set_mask(const mcp2515_context dev,
                              MCP2515_RX_MASK_T mask, bool ext, int id)
{
    assert(dev != NULL);

    MCP2515_ID_T idBuf;
    mcp2515_int_to_id(dev, id, ext, true, &idBuf);

    uint8_t reg = 0;

    switch (mask)
    {
    case MCP2515_RX_MASK0:
        reg = MCP2515_REG_MASK0;
        break;
    case MCP2515_RX_MASK1:
        reg = MCP2515_REG_MASK1;
        break;
    default:
        return UPM_ERROR_INVALID_PARAMETER;
    }

    return mcp2515_write_regs(dev, reg, idBuf.data, MCP2515_MAX_ID_BYTES);
}

upm_result_t mcp2515_set_rx_buffer_mode(const mcp2515_context dev,
                                        MCP2515_RX_BUFFER_T bufnum,
                                        MCP2515_RXMODE_T rxm)
{
    assert(dev != NULL);

    uint8_t reg = 0;
    uint8_t mask = 0, shift = 0;
    switch (bufnum)
    {
    case MCP2515_RX_BUFFER0:
        reg = MCP2515_REG_RXB0CTRL;
        mask = _MCP2515_RXB0CTRL_RXMODE_MASK;
        shift = _MCP2515_RXB0CTRL_RXMODE_SHIFT;
        break;

    case MCP2515_RX_BUFFER1:
        reg = MCP2515_REG_RXB1CTRL;
        mask = _MCP2515_RXB1CTRL_RXMODE_MASK;
        shift = _MCP2515_RXB1CTRL_RXMODE_SHIFT;
        break;

    default:
        return UPM_ERROR_INVALID_PARAMETER;
    }

    return mcp2515_bit_modify(dev, reg, mask << shift, rxm << shift);
}

upm_result_t mcp2515_set_tx_buffer_priority(const mcp2515_context dev,
                                            MCP2515_TX_BUFFER_T bufnum,
                                            MCP2515_TXP_T priority)
{
    assert(dev != NULL);

    uint8_t reg = 0;
    uint8_t mask = _MCP2515_TXBCTRL_TXP_MASK;
    uint8_t shift = _MCP2515_TXBCTRL_TXP_SHIFT;
    switch (bufnum)
    {
    case MCP2515_TX_BUFFER0:
        reg = MCP2515_REG_TXB0CTRL;
        break;

    case MCP2515_TX_BUFFER1:
        reg = MCP2515_REG_TXB1CTRL;
        break;

    case MCP2515_TX_BUFFER2:
        reg = MCP2515_REG_TXB2CTRL;
        break;

    default:
        printf("%s: invalid buffer specified\n", __FUNCTION__);
        return UPM_ERROR_INVALID_PARAMETER;
    }

    return mcp2515_bit_modify(dev, reg, mask << shift, priority << shift);
}

MCP2515_RXMSG_T mcp2515_rx_status_msgs(const mcp2515_context dev)
{
    assert(dev != NULL);

    uint8_t rx_status_byte;
    if (mcp2515_bus_read(dev, MCP2515_CMD_RX_STATUS, NULL, 0,
                               &rx_status_byte, 1))
        return MCP2515_RXMSG_NONE;

    MCP2515_RXMSG_T buffers =
        (rx_status_byte &
         (_MCP2515_RXSTATUS_RXMSG_MASK << _MCP2515_RXSTATUS_RXMSG_SHIFT))
        >> _MCP2515_RXSTATUS_RXMSG_SHIFT;

    return buffers;
}

MCP2515_MSGTYPE_T mcp2515_rx_status_msg_type(const mcp2515_context dev)
{
    assert(dev != NULL);

    uint8_t rx_status_byte;
    if (mcp2515_bus_read(dev, MCP2515_CMD_RX_STATUS, NULL, 0,
                               &rx_status_byte, 1))
        return MCP2515_MSGTYPE_STD; // not really, but....

    MCP2515_MSGTYPE_T msgtype =
        (rx_status_byte &
         (_MCP2515_RXSTATUS_MSGTYPE_MASK << _MCP2515_RXSTATUS_MSGTYPE_SHIFT))
        >> _MCP2515_RXSTATUS_MSGTYPE_SHIFT;

    return msgtype;
}

MCP2515_FILTERMATCH_T mcp2515_rx_status_filtermatch(const mcp2515_context dev)
{
    assert(dev != NULL);

    uint8_t rx_status_byte;
    if (mcp2515_bus_read(dev, MCP2515_CMD_RX_STATUS, NULL, 0,
                               &rx_status_byte, 1))
        return MCP2515_FILTERMATCH_RXF0; // not really, but....

    MCP2515_FILTERMATCH_T fm =
        (rx_status_byte &
         (_MCP2515_RXSTATUS_FILTERMATCH_MASK
          << _MCP2515_RXSTATUS_FILTERMATCH_SHIFT))
        >> _MCP2515_RXSTATUS_FILTERMATCH_SHIFT;

    return fm;
}

upm_result_t mcp2515_get_rx_msg(const mcp2515_context dev,
                                MCP2515_RX_BUFFER_T bufnum,
                                MCP2515_MSG_T *msg)
{
    assert(dev != NULL);
    assert(msg != NULL);

    uint8_t cmd = 0;
    uint8_t rxbctrl_reg = 0;
    switch (bufnum)
    {
    case MCP2515_RX_BUFFER0:
        cmd = MCP2515_CMD_READ_RXBUF_RXB0SIDH;
        rxbctrl_reg = MCP2515_REG_RXB0CTRL;
        break;

    case MCP2515_RX_BUFFER1:
        cmd = MCP2515_CMD_READ_RXBUF_RXB1SIDH;
        rxbctrl_reg = MCP2515_REG_RXB1CTRL;
        break;

    default:
        return UPM_ERROR_INVALID_PARAMETER;
    }

    // first see if we have a message waiting
    MCP2515_RXMSG_T rxmsgs = mcp2515_rx_status_msgs(dev);
    bool msgavail = false;
    if (rxmsgs == MCP2515_RXMSG_BOTH)
        msgavail = true;
    else if (rxmsgs == MCP2515_RXMSG_RXB0 && bufnum == MCP2515_RX_BUFFER0)
        msgavail = true;
    else if (rxmsgs == MCP2515_RXMSG_RXB1 && bufnum == MCP2515_RX_BUFFER1)
        msgavail = true;

    // nope
    if (!msgavail)
        return UPM_ERROR_NO_RESOURCES;

    // get the control reg
    upm_result_t rv;
    uint8_t rxbctrl;
    if ((rv = mcp2515_read_reg(dev, rxbctrl_reg, &rxbctrl)))
        return rv;

    // go ahead and grab the message and fill in the details
    memset(msg, 0, sizeof(MCP2515_MSG_T));
    if ((rv = mcp2515_bus_read(dev, cmd, NULL, 0,
                               msg->pkt.data, MCP2515_MAX_PKT_DATA)))
    {
        printf("mcp2515_bus_read failed\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    // if we are here, we got the packet, so decode and determine some
    // things.

    // id & ext
    MCP2515_ID_T did;
    did.SIDH = msg->pkt.SIDH;
    did.SIDL = msg->pkt.SIDL;
    did.EID8 = msg->pkt.EID8;
    did.EID0 = msg->pkt.EID0;

    msg->id = mcp2515_id_to_int(dev, &(msg->ext), &did);

    // rtr
    if (!msg->ext)
    {
        // The RTR flag bit is the same for both the RXB0CTRL and
        // RXB1CTRL registers, so it's safe to just compare against
        // the first regardless of the buffer we are interested in.
        if (rxbctrl & MCP2515_RXB0CTRL_RXRTR)
            msg->rtr = true;
    }
    else
    {
        // ext message stores rtr separately
        if (msg->pkt.DLC & MCP2515_RXBDLC_RTR)
            msg->rtr = true;
    }

    // filter num
    if (bufnum == MCP2515_RX_BUFFER0)
    {
        if (rxbctrl & MCP2515_RXB0CTRL_FILHIT)
            msg->filter_num = 1;
        else
            msg->filter_num = 0;

    }
    else
    {
        msg->filter_num =
            (int)((rxbctrl &
                   (_MCP2515_RXB1CTRL_FILHIT_MASK
                    << _MCP2515_RXB1CTRL_FILHIT_SHIFT))
                  >> _MCP2515_RXB1CTRL_FILHIT_SHIFT);
    }

    // now the payload length
    msg->len = ((msg->pkt.DLC & (_MCP2515_RXBDLC_MASK << _MCP2515_RXBDLC_SHIFT))
                >> _MCP2515_RXBDLC_SHIFT);


    return UPM_SUCCESS;
}

void mcp2515_print_msg(const mcp2515_context dev, MCP2515_MSG_T *msg)
{
    assert(dev != NULL);
    assert(msg != NULL);

    // print it out
    printf("id %08x ext %d rtr %d filt %d len %d\n",
           msg->id, msg->ext, msg->rtr, msg->filter_num, msg->len);
    printf("\tpayload: ");
    for (int i=0; i<msg->len; i++)
        printf("0x%02x ", msg->pkt.data[MCP2515_PKT_D0 + i]);
    printf("\n");
}

upm_result_t mcp2515_abort_tx(const mcp2515_context dev,
                              MCP2515_TX_BUFFER_T bufnum)
{
    assert(dev != NULL);

    uint8_t reg;
    switch (bufnum)
    {
    case MCP2515_TX_BUFFER0:
        reg = MCP2515_REG_TXB0CTRL;
        break;

    case MCP2515_TX_BUFFER1:
        reg = MCP2515_REG_TXB1CTRL;
        break;

    case MCP2515_TX_BUFFER2:
        reg = MCP2515_REG_TXB2CTRL;
        break;

    default:
        return UPM_ERROR_INVALID_PARAMETER;
    }

    // Clear the TXREQ bit.
    return mcp2515_bit_modify(dev, reg, MCP2515_TXBCTRL_TXREQ, 0);
}

upm_result_t mcp2515_install_isr(const mcp2515_context dev, int pin,
                                 void (*isr)(void *), void *arg)
{
    assert(dev != NULL);

    mcp2515_uninstall_isr(dev);

    if ( !(dev->intr = mraa_gpio_init(pin)) )
    {
        printf("%s: mraa_gpio_init failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    mraa_gpio_dir(dev->intr, MRAA_GPIO_IN);

    // install our interrupt handler
    mraa_gpio_isr(dev->intr, MRAA_GPIO_EDGE_FALLING,
                  isr, arg);

    return UPM_SUCCESS;
}

void mcp2515_uninstall_isr(const mcp2515_context dev)
{
    assert(dev != NULL);

    if (!dev->intr)
        return;

    mraa_gpio_isr_exit(dev->intr);
    mraa_gpio_close(dev->intr);
    dev->intr = NULL;
}

upm_result_t mcp2515_set_intr_enables(const mcp2515_context dev,
                                      uint8_t enables)
{
    assert(dev != NULL);

    return mcp2515_write_reg(dev, MCP2515_REG_CANINTE, enables);
}

upm_result_t mcp2515_get_intr_flags(const mcp2515_context dev, uint8_t *flags)
{
    assert(dev != NULL);

    return mcp2515_read_reg(dev, MCP2515_REG_CANINTF, flags);
}

upm_result_t mcp2515_set_intr_flags(const mcp2515_context dev, uint8_t flags)
{
    assert(dev != NULL);

    return mcp2515_bit_modify(dev, MCP2515_REG_CANINTF, flags, 0xff);
}

upm_result_t mcp2515_clear_intr_flags(const mcp2515_context dev, uint8_t flags)
{
    assert(dev != NULL);

    return mcp2515_bit_modify(dev, MCP2515_REG_CANINTF, flags, 0);
}

upm_result_t mcp2515_get_error_flags(const mcp2515_context dev, uint8_t *flags)
{
    assert(dev != NULL);

    return mcp2515_read_reg(dev, MCP2515_REG_EFLG, flags);
}

upm_result_t mcp2515_clear_error_flags(const mcp2515_context dev,
                                       uint8_t flags)
{
    assert(dev != NULL);

    return mcp2515_bit_modify(dev, MCP2515_REG_EFLG, flags, 0);
}
