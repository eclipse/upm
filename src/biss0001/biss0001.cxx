/*
 * Author: Zion Orent <sorent@ics.com>
 *         Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
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

#include "upm_string_parser.hpp"
#include "biss0001.hpp"

using namespace upm;

BISS0001::BISS0001(unsigned int pin) :
    m_biss0001(biss0001_init(pin))
{
    if (!m_biss0001)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": biss0001_init() failed");
}

BISS0001::BISS0001(std::string initStr) : mraaIo(initStr)
{
    mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();

    m_biss0001 = (biss0001_context)malloc(sizeof(struct _biss0001_context));

    if(!m_biss0001)
          throw std::runtime_error(std::string(__FUNCTION__) +
                                   ": biss0001_init() failed");

    m_biss0001->gpio = NULL;

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS) {
        biss0001_close(m_biss0001);
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": biss0001_init() failed");
    }

    // initialize the MRAA context
    if(!descs->gpios) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mraa_gpio_init() failed");
    }
    else {
        if( !(m_biss0001->gpio = descs->gpios[0]) ) {
          biss0001_close(m_biss0001);
          throw std::runtime_error(std::string(__FUNCTION__) +
                                   ": mraa_gpio_init() failed");
        }
    }
    mraa_gpio_dir(m_biss0001->gpio, MRAA_GPIO_IN);
}

BISS0001::~BISS0001()
{
    biss0001_close(m_biss0001);
}

bool BISS0001::value()
{
    return biss0001_motion_detected(m_biss0001);
}
