/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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

#include "vk2828u7.hpp"

using namespace upm;
using namespace std;

VK2828U7::VK2828U7(unsigned int uart, unsigned int baudrate,
                   int enable_pin) :
  m_vk2828u7(vk2828u7_init(uart, baudrate, enable_pin))
{
  if (!m_vk2828u7)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": vk2828u7_init() failed");
}

VK2828U7::~VK2828U7()
{
  vk2828u7_close(m_vk2828u7);
}

std::string VK2828U7::readStr(unsigned int size)
{
  char buffer[size];

  int rv;

  if ((rv = vk2828u7_read(m_vk2828u7, buffer, size)) < 0)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": vk2828u7_read() failed");

  return string(buffer, rv);
}

int VK2828U7::writeStr(std::string buffer)
{
  int rv;

  if ((rv = vk2828u7_write(m_vk2828u7, (char*)buffer.data(),
                           buffer.size())) < 0)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": vk2828u7_write() failed");

  return rv;  
}

void VK2828U7::enable(bool enable)
{
  if (vk2828u7_enable(m_vk2828u7, enable))
    throw std::runtime_error(string(__FUNCTION__)
                             + ": vk2828u7_enable() failed");
}

void VK2828U7::setBaudrate(unsigned int baudrate)
{
  if (vk2828u7_set_baudrate(m_vk2828u7, baudrate))
    throw std::runtime_error(string(__FUNCTION__)
                             + ": vk2828u7_baudrate() failed");
}

bool VK2828U7::dataAvailable(unsigned int millis)
{
  return vk2828u7_data_available(m_vk2828u7, millis);
}
