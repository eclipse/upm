/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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

#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <math.h>
#include <string.h>

#include "bme280.hpp"

using namespace upm;
using namespace std;

BME280::BME280(int bus, int addr, int cs, uint8_t theChipID) :
  BMP280(bus, addr, cs, theChipID)
{

  m_humidity = 0;

  m_dig_H1 = 0;
  m_dig_H2 = 0;
  m_dig_H3 = 0;
  m_dig_H4 = 0;
  m_dig_H5 = 0;
  m_dig_H6 = 0;

  // set sleep mode for now
  setMeasureMode(MODE_SLEEP);

  // read calibration data
  readCalibrationData();

  // set the default mode to the highest resolution mode
  setUsageMode(USAGE_MODE_INDOOR_NAV);
}

BME280::~BME280()
{
}

void BME280::update()
{
  // call the base class method first.  This will handle the details
  // WRT forced mode so that we can always be sure to read valid data
  // regardless of the underlying measurement mode.

  BMP280::update();

  int32_t hum = 0;

  const int dataLen = 2;
  uint8_t data[dataLen];
  memset(data, 0, dataLen);

  int rv;
  if ((rv = readRegs(REG_HUMIDITY_MSB, data, dataLen)) != dataLen)
    {
      throw std::runtime_error(std::string(__FUNCTION__)
                               + ": readRegs() failed, returned "
                               + std::to_string(rv));
    }

  // 20 bits unsigned stored in a 32bit signed quanty

  hum = ( (data[0] << 8) | data[1] );

  m_humidity = float(bme280_compensate_H_int32(hum));
  m_humidity /= 1024.0;
}

void BME280::readCalibrationData()
{
  m_dig_H1 = readReg(REG_CALIB_DIG_H1);

  const int calibLen = 7;
  uint8_t calibData[calibLen];
  readRegs(REG_CALIB_DIG_H2_LSB, calibData, calibLen);

  m_dig_H2 = int16_t((calibData[1] << 8) | calibData[0]);

  m_dig_H3 = calibData[2];

  m_dig_H4 = int16_t( (calibData[3] << 4) | 
                      (calibData[4] & 0x0f) );

  m_dig_H5 = int16_t( ((calibData[4] & 0xf0) >> 4) | 
                      (calibData[5] << 4) );

  m_dig_H6 = int8_t(calibData[6]);

# if 0
  cerr << std::dec 
       << "H1: "  << (int)m_dig_H1
       << " H2: " << (int)m_dig_H2
       << " H3: " << (int)m_dig_H3
       << " H4: " << (int)m_dig_H4
       << " H5: " << (int)m_dig_H5
       << " H6: " << (int)m_dig_H5
       << endl;
# endif // 0

  // The BMP280 ctor will call it's version of readCalibrationData
}

float BME280::getHumidity()
{
  return m_humidity;
}

void BME280::setOversampleRateHumidity(OSRS_H_T rate)
{
  uint8_t reg = readReg(REG_CTRL_HUM);

  reg &= ~(_CTRL_HUM_OSRS_H_MASK << _CTRL_HUM_OSRS_H_SHIFT);
  reg |= (rate << _CTRL_HUM_OSRS_H_SHIFT);

  writeReg(REG_CTRL_HUM, reg);
}


void BME280::setUsageMode(USAGE_MODE_T mode)
{
  // Here, we just set the default humidity oversample to 1 and then
  // call the base method.

  m_humidity = 0;

  // set sleep mode first
  setMeasureMode(MODE_SLEEP);
  
  setOversampleRateHumidity(OSRS_H_OVERSAMPLING_1);
  BMP280::setUsageMode(mode);
}

// This function comes from the BMP180 datasheet, section 4.2.3

// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format
// (22 integer and 10 fractional bits).  Output value of “47445”
// represents 47445/1024 = 46.333 %RH
uint32_t BME280::bme280_compensate_H_int32(int32_t adc_H)
{
  int32_t v_x1_u32r;
  v_x1_u32r = (m_t_fine - ((int32_t)76800));
  v_x1_u32r = (((((adc_H << 14) - (((int32_t)m_dig_H4) << 20) - (((int32_t)m_dig_H5) * v_x1_u32r)) +
                 ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)m_dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)m_dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)m_dig_H2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)m_dig_H1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
  v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
  return (int32_t)(v_x1_u32r>>12);
}

