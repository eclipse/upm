/*
 * Author: Zion Orent <sorent@ics.com>
 *         Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
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

#include "ppd42ns.hpp"

using namespace upm;

PPD42NS::PPD42NS(int pin) :
    m_ppd42ns(ppd42ns_init(pin))
{
    if (!m_ppd42ns)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                    ": ppd42ns_init() failed");
}

PPD42NS::~PPD42NS()
{
    ppd42ns_close(m_ppd42ns);
}

ppd42ns_dust_data PPD42NS::getData()
{
    return ppd42ns_get_data(m_ppd42ns);
}

