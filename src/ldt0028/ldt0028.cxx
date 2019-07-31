/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
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

#include "ldt0028.hpp"

using namespace upm;

LDT0028::LDT0028(unsigned int pin) {
    // initialize analog input
    if ( !(m_pin = mraa_aio_init(pin)) )
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_aio_init() failed, invalid pin?");
        return;
      }
    m_name = "ldt0-028";
}

LDT0028::LDT0028(std::string initStr) : mraaIo(initStr) {
  mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();

  if(!descs->aios)
  {
    throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
  }
  else
  {
    if( !(m_pin = descs->aios[0]) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                              ": mraa_aio_init() failed, invalid pin?");

    }
  }
  m_name = "ldt0-028";
}

LDT0028::~LDT0028() {
    // close analog input
    mraa_aio_close(m_pin);
}

std::string LDT0028::name() {
    return m_name;
}

int LDT0028::getSample() {
    return mraa_aio_read(m_pin);
}
