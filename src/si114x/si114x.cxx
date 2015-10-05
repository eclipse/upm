/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Thanks to Adafruit for some important clues
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
#include <math.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "si114x.h"

using namespace upm;
using namespace std;


SI114X::SI114X(int bus, uint8_t address)
{
  m_addr = address;
  m_uvIndex = 0;

  // setup our i2c link
  if ( !(m_i2c = mraa_i2c_init(bus)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_i2c_init() failed");
      return;
    }
      
  mraa_result_t rv;
  
  if ( (rv = mraa_i2c_address(m_i2c, m_addr)) != MRAA_SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_i2c_address() failed");
      return;
    }

  // The data sheet recommends setting the UV calibration values to
  // 0x7b, 0x6b, 0x01, and 0x00, however the adafruit code uses a
  // different set of values, presumably calibrated to their specific
  // implementation.  We will use those defaults here, as this was
  // developed on an adafruit device.  

  // Use setUVCalibration() to set a different set of values before
  // calling init() if you need different values.

  setUVCalibration(0x29, 0x89, 0x02, 0x00);
}

SI114X::~SI114X()
{
  mraa_i2c_stop(m_i2c);
}

bool SI114X::writeByte(uint8_t reg, uint8_t byte)
{
  mraa_result_t rv = mraa_i2c_write_byte_data(m_i2c, byte, reg);

  if (rv != MRAA_SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_i2c_write_byte() failed");
      return false;
    }

  return true;
}

uint8_t SI114X::readByte(uint8_t reg)
{
  return mraa_i2c_read_byte_data(m_i2c, reg);
}

uint16_t SI114X::readWord(uint8_t reg)
{
  return mraa_i2c_read_word_data(m_i2c, reg);
}

void SI114X::setUVCalibration(uint8_t uvcoeff0, uint8_t uvcoeff1, 
                              uint8_t uvcoeff2, uint8_t uvcoeff3)
{
  m_uv_cal[0] = uvcoeff0;
  m_uv_cal[1] = uvcoeff1;
  m_uv_cal[2] = uvcoeff2;
  m_uv_cal[3] = uvcoeff3; 
};


void SI114X::writeParam(SI114X_PARAM_T param, uint8_t value)
{
  // write a parameter to the RAM parameter area

  // We write the value to the PARAM_WR register, then execute a
  // PARAM_WRITE command

  writeByte(REG_PARAM_WR, value);

  // now write it to parameter memory
  writeByte(REG_COMMAND, CMD_PARAM_SET | param);
}

uint8_t SI114X::readParam(SI114X_PARAM_T param)
{
  // get the parameter into register REG_PARAM_READ, then read and return it.

  writeByte(REG_COMMAND, CMD_PARAM_QUERY | param);
  return readByte(REG_PARAM_READ);
}

void SI114X::reset()
{
  // reset the device 

  // zero out measuring rate
  writeByte(REG_MEAS_RATE0, 0);
  writeByte(REG_MEAS_RATE1, 0);

  // disable IRQ MODES
  // these are undocumented in the datasheet, but mentioned in Adafruit's code
  writeByte(REG_IRQ_MODE1, 0);
  writeByte(REG_IRQ_MODE2, 0);
  
  // turn off interrupts
  writeByte(REG_INT_CFG, 0);
  writeByte(REG_IRQ_STATUS, 0xff);

  // send a reset
  writeByte(REG_COMMAND, CMD_RESET);
  usleep(100);

  // set the hardware key
  writeByte(REG_HW_KEY, SI114X_HW_KEY);
  usleep(100);
}

void SI114X::initialize()
{
  // initialize the device

  // first, reset it
  reset();

  // UV coefficients
  writeByte(REG_UCOEF0, m_uv_cal[0]);
  writeByte(REG_UCOEF1, m_uv_cal[1]);
  writeByte(REG_UCOEF2, m_uv_cal[2]);
  writeByte(REG_UCOEF3, m_uv_cal[3]);
  
  // enable UV sensor only for now
  writeParam(PARAM_CHLIST, CHLIST_EN_UV);

  // auto-measure speed - slowest - (rate * 31.25us)
  writeByte(REG_MEAS_RATE0, 0xff); // 7.9ms

  // set autorun
  writeByte(REG_COMMAND, CMD_ALS_AUTO);
}

void SI114X::update()
{
  // for now, just update the UV Index member variable
  uint16_t uvi = readWord(REG_AUX_UVINDEX0);

  m_uvIndex = float(uvi) / 100.0;

  // Add any further data gets() here

  return;
}
