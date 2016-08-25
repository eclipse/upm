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

#include "otp538u.hpp"

#include "thermopile_vt_table.hpp"
#include "thermister_rt_table.hpp"

using namespace upm;
using namespace std;

OTP538U::OTP538U(int pinA, int pinO, float aref)
{
  const int adcHighRes = 4095;
  const int adcLowRes = 1023;

  // for subplatforms like the Arduino 101, we need to limit ADC
  // resolution to 10b currently.  For this sensor unfortunately, this
  // means readings will be less accurate.  This sensor really does
  // need to measure with about 1mV accuracy.
  bool isSubplatform = false;

  m_debug = false;

  if (pinA >= 512 || pinO >= 512)
    isSubplatform = true;

  // this is the internal voltage reference on the Grove IR temp
  // sensor module for the thermistor.

  m_internalVRef = 2.5;

  // analog reference in use
  m_aref = aref;

  // This is the value of the output resistor of the Grove IR
  // temp sensor's SIG2 output (ambient)
  m_vResistance = 2000000;      // 2M ohms

  // This was the default offset voltage in the seeedstudio code.  You
  // can adjust as neccessary depending on your calibration.
  m_offsetVoltage = 0.014;

  // We need around 1mV resolution (preferred), so use 12 bit
  // resolution (4096) if we can.
  //
  // This logic is over complicated due to the fact that it is
  // currently difficult to determine exactly what the capabilities of
  // the platform (sub or otherwise) actually are.  So for
  // subplatforms, we always limit to 1024.  Otherwise, we try 12b if
  // the mraa_adc_raw_bits() says we can, though this isn't
  // particularly accurate either, as it reports that the G2 can do
  // 12b, when in reality it can not.  We are just lucky that it works
  // anyway (ie: will give 12b resolution, though underneath it's just
  // scaling the real 10b value.).  Sigh.  But trying 12b resolution
  // on the 101 via firmata will definitely break things, so don't
  // even try until whatever the problem it has with 12b is fixed.
  if (isSubplatform)
    {
      m_adcResolution = adcLowRes; // 10b
    }
  else
    {
      if (mraa_adc_raw_bits() == 12)
        m_adcResolution = adcHighRes; // 12b
      else
        m_adcResolution = adcLowRes; // 10b
    }

  // notify the user
  if (m_adcResolution == adcLowRes)
    cerr << "Using 10 bit ADC resolution.  Values will be less accurate."
         << endl;


  if ( !(m_aioA = mraa_aio_init(pinA)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(pinA) failed");
      return;
    }

  // enable 12 bit resolution, if we can
  if (m_adcResolution == adcHighRes)
    mraa_aio_set_bit(m_aioA, 12);

  if ( !(m_aioO = mraa_aio_init(pinO)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(pinO) failed");
      return;
    }


  // enable 12 bit resolution, if we can
  if (m_adcResolution == adcHighRes)
    mraa_aio_set_bit(m_aioO, 12);

  if (isSubplatform)
    {
      // The first analog read always seems to return 0 on the 101
      // with firmata, so just do a couple of reads here and discard
      // them.  Then sleep for half a second.  THIS IS A HACK.  The
      // real problem should be fixed elsewhere (Firmata?).
      mraa_aio_read(m_aioA);
      mraa_aio_read(m_aioO);

      usleep(500000);
    }
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
  float res = 0;

  for (int i=0; i<samples; i++)
    {
      val = mraa_aio_read(m_aioA);
      if (val == -1) {
          throw std::runtime_error(std::string(__FUNCTION__) +
                                   ": failed to do aio read");
      }
      temp += (float)val;
      usleep(10000);
    }
  temp = temp / samples;

  float volts = temp * m_aref / m_adcResolution;
  if (m_debug)
    {
      cerr << "\tAMB sample " << temp << " m_aref " << m_aref
           << " VOLTS " << volts << endl;
    }

  // compute the resistance of the thermistor
  res = m_vResistance * volts / (m_internalVRef - volts);

  if (m_debug)
    {
      cerr << "\tAMB computed resistance: " << res << endl;
    }

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
                              ": ambient temperature out of range (high).");
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
                              ": ambient temperature out of range (low).");
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
      if (val == -1) {
          throw std::runtime_error(std::string(__FUNCTION__) +
                                               ": failed to do aio read.");
          return 0;
      }
      temp += val;
      usleep(10000);
    }

  temp = temp / samples;

  if (m_debug)
    cerr << "\tOBJ sample " << temp << " ";

  float volts = temp * m_aref / m_adcResolution;

  if (m_debug)
    cerr << "VOLTS: " << volts << " ";

  float sensorVolts = volts - (reference_vol + m_offsetVoltage);

  if (m_debug)
    cerr << "Sensor Voltage (computed): " << sensorVolts << endl;

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

  if (m_debug)
    {
      cerr << "\tVoltage (" << voltage << "): TABLE VALUE [" << slot << "][" <<
        voltOffset << "] = " << otp538u_vt_table[slot][voltOffset] << endl;
    }

  return (ambTemp + objTemp);
}
