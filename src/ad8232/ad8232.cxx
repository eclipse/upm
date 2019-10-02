/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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
