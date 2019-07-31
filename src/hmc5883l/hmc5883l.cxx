/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * Ported based on original C++ code by:
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <string>
#include <stdexcept>

#include "hmc5883l.hpp"


using namespace upm;
using namespace std;

Hmc5883l::Hmc5883l(int bus) :
    m_hmc5883l(hmc5883l_init(bus))
{
    if (!m_hmc5883l)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": hmc5883l_init() failed");
}

upm_result_t Hmc5883l::update(void)
{
    if (hmc5883l_update(m_hmc5883l))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": hmc5883l_update() failed");

    // kind of pointless, but want to maintain some compatibility with
    // original code.
    return UPM_SUCCESS;
}

float Hmc5883l::direction(void)
{
    return hmc5883l_direction(m_hmc5883l);
}

float Hmc5883l::heading(void)
{
    return hmc5883l_heading(m_hmc5883l);
}

const int16_t *Hmc5883l::coordinates(void)
{
    return hmc5883l_coordinates(m_hmc5883l);
}

void Hmc5883l::set_declination(float dec)
{
    return hmc5883l_set_declination(m_hmc5883l, dec);
}

float Hmc5883l::get_declination()
{
    return hmc5883l_get_declination(m_hmc5883l);
}
