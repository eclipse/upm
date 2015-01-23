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

#include <unistd.h>
#include <iostream>
#include <string>

#include "grovemd.h"

using namespace upm;
using namespace std;


GroveMD::GroveMD(int bus, uint8_t address)
{
  m_addr = address;

  // setup our i2c link
  if ( !(m_i2c = mraa_i2c_init(bus)) )
    {
      cerr << __FUNCTION__ << ": mraa_i2c_init() failed" << endl;
      return;
    }

  // this board *requires* 100Khz i2c bus only
  mraa_result_t rv;
  if ( (rv = mraa_i2c_frequency(m_i2c, MRAA_I2C_STD)) != MRAA_SUCCESS )
    {
      cerr << "GroveMD: Could not set i2c frequency (MRAA_I2C_STD). " << endl;
      mraa_result_print(rv);
      return;
    }

  if (mraa_i2c_address(m_i2c, m_addr))
    {
      cerr << "GroveMD: Could not initialize i2c bus. " << endl;
      return;
    }
}

GroveMD::~GroveMD()
{
  setMotorSpeeds(0, 0);
  mraa_i2c_stop(m_i2c);
}

bool GroveMD::writePacket(REG_T reg, uint8_t data1, uint8_t data2)
{
  uint8_t buf[3];

  buf[0] = reg;
  buf[1] = data1;
  buf[2] = data2;

  mraa_result_t rv;
  if ( (rv = mraa_i2c_address(m_i2c, m_addr)) != MRAA_SUCCESS )
    {
      cerr << __FUNCTION__ << ": mraa_i2c_address() failed. " << endl;
      mraa_result_print(rv);
      return false;
    }

  // This sleep appears to be required.  Without it, writes randomly
  // fail (no ACK received).  This happens most often on the SET_SPEED
  // packet.  I am guessing that there is a timing problem and/or bug
  // in the motor driver's firmware.

  usleep(100);

  if ( (rv = mraa_i2c_write(m_i2c, buf, 3)) != MRAA_SUCCESS )
    {
      cerr << __FUNCTION__ << ": mraa_i2c_write() failed:" << endl;
      mraa_result_print(rv);
      return false;
    }

  return true;
}

bool GroveMD::setMotorSpeeds(uint8_t speedA, uint8_t speedB)
{
  return writePacket(SET_SPEED, speedA, speedB);
}

bool GroveMD::setPWMFrequencyPrescale(uint8_t freq)
{
  return writePacket(SET_PWM_FREQ, freq, GROVEMD_NOOP);
}

bool GroveMD::setDirection(DIRECTION_T dir)
{
  return writePacket(SET_DIRECTION, dir, GROVEMD_NOOP);
}

bool GroveMD::enableStepper(DIRECTION_T dir, uint8_t speed)
{
  return writePacket(STEPPER_ENABLE, dir, speed);
}

bool GroveMD::disableStepper()
{
  return writePacket(STEPPER_DISABLE, GROVEMD_NOOP, GROVEMD_NOOP);
}

bool GroveMD::setStepperSteps(uint8_t steps)
{
  if (steps == 0)
    {
      // invalid
      cerr << __FUNCTION__ << ": invalid number of steps.  "
           << "Valid values are between 1 and 255." << endl;
      return false;
    }

  return writePacket(STEPPER_NUM_STEPS, steps, GROVEMD_NOOP);
}

