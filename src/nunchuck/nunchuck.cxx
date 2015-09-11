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
#include <unistd.h>

#include "nunchuck.h"

using namespace upm;
using namespace std;


NUNCHUCK::NUNCHUCK(int bus, uint8_t addr)
{
  stickX = 0;
  stickY = 0;
  accelX = 0;
  accelY = 0;
  accelZ = 0;
  buttonC = false;
  buttonZ = false;

  // setup our i2c link
  if ( !(m_i2c = mraa_i2c_init(bus)) ) 
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_i2c_init() failed");
      return;
    }

  mraa_result_t rv;

  if ( (rv = mraa_i2c_address(m_i2c, addr)) != MRAA_SUCCESS )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_i2c_address() failed");
    }
}

NUNCHUCK::~NUNCHUCK()
{
  mraa_i2c_stop(m_i2c);
}

bool NUNCHUCK::writeByte(uint8_t reg, uint8_t byte)
{
  mraa_result_t rv;

  if ( (rv = mraa_i2c_write_byte_data(m_i2c, byte, reg)) != MRAA_SUCCESS )
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_i2c_write_byte_data() failed");
      return false;
    }

  return true;
}

int NUNCHUCK::readBytes(uint8_t reg, uint8_t *buffer, int len)
{
  if (!len || !buffer)
    return 0;

  mraa_i2c_address(m_i2c, NUNCHUCK_I2C_ADDR);
  mraa_i2c_write_byte(m_i2c, reg);

  return mraa_i2c_read(m_i2c, buffer, len);
}

bool NUNCHUCK::init()
{
  usleep(1000000);

  // disable encryption
  if (!writeByte(0xf0, 0x55))
    return false;

  if (!writeByte(0xfb, 0x00))
    return false;

  return true;
}

void NUNCHUCK::update()
{
  const int bufsize = 6;
  uint8_t buf[bufsize];
  int rv;

  rv = readBytes(0x00, buf, bufsize);

  if (rv != bufsize)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": readBytes() failed");
      return;
    }

  // analog stick X
  stickX = buf[0];

  // analog stick Y
  stickY = buf[1];

  // accelerometer X
  accelX = ( (buf[2] << 2) | ((buf[5] & 0x0c) >> 2) );

  // accelerometer Y
  accelY = ( (buf[3] << 2) | ((buf[5] & 0x30) >> 4) );

  // accelerometer Z
  accelZ = ( (buf[4] << 2) | ((buf[5] & 0xc0) >> 6) );

  // buttonC
  if (buf[5] & 0x02)
    buttonC = false;
  else
    buttonC = true;

  // buttonZ
  if (buf[5] & 0x01)
    buttonZ = false;
  else
    buttonZ = true;
}

