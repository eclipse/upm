/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
