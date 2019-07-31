/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
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

#include "rpr220.hpp"

using namespace upm;
using namespace std;

RPR220::RPR220(int pin) :
    m_rpr220(rpr220_init(pin))
{
    if (!m_rpr220)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": rpr220_init() failed");
}

RPR220::~RPR220()
{
    rpr220_close(m_rpr220);
}

bool RPR220::blackDetected()
{
    return rpr220_black_detected(m_rpr220);
}



void RPR220::installISR(void (*isr)(void *), void *arg)
{
    rpr220_install_isr(m_rpr220, isr, arg);
}

void RPR220::uninstallISR()
{
    rpr220_uninstall_isr(m_rpr220);
}
