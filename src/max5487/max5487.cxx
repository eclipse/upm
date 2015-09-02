/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdexcept>

#include "max5487.h"

using namespace upm;

MAX5487::MAX5487 (int csn) : m_csnPinCtx(csn), m_spi(0) {
    mraa::Result error = mraa::SUCCESS;
    m_name = "MAX5487";

    if (csn == -1) {
        throw std::invalid_argument(std::string(__FUNCTION__));
    }

    error = m_csnPinCtx.dir (mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": mraa_gpio_dir() failed");
    }

    CSOff ();
}

void
MAX5487::setWiperA (uint8_t wiper) {
    uint8_t data[2] = { 0x00, 0x00};

    CSOn ();

    data[0] = R_WR_WIPER_A;
    data[1] = wiper;

    uint8_t* retData = m_spi.write(data, 2);

    CSOff ();
}

void
MAX5487::setWiperB (uint8_t wiper) {
    uint8_t data[2] = { 0x00, 0x00};

    CSOn ();

    data[0] = R_WR_WIPER_B;
    data[1] = wiper;

    uint8_t* retData = m_spi.write(data, 2);

    CSOff ();
}

/*
 * **************
 *  private area
 * **************
 */

mraa::Result
MAX5487::CSOn () {
    return m_csnPinCtx.write(LOW);
}

mraa::Result
MAX5487::CSOff () {
    return m_csnPinCtx.write(HIGH);
}
