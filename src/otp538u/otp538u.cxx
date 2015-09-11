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
#include <string>
#include <stdexcept>

#include "otp538u.h"

#include "thermopile_vt_table.h"
#include "thermister_rt_table.h"

using namespace upm;
using namespace std;

OTP538U::OTP538U(int pinA, int pinO, float aref)
{
  // this is the internal voltage reference on the Grove IR temp
  // sensor module
  m_vref = 2.5;

  // analog reference in use
  m_aref = aref;

  // This is the value of the output resistor of the Grove IR
  // temp sensor's SIG2 output (ambient)
  m_vResistance = 2000000;      // 2M ohms

  // This was the default offset voltage in the seeedstudio code.  You
  // can adjust as neccessary depending on your calibration.
  m_offsetVoltage = 0.014;

  // We need around 1mV resolution, so use 12 bit resolution (4096)
  // with a default aref of 5.0.
  m_adcResolution = 4096;

  if ( !(m_aioA = mraa_aio_init(pinA)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(pinA) failed, invalid pin?");
      return;
    }

  // enable 12 bit resolution
  mraa_aio_set_bit(m_aioA, 12);

  if ( !(m_aioO = mraa_aio_init(pinO)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(pinO) failed, invalid pin?");
      return;
    }

  // enable 12 bit resolution
  mraa_aio_set_bit(m_aioO, 12);
}

OTP538U::~OTP538U()
{
  mraa_aio_close(m_aioA);
  mraa_aio_close(m_aioO);
}

float OTP538U::ambientTemperature()
{
  const int samples = 5;
  int val = 0;
  float temp = 0;
  float res;

  for (int i=0; i<samples; i++)
    {
      val = mraa_aio_read(m_aioA);
      temp += val;
      usleep(10000);
    }

  temp = temp / samples;
  temp = temp * m_aref / m_adcResolution;

  // compute the resistance of the thermistor
  res = m_vResistance * temp / (m_vref - temp);

  // look it up in the thermistor (RT) resistence/temperature table
  int rawslot;
  int j;
  for (j=0; j<otp538u_rt_table_max; j++)
    if (otp538u_rt_table[j] < res)
      {
        rawslot = j;
        break;
      }

  if (j >= otp538u_rt_table_max)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": ambient temperature out of range.");
      return 0;
    }

  // we need to compensate for the fact that we are supporting
  // temperature values less than 0 (-20C), so adjust correspondingly
  // so that we obtain the correct temperature 'slot'.  This will be
  // our base temperature.
  int slot = rawslot - 20;
  
  // too cold
  if (slot < 0)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": ambient temperature out of range.");
      return 0;
    }

  // now compute the ambient temperature
  float ambientTemp = slot - 1 +
    (otp538u_rt_table[rawslot - 1]-res) / (otp538u_rt_table[rawslot - 1] - 
                                           otp538u_rt_table[rawslot]);
  
  return ambientTemp;
}

float OTP538U::objectTemperature()
{
  const int samples = 5;
  const float reference_vol= 0.5; // what is this value? (from seeedstudio)
  const float tempIncrement=10;
  int val = 0;
  float temp = 0;
  float ambTemp = ambientTemperature();
  
  for (int i=0; i<samples; i++)
    {
      val = mraa_aio_read(m_aioO);
      temp += val;
      usleep(10000);
    }

  temp = temp / samples;

  float temp1 = temp * m_aref / m_adcResolution;
  float sensorVolts = temp1 - (reference_vol + m_offsetVoltage);
  // cout << "Sensor Voltage: " << sensorVolts << endl;

  // search the VT (voltage/temperature) table to find the object
  // temperature.
  int slot;
  // add +2 to compensate for the -20C and -10C slots below zero
  int voltOffset = int(ambTemp / 10) + 1 + 2;
  float voltage = sensorVolts * 10.0;
  for (slot=0; slot<(otp538u_vt_table_max - 1); slot++)		
    {
      if ( (voltage > otp538u_vt_table[slot][voltOffset]) &&
           (voltage < otp538u_vt_table[slot+1][voltOffset]) )
        {
          break;
        }
    }

  if (slot >= (otp538u_vt_table_max - 1))
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": object temperature out of range.");
      return 0;
    }

  float objTemp = (float(tempIncrement) * voltage) /
    ( otp538u_vt_table[slot + 1][voltOffset] - 
      otp538u_vt_table[slot][voltOffset] );    

  //  cout << "TABLE VALUE [" << slot << "][" <<
  //    voltOffset << "] = " << otp538u_vt_table[slot][voltOffset] << endl;

  return (ambTemp + objTemp);
}
