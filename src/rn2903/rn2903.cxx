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

#include <iostream>
#include <stdexcept>
#include <string.h>

#include "rn2903.hpp"

using namespace upm;
using namespace std;

RN2903::RN2903(unsigned int uart, unsigned int baudrate) :
    m_rn2903(rn2903_init(uart, baudrate))
{
    if (!m_rn2903)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_init() failed");
}

RN2903::RN2903(string uart_path, unsigned int baudrate) :
    m_rn2903(rn2903_init_tty(uart_path.c_str(), baudrate))
{
    if (!m_rn2903)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_init_tty() failed");
}

RN2903::~RN2903()
{
    rn2903_close(m_rn2903);
}

std::string RN2903::read(int size)
{
    char buffer[size];

    int rv;

    if ((rv = rn2903_read(m_rn2903, buffer, (size_t)size)) < 0)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_read() failed");

    return string(buffer, rv);
}

int RN2903::write(std::string buffer)
{
    int rv;

    if ((rv = rn2903_write(m_rn2903, (char*)buffer.data(),
                           buffer.size())) < 0)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_write() failed");

    return rv;
}

void RN2903::setResponseWaitTime(unsigned int wait_time)
{
    rn2903_set_response_wait_time(m_rn2903, wait_time);
}

void RN2903::setResponse2WaitTime(unsigned int wait_time)
{
    rn2903_set_response2_wait_time(m_rn2903, wait_time);
}

bool RN2903::dataAvailable(unsigned int millis)
{
    return rn2903_data_available(m_rn2903, millis);
}

void RN2903::drain()
{
    rn2903_drain(m_rn2903);
    return;
}

RN2903_RESPONSE_T RN2903::command(const std::string cmd)
{
    return rn2903_command(m_rn2903, cmd.c_str());
}

RN2903_RESPONSE_T RN2903::commandWithArg(const std::string cmd,
                                         const std::string arg)

{
    return rn2903_command_with_arg(m_rn2903, cmd.c_str(), arg.c_str());
}

RN2903_RESPONSE_T RN2903::waitForResponse(int wait_ms)
{
    return rn2903_waitfor_response(m_rn2903, wait_ms);
}

std::string RN2903::getResponse()
{
    return string(rn2903_get_response(m_rn2903),
                  rn2903_get_response_len(m_rn2903));
}

int RN2903::getResponseLen()
{
    return rn2903_get_response_len(m_rn2903);
}

void RN2903::setDeviceEUI(const std::string str)
{
    if (rn2903_set_device_eui(m_rn2903, str.c_str()))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_set_device_eui() failed");
}

void RN2903::getDeviceEUI()
{
    if (rn2903_get_device_eui(m_rn2903))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_get_device_eui() failed");
}

void RN2903::setNetworkSessionKey(const std::string str)
{
    if (rn2903_set_network_session_key(m_rn2903, str.c_str()))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_set_network_session_key() failed");
}

void RN2903::setApplicationSessionKey(const std::string str)
{
    if (rn2903_set_application_session_key(m_rn2903, str.c_str()))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_set_application_session_key() failed");
}

void RN2903::setApplicationEUI(const std::string str)
{
    if (rn2903_set_application_eui(m_rn2903, str.c_str()))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_set_application_eui() failed");
}

void RN2903::getApplicationEUI()
{
    if (rn2903_get_application_eui(m_rn2903))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_get_application_eui() failed");
}

void RN2903::setApplicationKey(const std::string str)
{
    if (rn2903_set_application_key(m_rn2903, str.c_str()))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_set_application_key() failed");
}

void RN2903::getApplicationKey()
{
    if (rn2903_get_application_key(m_rn2903))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_get_application_key() failed");
}

void RN2903::setDeviceAddr(const std::string str)
{
    if (rn2903_set_device_addr(m_rn2903, str.c_str()))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_set_device_addr() failed");
}

void RN2903::getDeviceAddr()
{
   if (rn2903_get_device_addr(m_rn2903))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_get_device_addr() failed");
}

std::string RN2903::toHex(const std::string src)
{
    const char *buf = rn2903_to_hex(m_rn2903, src.c_str(), src.size());

    if (!buf)
        return string("");
    else
        return string(buf);
}

std::string RN2903::fromHex(const std::string src)
{
    const char *buf = rn2903_from_hex(m_rn2903, src.c_str());

    if (!buf)
        return string("");
    else
        return string(buf);
}

RN2903_JOIN_STATUS_T RN2903::join(RN2903_JOIN_TYPE_T type)
{
    return rn2903_join(m_rn2903, type);
}

RN2903_MAC_TX_STATUS_T RN2903::macTx(RN2903_MAC_MSG_TYPE_T type, int port,
                                     std::string payload)
{
    return rn2903_mac_tx(m_rn2903, type, port, payload.c_str());
}

RN2903_RESPONSE_T RN2903::radioTx(const std::string payload)
{
    return rn2903_radio_tx(m_rn2903, payload.c_str());
}

RN2903_RESPONSE_T RN2903::radioRx(int window_size)
{
    return rn2903_radio_rx(m_rn2903, window_size);
}

std::string RN2903::getHardwareEUI()
{
    return string(rn2903_get_hardware_eui(m_rn2903));
}

void RN2903::updateMacStatus()
{
    if (rn2903_update_mac_status(m_rn2903))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_update_mac_status() failed");
}

int RN2903::getMacStatusWord()
{
    return int(rn2903_get_mac_status_word(m_rn2903));
}

RN2903_MAC_STATUS_T RN2903::getMacStatus()
{
    return rn2903_get_mac_status(m_rn2903);
}

void RN2903::macSave()
{
    if (rn2903_mac_save(m_rn2903))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_mac_save() failed");
}

void RN2903::macPause()
{
    if (rn2903_mac_pause(m_rn2903))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_mac_pause() failed");
}

void RN2903::macResume()
{
    if (rn2903_mac_resume(m_rn2903))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_mac_resume() failed");
}

void RN2903::reset()
{
    if (rn2903_reset(m_rn2903))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_reset() failed");
}

void RN2903::macSetBattery(int level)
{
    if (rn2903_mac_set_battery(m_rn2903, level))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_mac_set_battery() failed");
}

void RN2903::setDebug(bool enable)
{
    rn2903_set_debug(m_rn2903, enable);
}

void RN2903::setBaudrate(unsigned int baudrate)
{
    if (rn2903_set_baudrate(m_rn2903, baudrate))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_set_baudrate() failed");
}

void RN2903::setFlowControl(RN2903_FLOW_CONTROL_T fc)
{
    if (rn2903_set_flow_control(m_rn2903, fc))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": rn2903_set_flow_control() failed");
}

bool RN2903::find(const std::string str)
{
    return rn2903_find(m_rn2903, str.c_str());
}

std::string RN2903::getRadioRxPayload()
{
    const char *payload = rn2903_get_radio_rx_payload(m_rn2903);

    if (!payload)
        return string("");
    else
        return string(payload);
}

bool RN2903::autobaud(int retries)
{
    return rn2903_autobaud(m_rn2903, retries);
}
