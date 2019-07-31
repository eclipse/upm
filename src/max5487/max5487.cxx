/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdexcept>

#include "max5487.hpp"

using namespace upm;

MAX5487::MAX5487 (int csn) :m_spi(0), m_csnPinCtx(csn) {
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

    m_spi.write(data, 2);

    CSOff ();
}

void
MAX5487::setWiperB (uint8_t wiper) {
    uint8_t data[2] = { 0x00, 0x00};

    CSOn ();

    data[0] = R_WR_WIPER_B;
    data[1] = wiper;

    m_spi.write(data, 2);

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
