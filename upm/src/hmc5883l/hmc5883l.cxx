/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * Ported based on original C++ code by:
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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
