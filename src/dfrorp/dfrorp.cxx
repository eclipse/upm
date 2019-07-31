/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <stdexcept>

#include "dfrorp.hpp"
#include "upm_string_parser.hpp"

using namespace upm;
using namespace std;

DFRORP::DFRORP(int apin, float a_ref) :
  m_dfrorp(dfrorp_init(apin, a_ref))
{
  if (!m_dfrorp)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": dfrorp_init() failed");
}

DFRORP::DFRORP(std::string initStr) : mraaIo(initStr)
{
  mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
  std::vector<std::string> upmTokens;
  
  if(!mraaIo.getLeftoverStr().empty()) {
    upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
  }
  
  m_dfrorp = (dfrorp_context)malloc(sizeof(struct _dfrorp_context));
  
  if (!m_dfrorp)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": dfrorp_init() failed");
  
  memset((void *)m_dfrorp, 0, sizeof(struct _dfrorp_context));
  
  m_dfrorp->aio = NULL;

  m_dfrorp->offset = 0.0;
  m_dfrorp->scale = 1.0;

  // make sure MRAA is initialized
  int mraa_rv;
  if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
  {
    dfrorp_close(m_dfrorp);
    throw std::runtime_error(std::string(__FUNCTION__) 
                             + ": mraa_init() failed");
  }
  
  if(!descs->aios)
  {
    dfrorp_close(m_dfrorp);
    throw std::runtime_error(std::string(__FUNCTION__) 
                             + ": mraa_aio_init() failed");    
  }
  else 
  {
    if( !(m_dfrorp->aio = descs->aios[0]) )
    {
      dfrorp_close(m_dfrorp);
      throw std::runtime_error(std::string(__FUNCTION__) 
                              + ": mraa_aio_init() failed");          
    }
  }
  
  // set our analog resolution
  m_dfrorp->a_res = (float)(1 << mraa_aio_get_bit(m_dfrorp->aio)) - 1;

  for (std::string tok : upmTokens) {
    if(tok.substr(0,6) == "a_ref:")
      m_dfrorp->a_ref = std::stof(tok.substr(6));
    if(tok.substr(0,10) == "setOffset:") {
      float offset = std::stof(tok.substr(10));
      setOffset(offset);
    }
    if(tok.substr(0,9) == "setScale:") {
      float scale = std::stof(tok.substr(9));
      setScale(scale);
    }
    if(tok.substr(0,21) == "setCalibrationOffset:") {
      float offset = std::stof(tok.substr(21));
      setCalibrationOffset(offset);
    }
  }
  
}

DFRORP::~DFRORP()
{
  dfrorp_close(m_dfrorp);
}

void DFRORP::update()
{
  upm_result_t rv;

  if ((rv = dfrorp_update(m_dfrorp)))
    {
      throw std::runtime_error(string(__FUNCTION__)
                               + ": dfrorp_update() failed with UPM error "
                               + to_string(int(rv)) );
    }
}

void DFRORP::setOffset(float offset)
{
  dfrorp_set_offset(m_dfrorp, offset);
}

void DFRORP::setScale(float scale)
{
  dfrorp_set_scale(m_dfrorp, scale);
}

void DFRORP::setCalibrationOffset(float offset)
{
  dfrorp_set_calibration_offset(m_dfrorp, offset);
}

float DFRORP::getORP()
{
  return dfrorp_get_orp(m_dfrorp);
}

float DFRORP::getORPValue()
{
  DFRORP::update();
  return DFRORP::getORP();
}

float DFRORP::getVolts()
{
  return dfrorp_get_volts(m_dfrorp);
}

float DFRORP::getNormalized()
{
  return dfrorp_get_normalized(m_dfrorp);
}
