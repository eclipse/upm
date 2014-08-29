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

#include "max5487.h"

using namespace upm;

struct MAX5487Exception : public std::exception {
    std::string message;
    MAX5487Exception (std::string msg) : message (msg) { }
    ~MAX5487Exception () throw () { }
    const char* what() const throw () { return message.c_str(); }
};

MAX5487::MAX5487 (int csn = -1) {
    mraa_result_t error = MRAA_SUCCESS;
    m_name = "MAX5487";

    m_csnPinCtx = NULL;
    if (csn != -1) {
        m_csnPinCtx = mraa_gpio_init (csn);
        if (m_csnPinCtx == NULL) {
            throw MAX5487Exception ("GPIO failed to initilize");
        }

        error = mraa_gpio_dir (m_csnPinCtx, MRAA_GPIO_OUT);
        if (error != MRAA_SUCCESS) {
            throw MAX5487Exception ("GPIO failed to initilize");
        }
    }

    m_spi = mraa_spi_init (0);
    if (m_spi == NULL) {
        throw MAX5487Exception ("SPI failed to initilize");
    }

    CSOff ();
}

MAX5487::~MAX5487() {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_spi_stop(m_spi);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
    if (m_csnPinCtx != NULL) {
        error = mraa_gpio_close (m_csnPinCtx);
        if (error != MRAA_SUCCESS) {
            mraa_result_print(error);
        }
    }
}

void
MAX5487::setWiperA (uint8_t wiper) {
    uint8_t data[2] = { 0x00, 0x00};

    CSOn ();

    data[0] = R_WR_WIPER_A;
    data[1] = wiper;

    uint8_t* retData = mraa_spi_write_buf(m_spi, data, 2);

    CSOff ();
}

void
MAX5487::setWiperB (uint8_t wiper) {
    uint8_t data[2] = { 0x00, 0x00};

    CSOn ();

    data[0] = R_WR_WIPER_B;
    data[1] = wiper;

    uint8_t* retData = mraa_spi_write_buf(m_spi, data, 2);

    CSOff ();
}

/*
 * **************
 *  private area
 * **************
 */

mraa_result_t
MAX5487::CSOn () {
    if (m_csnPinCtx != NULL)
        return mraa_gpio_write (m_csnPinCtx, LOW);
    return MRAA_SUCCESS;
}

mraa_result_t
MAX5487::CSOff () {
    if (m_csnPinCtx != NULL)
        return mraa_gpio_write (m_csnPinCtx, HIGH);
    return MRAA_SUCCESS;
}
