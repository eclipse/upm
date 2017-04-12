/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
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

#include <iostream>
#include <stdexcept>
#include <string.h>

#include "uartat.hpp"

using namespace upm;
using namespace std;

UARTAT::UARTAT(unsigned int uart, unsigned int baudrate) :
    m_uartat(uartat_init(uart, baudrate))
{
    if (!m_uartat)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": uartat_init() failed");
}

UARTAT::UARTAT(string uart_path, unsigned int baudrate) :
    m_uartat(uartat_init_tty(uart_path.c_str(), baudrate))
{
    if (!m_uartat)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": uartat_init_tty() failed");
}

UARTAT::~UARTAT()
{
    uartat_close(m_uartat);
}

std::string UARTAT::readStr(size_t size)
{
    char buffer[size];

    int rv;

    if ((rv = uartat_read(m_uartat, buffer, size)) < 0)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": uartat_read() failed");

    return string(buffer, rv);
}

int UARTAT::writeStr(std::string buffer)
{
    int rv;

    if ((rv = uartat_write(m_uartat, (char*)buffer.data(),
                           buffer.size())) < 0)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": uartat_write() failed");

    return rv;
}

void UARTAT::setBaudrate(unsigned int baudrate)
{
    if (uartat_set_baudrate(m_uartat, baudrate))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": uartat_baudrate() failed");
}

void UARTAT::setResponseWaitTime(unsigned int wait_time)
{
    uartat_set_response_wait_time(m_uartat, wait_time);
}

bool UARTAT::dataAvailable(unsigned int millis)
{
    return uartat_data_available(m_uartat, millis);
}

bool UARTAT::commandMode(std::string cmd_chars, unsigned int guard_ms)
{
    return uartat_command_mode(m_uartat, cmd_chars.c_str(), guard_ms);
}

bool UARTAT::inCommandMode()
{
    return uartat_in_command_mode(m_uartat);
}

void UARTAT::drain()
{
    uartat_drain(m_uartat);
    return;
}

string UARTAT::commandWithResponse(const string cmd, size_t resp_len)
{
    char buffer[resp_len];

    int rv;

    if ((rv = uartat_command_with_response(m_uartat, cmd.c_str(), buffer,
                                           resp_len)) < 0)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": uartat_command_with_response() failed");

    return string(buffer, rv);
}

string UARTAT::commandWaitFor(const std::string cmd, size_t resp_len,
                              const std::string waitString,
                              unsigned int millis)
{
    char buffer[resp_len];

    if (uartat_command_waitfor(m_uartat, cmd.c_str(), buffer, resp_len,
                               waitString.c_str(), millis))
        return string(buffer, strlen(buffer));
    else
        return string("");
}

void UARTAT::command(const string cmd)
{
    uartat_command(m_uartat, cmd.c_str());

    return;
}

string UARTAT::stringCR2LF(string str)
{
  for (size_t i=0; i<str.size(); i++)
    if (str[i] == '\r')
      str[i] = '\n';

  return str;
}

void UARTAT::setFlowControl(UARTAT_FLOW_CONTROL_T fc)
{
    if (uartat_set_flow_control(m_uartat, fc))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": uartat_set_flow_control() failed");

    return;
}

bool UARTAT::find(const std::string buffer, const std::string str)
{
    return uartat_find(m_uartat, buffer.c_str(), str.c_str());
}

void UARTAT::filterCR(bool enable)
{
    uartat_filter_cr(m_uartat, enable);
}
