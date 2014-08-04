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

#include "max31723.h"

using namespace upm;

struct MAX31723Exception : public std::exception {
    std::string message;
    MAX31723Exception (std::string msg) : message (msg) { }
    ~MAX31723Exception () throw () { }
    const char* what() const throw () { return message.c_str(); }
};

MAX31723::MAX31723 (int csn) {
    mraa_result_t error = MRAA_SUCCESS;
    m_name = "MAX31723";

    m_csnPinCtx = mraa_gpio_init (csn);
    if (m_csnPinCtx == NULL) {
        throw MAX31723Exception ("GPIO failed to initilize");
    }

    error = mraa_gpio_dir (m_csnPinCtx, MRAA_GPIO_OUT);
    if (error != MRAA_SUCCESS) {
        throw MAX31723Exception ("GPIO failed to initilize");
    }

    m_spi = mraa_spi_init (0);
    if (m_spi == NULL) {
        throw MAX31723Exception ("SPI failed to initilize");
    }

    CSOff ();
}

MAX31723::~MAX31723() {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_spi_stop(m_spi);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
    error = mraa_gpio_close (m_csnPinCtx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
}

uint16_t
MAX31723::getTemperature () {
    uint8_t lsb = 0;
    uint8_t msb = 0;
    uint8_t buf[2] = { 0x01, 0x00};

    CSOn ();

    uint8_t* x = mraa_spi_write_buf(m_spi, buf, 2);

    printf ("%d\n", (uint16_t)*x);

    /*mraa_spi_write (m_spi, R_TEMPERATURE_LSB);
    lsb = mraa_spi_write (m_spi, lsb);
    lsb >>= 4;*/

    /*mraa_spi_write (m_spi, R_TEMPERATURE_MSB);
    msb = mraa_spi_write (m_spi, msb);

    if ((msb & 0x80) != 0)
        msb |= ~((1 << 8) - 1);*/

    CSOff ();

    return msb;
}

/*
 * **************
 *  private area
 * **************
 */

mraa_result_t
MAX31723::CSOn () {
    return mraa_gpio_write (m_csnPinCtx, HIGH);
}

mraa_result_t
MAX31723::CSOff () {
    return mraa_gpio_write (m_csnPinCtx, LOW);
}
