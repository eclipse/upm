/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <typeinfo>

#include "upm_string_parser.hpp"
#include "ad8232.hpp"

using namespace upm;
using namespace std;


AD8232::AD8232(int loPlus, int loMinus, int output, float aref) : 
  m_gpioLOPlus(loPlus), m_gpioLOMinus(loMinus), m_aioOUT(output)
{
  m_gpioLOPlus.dir(mraa::DIR_IN);
  m_gpioLOMinus.dir(mraa::DIR_IN);

  m_aref = aref;
  m_ares = (1 << m_aioOUT.getBit());
}

AD8232::AD8232(std::string initStr) : 
  m_gpioLOPlus(nullptr), m_gpioLOMinus(nullptr), m_aioOUT(nullptr), mraaIo(initStr) {
    
  mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
  std::vector<std::string> upmTokens;
  
  if(!mraaIo.getLeftoverStr().empty()) {
    upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
  }

  if(!descs->gpios) {
    throw std::invalid_argument(std::string(__FUNCTION__) +
                          ": mraa_gpio_init() must initialize two pins");
  } else {
    if(descs->gpios[0])
      m_gpioLOPlus = descs->gpios[0];
    if(descs->gpios[1])
      m_gpioLOMinus = descs->gpios[1];
  }
  
  if(!descs->aios){
    throw std::invalid_argument(std::string(__FUNCTION__) +
                            ": mraa_aio_init() failed, invalid pin?");    
  } else {
    m_aioOUT = descs->aios[0];
  }
  
  for (std::string tok : upmTokens) {
    if(tok.substr(0, 5) == "aref:") {
      m_aref = std::stof(tok.substr(5));
    }
  }
  m_ares = (1 << m_aioOUT.getBit());
}

AD8232::~AD8232()
{
}

int AD8232::value()
{
  if (m_gpioLOPlus.read() || m_gpioLOMinus.read())
    return 0;
  else
    return m_aioOUT.read();
}
