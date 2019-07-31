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

#include "cwlsxxa.hpp"
#include "upm_string_parser.hpp"

using namespace upm;
using namespace std;

// for current loop reads that seems a little noisy, we average over
// several aio reads.
static const int maxSamples = 50;

// conversion from celsius to fahrenheit

static float c2f(float c)
{
  return (c * (9.0 / 5.0) + 32.0);
}

CWLSXXA::CWLSXXA(int gPin, int hPin, int tPin, float rResistor, float aref) :
  m_aioCO2(0), m_aioHum(0), m_aioTemp(0)
{
  m_aioCO2 = new mraa::Aio(gPin);

  m_hasHum = (hPin >= 0) ? true : false;
  m_hasTemp = (tPin >= 0) ? true : false;

  if (m_hasTemp)
    {
      m_aioTemp = new mraa::Aio(tPin);
      m_aResTemp = (1 << m_aioTemp->getBit());
    }
  else
    m_aResTemp = 0;

  if (m_hasHum)
    {
      m_aioHum = new mraa::Aio(hPin);
      m_aResHum = (1 << m_aioHum->getBit());
    }
  else
    m_aResHum = 0;

  m_aResCO2 = (1 << m_aioCO2->getBit());

  m_temperature = 0.0;
  m_humidity = 0.0;
  m_co2 = 0.0;

  m_aref = aref;
  m_rResistor = rResistor;
}

CWLSXXA::CWLSXXA(std::string initStr)
{
  mraaIo = new mraa::MraaIo(initStr);
  if(mraaIo == NULL)
  {
    throw std::invalid_argument(std::string(__FUNCTION__) +
                            ": Failed to allocate memory for internal member");
  }

  if(!mraaIo->aios.empty())
  {
    m_aioCO2 = &mraaIo->aios[0];

    if(mraaIo->aios.size() > 1)
    {
      m_hasTemp = 1;
      m_aioTemp = &mraaIo->aios[1];
      m_aResTemp = (1 << m_aioTemp->getBit());

      m_hasHum = 1;
      m_aioHum = &mraaIo->aios[2];
      m_aResHum = (1 << m_aioHum->getBit());
    }
  }
  else
  {
    m_hasTemp  = 0;
    m_aResTemp = 0;
    m_hasHum   = 0;
    m_aResHum  = 0;

    throw std::invalid_argument(std::string(__FUNCTION__) +
                            ": mraa_aio_init() failed, invalid pin?");
  }

  m_aResCO2 = (1 << m_aioCO2->getBit());

  m_temperature = 0.0;
  m_humidity = 0.0;
  m_co2 = 0.0;

  std::vector<std::string> upmTokens;

  if(!mraaIo->getLeftoverStr().empty())
  {
      upmTokens = UpmStringParser::parse(mraaIo->getLeftoverStr());
  }

  for(std::string tok : upmTokens)
  {
    if(tok.substr(0, 5) == "aref:")
    {
      float aref = std::stof(tok.substr(5));
      m_aref = aref;
    }
    if(tok.substr(0, 10) == "rResistor:")
    {
      float rResistor = std::stof(tok.substr(10));
      m_rResistor = rResistor;
    }

  }
}

CWLSXXA::~CWLSXXA()
{
  if(mraaIo)
    delete mraaIo;
  else
  {
    delete m_aioCO2;

    if (m_aioHum)
      delete m_aioHum;
    if (m_aioTemp)
      delete m_aioTemp;
  }
}

void CWLSXXA::update()
{
  // temperature
  int val;
  float volts;
  float milliamps = 0.0;

  int samples;
  if (m_rResistor)
    samples = maxSamples;
  else
    samples = 1;


  // temperature
  if (m_hasTemp)
    {
      // fortunately, this sensor always reports temperatures in C,
      // regardless of the configuration of the LCD display (for
      // models that have an LCD display)

      val = average(m_aioTemp, samples);

      volts = (float(val) * (m_aref / m_aResTemp));

      // valid range is 10-35C, current loop range is 16ma (20ma - 4ma)
      if (!m_rResistor)
        m_temperature = ((volts / m_aref) * 25.0) + 10.0;
      else
        {
          milliamps = ((volts / m_rResistor * 1000.0) - 4.0);
          if (milliamps < 0.0) // not connected
            milliamps = 0.0;
          m_temperature = (milliamps * (25.0 / 16.0)) + 10.0;
        }
    }

  // humidity
  if (m_hasHum)
    {
      val = average(m_aioHum, samples);
      volts = (float(val) * (m_aref / m_aResHum));

      // range is 0-100
      if (!m_rResistor)
        m_humidity = ((volts / m_aref) * 100.0);
      else
        {
          milliamps = ((volts / m_rResistor * 1000.0) - 4.0);
          if (milliamps < 0.0) // not connected
            milliamps = 0.0;
          m_humidity = milliamps * (100.0 / 16.0);
        }
    }

  // CO2
  val = average(m_aioCO2, samples);
  volts = (float(val) * (m_aref / m_aResCO2));

  // CO2 range is 0-2000ppm
  if (!m_rResistor)
    m_co2 = ((volts / m_aref) * 2000.0);
  else
    {
      milliamps = ((volts / m_rResistor * 1000.0) - 4.0);
      if (milliamps < 0.0) // not connected
        milliamps = 0.0;
      m_co2 = milliamps * (2000.0 / 16.0);
    }
}

float CWLSXXA::getTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_temperature);
  else
    return m_temperature;
}

float CWLSXXA::getTemperature()
{
  update();
  return m_temperature;
}

float CWLSXXA::getHumidity()
{
  update();
  return m_humidity;
}

float CWLSXXA::getCO2()
{
  return m_co2;
}

float CWLSXXA::getConcentration()
{
  update();
  return m_co2;
}

int CWLSXXA::average(mraa::Aio *aio, int samples)
{
  if (samples <= 0)
    samples = 1;

  int avg = 0;
  for (int i=0; i<samples; i++)
    avg += aio->read();

  return (avg / samples);
}
