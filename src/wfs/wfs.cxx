/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2017 Intel Corporation.
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
#include <string>
#include <stdexcept>

#include "wfs.hpp"

using namespace upm;
using namespace std;

WFS::WFS(int pin) :
    m_wfs(wfs_init(pin))
{
    if (!m_wfs)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": wfs_init() failed");
}

WFS::~WFS()
{
    wfs_close(m_wfs);
}

void WFS::initClock()
{
    wfs_init_clock(m_wfs);
}

uint32_t WFS::getMillis()
{
    return wfs_get_millis(m_wfs);
}

void WFS::startFlowCounter()
{
    if (wfs_start_flow_counter(m_wfs))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": wfs_start_flow_counter() failed");
}

void WFS::stopFlowCounter()
{
    wfs_stop_flow_counter(m_wfs);
}

float WFS::flowRate()
{
    return wfs_flow_rate(m_wfs);
}
