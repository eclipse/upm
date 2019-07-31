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

#include "upm_string_parser.hpp"
#include "bh1750.hpp"

using namespace upm;
using namespace std;

BH1750::BH1750(int bus, int addr, BH1750_OPMODES_T mode) :
  m_bh1750(bh1750_init(bus, addr, mode))
{
  if (!m_bh1750)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": bh1750_init() failed");
}

BH1750::BH1750(std::string initStr) : mraaIo(initStr)
{
  mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
  std::vector<std::string> upmTokens;

  if(!mraaIo.getLeftoverStr().empty()) {
    upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
  }

  // make sure MRAA is initialized
  int mraa_rv;
  if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
  {
    throw std::runtime_error(std::string(__FUNCTION__) +
                             ": mraa_init() failed");
  }

  m_bh1750 = (bh1750_context)malloc(sizeof(struct _bh1750_context));
  if(!m_bh1750)
    throw std::runtime_error(std::string(__FUNCTION__) +
                             ": bh1750_init() failed");

  m_bh1750->is_continuous = false;
  // init the i2c context
  if(!descs->i2cs)
  {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_i2c_init() failed");
  }
  else
  {
      if( !(m_bh1750->i2c = descs->i2cs[0]) )
      {
        free(m_bh1750);
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mraa_i2c_init() failed");
      }
  }

  for (std::string tok : upmTokens)
  {
    if(tok.substr(0, 5) == "mode:") {
      BH1750_OPMODES_T mode = (BH1750_OPMODES_T)std::stoi(tok.substr(5), nullptr, 0);
        if(bh1750_set_opmode(m_bh1750, mode) != UPM_SUCCESS)
        {
          bh1750_close(m_bh1750);
          throw std::runtime_error(std::string(__FUNCTION__) +
                                   ": bh1750_init() failed");
        }
    }
    if(tok.substr(0, 8) == "powerUp:") {
      powerUp();
    }
    if(tok.substr(0, 10) == "powerDown:") {
      powerDown();
    }
    if(tok.substr(0, 12) == "sendCommand:") {
      uint8_t  mode = (uint8_t)std::stoul(tok.substr(12), nullptr, 0);
      sendCommand(mode);
    }
  }
}

BH1750::~BH1750()
{
  bh1750_close(m_bh1750);
}

void BH1750::reset()
{
  bh1750_reset(m_bh1750);
}

float BH1750::getLux()
{
  float lux = 0.0;

  if (bh1750_get_lux(m_bh1750, &lux) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": bh1750_get_lux() failed");

  return lux;
}

float BH1750::getLuminance()
{
    return getLux();
}

void BH1750::powerUp()
{
  if (bh1750_power_up(m_bh1750) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": bh1750_power_up() failed");
}

void BH1750::powerDown()
{
  if (bh1750_power_down(m_bh1750) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": bh1750_power_down() failed");
}

void BH1750::setOpmode(BH1750_OPMODES_T mode)
{
  if (bh1750_set_opmode(m_bh1750, mode) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": bh1750_set_opmode() failed");
}

void BH1750::sendCommand(uint8_t cmd)
{
  if (bh1750_send_command(m_bh1750, cmd) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": bh1750_send_command() failed");
}

uint16_t BH1750::readData()
{
  uint16_t data = 0;

  if (bh1750_read_data(m_bh1750, &data) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": bh1750_read_data() failed");

  return data;
}
