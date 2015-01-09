/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include "mpr121.h"

using namespace upm;
using namespace std;


MPR121::MPR121(int bus, uint8_t address)
{
  // setup our i2c link
  m_i2c = mraa_i2c_init(bus);

  m_addr = address;

  mraa_result_t ret = mraa_i2c_address(m_i2c, m_addr);

  if (ret != MRAA_SUCCESS) 
    cerr << "MPR121: Could not initialize i2c bus. " << endl;

  m_buttonStates = 0;
  m_overCurrentFault = false;
}

MPR121::~MPR121()
{
  mraa_i2c_stop(m_i2c);
}

mraa_result_t MPR121::writeBytes(uint8_t reg, uint8_t *buffer, unsigned int len)
{
  if (!len || !buffer)
    return MRAA_SUCCESS;

  // create a buffer 1 byte larger than the supplied buffer,
  // store the register in the first byte
  uint8_t buf2[len + 1];

  buf2[0] = reg;

  // copy in the buffer after the reg byte
  for (int i=1; i<(len + 1); i++)
    buf2[i] = buffer[i-1];

  mraa_i2c_address(m_i2c, m_addr);

  return mraa_i2c_write(m_i2c, buf2, len + 1);
}

void MPR121::readBytes(uint8_t reg, uint8_t *buffer, unsigned int len)
{
  if (!len || !buffer)
    return;

  // The usual mraa_i2c_read() does not work here, so we need to
  // read each byte individually.
  for (int i=0; i<len; i++)
    buffer[i] = mraa_i2c_read_byte_data(m_i2c, reg + i);

  return;
}

bool MPR121::configAN3944()
{
  // Configure the mpr121 chip as recommended in the AN3944 MPR121
  // Quick Start Guide

  mraa_result_t rv;

  // First, turn off all electrodes by zeroing out the Electrode Configuration
  // register.
  // If this one fails, it's unlikely any of the others will succeed.
  uint8_t eleConf = 0x00;
  if ((rv = writeBytes(0x5e, &eleConf, 1)) != MRAA_SUCCESS)
    {
      cerr << __FUNCTION__ << ": " << __LINE__<< ": I2C write failed." << endl;
      return false;
    }

  // Section A
  // Filtering when data is greater than baseline
  // regs 0x2b-0x2e
  uint8_t sectA[] = { 0x01, 0x01, 0x00, 0x00 };
  if ((rv = writeBytes(0x2b, sectA, 4)) != MRAA_SUCCESS)
    {
      cerr << __FUNCTION__ << ": " << __LINE__<< ": I2C write failed." << endl;
      return false;
    }

  // Section B
  // Filtering when data is less than baseline
  // regs 0x2f-0x32
  uint8_t sectB[] = { 0x01, 0x01, 0xff, 0x02 };
  if ((rv = writeBytes(0x2f, sectB, 4)) != MRAA_SUCCESS)
    {
      cerr << __FUNCTION__ << ": " << __LINE__<< ": I2C write failed." << endl;
      return false;
    }

  // Section C
  // Touch Threshold/Release registers, ELE0-ELE11
  // regs 0x41-0x58
  //                __T_  __R_
  uint8_t sectC[] = { 0x0f, 0x0a, 
                      0x0f, 0x0a,
                      0x0f, 0x0a,
                      0x0f, 0x0a,
                      0x0f, 0x0a,
                      0x0f, 0x0a,
                      0x0f, 0x0a,
                      0x0f, 0x0a,
                      0x0f, 0x0a,
                      0x0f, 0x0a,
                      0x0f, 0x0a,
                      0x0f, 0x0a };
  if ((rv = writeBytes(0x41, sectC, 24)) != MRAA_SUCCESS)
    {
      cerr << __FUNCTION__ << ": " << __LINE__<< ": I2C write failed." << endl;
      return false;
    }

  // Section D
  // Filter configuration
  // reg 0x5d
  uint8_t filterConf = 0x04;
  if ((rv = writeBytes(0x5d, &filterConf, 1)) != MRAA_SUCCESS)
    {
      cerr << __FUNCTION__ << ": " << __LINE__<< ": I2C write failed." << endl;
      return false;
    }

  // Section F
  // Autoconfiguration registers
  // regs 0x7b-0x7f
  uint8_t sectF0 = 0x0b;
  if ((rv = writeBytes(0x7b, &sectF0, 1)) != MRAA_SUCCESS)
    {
      cerr << __FUNCTION__ << ": " << __LINE__<< ": I2C write failed." << endl;
      return false;
    }
  uint8_t sectF1[] = { 0x9c, 0x65, 0x8c };
  if ((rv = writeBytes(0x7d, sectF1, 3)) != MRAA_SUCCESS)
    {
      cerr << __FUNCTION__ << ": " << __LINE__<< ": I2C write failed." << endl;
      return false;
    }

  // Section E - this one must be set last, and switches to run mode
  // Enable all 12 electrodes, and set a pre-calibration to avoid
  // excessive calibration delay on startup.
  // reg 0x5e
  eleConf = 0x8c;
  if ((rv = writeBytes(0x5e, &eleConf, 1)) != MRAA_SUCCESS)
    {
      cerr << __FUNCTION__ << ": " << __LINE__<< ": I2C write failed." << endl;
      return false;
    }

  return true;
}

void MPR121::readButtons()
{
  uint8_t rv;
  uint8_t buffer[2];

  // read in the 2 bytes at register 0x00-0x01, and setup the member
  // variables accordingly.

  readBytes(0x00, buffer, 2);

  m_buttonStates = (buffer[0] | ((buffer[1] & 0x1f) << 8));
  if (buffer[1] & 0x80)
    m_overCurrentFault = true;
  else
    m_overCurrentFault = false;

  return;
}
