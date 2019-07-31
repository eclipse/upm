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
#include <iostream>
#include <stdexcept>
#include <string>

#include "mcp2515.hpp"

using namespace upm;
using namespace std;

MCP2515::MCP2515(int bus, int csPin) :
    m_mcp2515(mcp2515_init(bus, csPin))
{
    if (!m_mcp2515)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_init() failed");
}

MCP2515::~MCP2515()
{
    mcp2515_close(m_mcp2515);
}

void MCP2515::reset()
{
    if (mcp2515_reset(m_mcp2515))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_reset() failed");
}

void MCP2515::setOpmode(MCP2515_OPMODE_T opmode)
{
    if (mcp2515_set_opmode(m_mcp2515, opmode))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_set_opmode() failed");
}

void MCP2515::setSpeed(MCP2515_SPEED_T speed)
{
    if (mcp2515_set_speed(m_mcp2515, speed))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_set_speed() failed");
}

void MCP2515::loadTXBuffer(MCP2515_TX_BUFFER_T bufnum, int id, bool ext,
                  bool rtr, std::string payload)
{
    if (mcp2515_load_tx_buffer(m_mcp2515, bufnum, id, ext, rtr,
                               (uint8_t *)payload.data(), payload.size()))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_load_tx_buffer() failed");

}

void MCP2515::transmitBuffer(MCP2515_TX_BUFFER_T bufnum, bool wait)
{
    if (mcp2515_transmit_buffer(m_mcp2515, bufnum, wait))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_transmit_buffer() failed");
}

bool MCP2515::isTXBufferFree(MCP2515_TX_BUFFER_T bufnum)
{
    return mcp2515_tx_buffer_is_free(m_mcp2515, bufnum);
}

MCP2515_TX_BUFFER_T MCP2515::findFreeTXBuffer()
{
    return mcp2515_find_free_tx_buffer(m_mcp2515);
}

void MCP2515::setTXBufferPriority(MCP2515_TX_BUFFER_T bufnum,
                                  MCP2515_TXP_T priority)
{
    if (mcp2515_set_tx_buffer_priority(m_mcp2515, bufnum, priority))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_set_tx_buffer_priority() failed");
}

void MCP2515::abortTX(MCP2515_TX_BUFFER_T bufnum)
{
    if (mcp2515_abort_tx(m_mcp2515, bufnum))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_abort_tx() failed");
}

void MCP2515::setRXBufferMode(MCP2515_RX_BUFFER_T bufnum, MCP2515_RXMODE_T rxm)
{
    if (mcp2515_set_rx_buffer_mode(m_mcp2515, bufnum, rxm))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_set_rx_buffer_mode() failed");
}

MCP2515_RXMSG_T MCP2515::rxStatusMsgs()
{
    return mcp2515_rx_status_msgs(m_mcp2515);
}

MCP2515_MSGTYPE_T MCP2515::rxStatusMsgType()
{
    return mcp2515_rx_status_msg_type(m_mcp2515);
}

MCP2515_FILTERMATCH_T MCP2515::rxStatusFiltermatch()
{
    return mcp2515_rx_status_filtermatch(m_mcp2515);
}

upm_result_t MCP2515::getRXMsg(MCP2515_RX_BUFFER_T bufnum)
{
    return mcp2515_get_rx_msg(m_mcp2515, bufnum, &m_message);
}

void MCP2515::printMsg()
{
    mcp2515_print_msg(m_mcp2515, &m_message);
}

void MCP2515::installISR(int pin, void (*isr)(void *), void *arg)
{
    if (mcp2515_install_isr(m_mcp2515, pin, isr, arg))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_install_isr() failed");
}

void MCP2515::uninstallISR()
{
    mcp2515_uninstall_isr(m_mcp2515);
}

void MCP2515::setIntrEnables(uint8_t enables)
{
    if (mcp2515_set_intr_enables(m_mcp2515, enables))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_set_intr_enables() failed");
}

uint8_t MCP2515::getIntrFlags()
{
    uint8_t flags = 0;
    if (mcp2515_get_intr_flags(m_mcp2515, &flags))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_get_intr_flags() failed");
    return flags;
}

void MCP2515::setIntrFlags(uint8_t flags)
{
    if (mcp2515_set_intr_flags(m_mcp2515, flags))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_set_intr_flags() failed");
}

void MCP2515::clearIntrFlags(uint8_t flags)
{
    if (mcp2515_clear_intr_flags(m_mcp2515, flags))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_clear_intr_flags() failed");
}

uint8_t MCP2515::getErrorFlags()
{
    uint8_t flags = 0;
    if (mcp2515_get_error_flags(m_mcp2515, &flags))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_get_error_flags() failed");
    return flags;
}

void MCP2515::clearErrorFlags(uint8_t flags)
{
    if (mcp2515_clear_error_flags(m_mcp2515, flags))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_clear_error_flags() failed");
}

void MCP2515::setFilter(MCP2515_RX_FILTER_T filter, bool ext, int id)
{
    if (mcp2515_set_filter(m_mcp2515, filter, ext, id))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_set_filter() failed");
}

void MCP2515::setMask(MCP2515_RX_MASK_T mask, bool ext, int id)
{
    if (mcp2515_set_mask(m_mcp2515, mask, ext, id))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_set_mask() failed");
}

string MCP2515::busRead(uint8_t cmd, std::string args, int datalen)
{
    uint8_t data[datalen];

    if (mcp2515_bus_read(m_mcp2515, cmd, (uint8_t *)args.data(), args.size(),
                         data, datalen))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_bus_read() failed");
    return string((char *)data, datalen);
}

void MCP2515::busWrite(uint8_t cmd, std::string data)
{
    if (mcp2515_bus_write(m_mcp2515, cmd, (uint8_t *)data.data(), data.size()))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_bus_write() failed");
}

void MCP2515::writeReg(uint8_t reg, uint8_t value)
{
    if (mcp2515_write_reg(m_mcp2515, reg, value))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_write_reg() failed");
}

void MCP2515::writeRegs(uint8_t reg, std::string buffer)
{
    if (mcp2515_write_regs(m_mcp2515, reg, (uint8_t *)buffer.data(),
                           buffer.size()))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_write_regs() failed");
}

void MCP2515::bitModify(uint8_t addr, uint8_t mask, uint8_t value)
{
    if (mcp2515_bit_modify(m_mcp2515, addr, mask, value))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mcp2515_bit_modify() failed");
}
