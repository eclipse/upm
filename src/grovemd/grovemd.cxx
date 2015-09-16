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
#include <stdexcept>

#include "grovemd.h"

using namespace upm;
using namespace std;


GroveMD::GroveMD(int bus, uint8_t address) :
  m_i2c(bus)
{
  m_addr = address;

  // this board *requires* 100Khz i2c bus only
  mraa::Result rv;
  if ( (rv = m_i2c.frequency(mraa::I2C_STD)) != mraa::SUCCESS )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": I2c.frequency(I2C_STD) failed");
      return;
    }

  if (m_i2c.address(m_addr))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.address() failed");
      return;
    }

  initClock();
  // default to mode1 stepper operation, 200 steps per rev.
  configStepper(200, STEP_MODE1);
}

GroveMD::~GroveMD()
{
  setMotorSpeeds(0, 0);
  writePacket(SET_DIRECTION, 0, GROVEMD_NOOP);
}

bool GroveMD::writePacket(REG_T reg, uint8_t data1, uint8_t data2)
{
  uint8_t buf[3];

  buf[0] = reg;
  buf[1] = data1;
  buf[2] = data2;

  if ( m_i2c.write(buf, 3) != mraa::SUCCESS )
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.write() failed");
      return false;
    }

  // This sleep appears to be required.  Without it, writes randomly
  // fail (no ACK received).  This happens most often on the SET_SPEED
  // packet.  I am guessing that there is a timing problem and/or bug
  // in the motor driver's firmware.

  usleep(100);

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

bool GroveMD::setMotorDirections(DC_DIRECTION_T dirA, DC_DIRECTION_T dirB)
{
  uint8_t dir = ((dirB & 0x03) << 2) | (dirA & 0x03);
  return writePacket(SET_DIRECTION, dir, GROVEMD_NOOP);
}

bool GroveMD::enableStepper(STEP_DIRECTION_T dir, uint8_t speed)
{
  // If mode 2, send the command and return immediately
  if (m_stepMode == STEP_MODE2)
    return writePacket(STEPPER_ENABLE, dir, speed);

  // otherwise, mode 1, setup the basics and start stepping.

  m_stepDelay = 60 * 1000 / m_stepsPerRev / speed;
  m_stepDirection = ((dir == STEP_DIR_CW) ? 1 : -1);

  // seeed says speed should always be 255,255 for stepper operation
  setMotorSpeeds(255, 255);

  while (m_totalSteps > 0)
    {
      if (getMillis() >= m_stepDelay)
        {
          // reset the clock
          initClock();

          m_currentStep += m_stepDirection;

          if (m_stepDirection == 1)
            {
              if (m_currentStep >= m_stepsPerRev)
                m_currentStep = 0;
            }
          else
            {
              if (m_currentStep <= 0)
                m_currentStep = m_stepsPerRev;
            }

          m_totalSteps--;
          stepperStep();
        }
    }

  // and... we're done
  return true;
}

bool GroveMD::disableStepper()
{
  if (m_stepMode == STEP_MODE2)
    return writePacket(STEPPER_DISABLE, GROVEMD_NOOP, GROVEMD_NOOP);

  // else, mode 1
  writePacket(SET_DIRECTION, 0, GROVEMD_NOOP);
  return setMotorSpeeds(0, 0);
}

bool GroveMD::setStepperSteps(unsigned int steps)
{
  if (m_stepMode == STEP_MODE2)
    {
      if (steps == 0)
        {
          // invalid
          throw std::out_of_range(std::string(__FUNCTION__) +
                                  ": invalid number of steps.  " +
                                  "Valid values are between 1 and 255.");
          return false;
        }
      return writePacket(STEPPER_NUM_STEPS, steps, GROVEMD_NOOP);
    }

  // for mode one, just store it for future use by enableStepper()
  m_totalSteps = steps;
  return true;
}

void GroveMD::initClock()
{
  gettimeofday(&m_startTime, NULL);
}

uint32_t GroveMD::getMillis()
{
  struct timeval elapsed, now;
  uint32_t elapse;

  // get current time
  gettimeofday(&now, NULL);

  // compute the delta since m_startTime
  if( (elapsed.tv_usec = now.tv_usec - m_startTime.tv_usec) < 0 )
    {
      elapsed.tv_usec += 1000000;
      elapsed.tv_sec = now.tv_sec - m_startTime.tv_sec - 1;
    }
  else
    {
      elapsed.tv_sec = now.tv_sec - m_startTime.tv_sec;
    }

  elapse = (uint32_t)((elapsed.tv_sec * 1000) + (elapsed.tv_usec / 1000));

  // never return 0
  if (elapse == 0)
    elapse = 1;

  return elapse;
}

void GroveMD::configStepper(unsigned int stepsPerRev, STEP_MODE_T mode)
{
  m_stepsPerRev = stepsPerRev;
  m_stepMode = mode;
  m_currentStep = 0;
  m_stepDelay = 0;
  m_stepDirection = 1;
  m_totalSteps = 0;
}

void GroveMD::stepperStep()
{
  int step = m_currentStep % 4;

  switch (step)
    {
    case 0:
      writePacket(SET_DIRECTION, 0b0101, GROVEMD_NOOP);
      break;
    case 1:
      writePacket(SET_DIRECTION, 0b0110, GROVEMD_NOOP);
      break;
    case 2:
      writePacket(SET_DIRECTION, 0b1010, GROVEMD_NOOP);
      break;
    case 3:
      writePacket(SET_DIRECTION, 0b1001, GROVEMD_NOOP);
      break;
    }
}
