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
#include <stdexcept>

#include "dfrph.hpp"
#include "upm_string_parser.hpp"

using namespace upm;

DFRPH::DFRPH(int pin, float vref) : _dev(dfrph_init(pin))
{
    if (_dev == NULL)
        throw std::invalid_argument(std::string(__FUNCTION__) +
                ": dfrph_init() failed, invalid pin?");
}

DFRPH::DFRPH(std::string initStr) : mraaIo(initStr)
{
    mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
    std::vector<std::string> upmTokens;

    if(!mraaIo.getLeftoverStr().empty()) {
        upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
    }
    
    _dev = (dfrph_context) malloc(sizeof(struct _dfrph_context));

    if(!_dev)
        throw std::runtime_error(std::string(__FUNCTION__)
                                + ": dfrph_init() failed");
    
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
      dfrph_close(_dev);
      throw std::runtime_error(std::string(__FUNCTION__) 
                              + ": mraa_init() failed");
    }

    if(!descs->aios)
    {
      dfrph_close(_dev);
      throw std::runtime_error(std::string(__FUNCTION__) 
                              + ": mraa_aio_init() failed");    
    }
    else 
    {
      if( !(_dev->aio = descs->aios[0]) )
      {
        dfrph_close(_dev);
        throw std::runtime_error(std::string(__FUNCTION__) 
                                + ": mraa_aio_init() failed");          
      }
    }
    
    /* Set the ref, offset, and scale */
    _dev->m_aref = 5.0;
    _dev->m_offset = 0.0;
    _dev->m_scale = 1.0; 
    
    for (std::string tok : upmTokens) {
        if(tok.substr(0,10) == "setOffset:") {
          float offset = std::stof(tok.substr(10));
          setOffset(offset);
        }
        if(tok.substr(0,9) == "setScale:") {
          float scale = std::stof(tok.substr(9));
          setScale(scale);
        }
    }    
}

DFRPH::~DFRPH()
{
    dfrph_close(_dev);
}

void DFRPH::setOffset(float offset)
{
    dfrph_set_offset(_dev, offset);
}

void DFRPH::setScale(float scale)
{
    dfrph_set_scale(_dev, scale);
}

float DFRPH::volts()
{
    float volts = 0.0;
    dfrph_get_raw_volts(_dev, &volts);
    return volts;
}

float DFRPH::pH(unsigned int samples)
{
    float ph_avg = 0.0;

    // Read at least 1 sample
    if (samples == 0) samples = 1;

    float ph = 0.0;
    for (unsigned int i = 0; i < samples; i++)
    {
        dfrph_get_ph(_dev, &ph);
        ph_avg += ph;
    }

    return ph_avg/samples;
}

float DFRPH::getPH()
{
    return pH(10);
}
