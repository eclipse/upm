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

bool BISS0001::motionDetected()
{
  return value();
}
