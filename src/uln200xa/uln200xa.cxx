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

#include "uln200xa.h"

using namespace upm;
using namespace std;

ULN200XA::ULN200XA(int stepsPerRev, int i1, int i2, int i3, int i4)
{
  m_stepsPerRev = stepsPerRev;
  m_currentStep = 0;
  m_stepDelay = 0;
  m_stepDirection = 1;          // default is forward

  if ( !(m_stepI1 = mraa_gpio_init(i1)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(i1) failed, invalid pin?");
      return;
    }
  mraa_gpio_dir(m_stepI1, MRAA_GPIO_OUT);

  if ( !(m_stepI2 = mraa_gpio_init(i2)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(i2) failed, invalid pin?");
      mraa_gpio_close(m_stepI1);
      return;
    }
  mraa_gpio_dir(m_stepI2, MRAA_GPIO_OUT);

  if ( !(m_stepI3 = mraa_gpio_init(i3)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(i3) failed, invalid pin?");
      mraa_gpio_close(m_stepI1);
      mraa_gpio_close(m_stepI2);
      return;
    }
  mraa_gpio_dir(m_stepI3, MRAA_GPIO_OUT);

  if ( !(m_stepI4 = mraa_gpio_init(i4)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(i4) failed, invalid pin?");
      mraa_gpio_close(m_stepI1);
      mraa_gpio_close(m_stepI2);
      mraa_gpio_close(m_stepI3);
      
      return;
    }
  mraa_gpio_dir(m_stepI4, MRAA_GPIO_OUT);

  // set default speed to 1
  setSpeed(1);
}

void ULN200XA::initClock()
{
  gettimeofday(&m_startTime, NULL);
}

uint32_t ULN200XA::getMillis()
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


ULN200XA::~ULN200XA()
{
  mraa_gpio_close(m_stepI1);
  mraa_gpio_close(m_stepI2);
  mraa_gpio_close(m_stepI3);
  mraa_gpio_close(m_stepI4);
}

void ULN200XA::setSpeed(int speed)
{
  m_stepDelay = 60 * 1000 / m_stepsPerRev / speed;
}

void ULN200XA::setDirection(ULN200XA_DIRECTION_T dir)
{
  switch (dir)
    {
    case DIR_CW:
      m_stepDirection = 1;
      break;
    case DIR_CCW:
      m_stepDirection = -1;
      break;
    }
}

void ULN200XA::stepperStep()
{
  int step = m_currentStep % 8;

  // This motor requires a different sequencing order in 8-steps than
  // usual.

  //   Step I0 I1 I2 I3
  //     1  0  0  0  1
  //     2  0  0  1  1
  //     3  0  0  1  0
  //     4  0  1  1  0
  //     5  0  1  0  0
  //     6  1  1  0  0
  //     7  1  0  0  0
  //     8  1  0  0  1

  switch (step)
    {
    case 0:    // 0001
      mraa_gpio_write(m_stepI1, 0);
      mraa_gpio_write(m_stepI2, 0);
      mraa_gpio_write(m_stepI3, 0);
      mraa_gpio_write(m_stepI4, 1);
      break;
    case 1:    // 0011
      mraa_gpio_write(m_stepI1, 0);
      mraa_gpio_write(m_stepI2, 0);
      mraa_gpio_write(m_stepI3, 1);
      mraa_gpio_write(m_stepI4, 1);
      break;
    case 2:    // 0010
      mraa_gpio_write(m_stepI1, 0);
      mraa_gpio_write(m_stepI2, 0);
      mraa_gpio_write(m_stepI3, 1);
      mraa_gpio_write(m_stepI4, 0);
      break;
    case 3:    // 0110
      mraa_gpio_write(m_stepI1, 0);
      mraa_gpio_write(m_stepI2, 1);
      mraa_gpio_write(m_stepI3, 1);
      mraa_gpio_write(m_stepI4, 0);
      break;
    case 4:    // 0100
      mraa_gpio_write(m_stepI1, 0);
      mraa_gpio_write(m_stepI2, 1);
      mraa_gpio_write(m_stepI3, 0);
      mraa_gpio_write(m_stepI4, 0);
      break;
    case 5:    // 1100
      mraa_gpio_write(m_stepI1, 1);
      mraa_gpio_write(m_stepI2, 1);
      mraa_gpio_write(m_stepI3, 0);
      mraa_gpio_write(m_stepI4, 0);
      break;
    case 6:    // 1000
      mraa_gpio_write(m_stepI1, 1);
      mraa_gpio_write(m_stepI2, 0);
      mraa_gpio_write(m_stepI3, 0);
      mraa_gpio_write(m_stepI4, 0);
      break;
    case 7:    // 1001
      mraa_gpio_write(m_stepI1, 1);
      mraa_gpio_write(m_stepI2, 0);
      mraa_gpio_write(m_stepI3, 0);
      mraa_gpio_write(m_stepI4, 1);
      break;
    }
}

void ULN200XA::stepperSteps(unsigned int steps)
{
  while (steps > 0)
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

          steps--;
          stepperStep();
        }
    }
}

void ULN200XA::release()
{
  mraa_gpio_write(m_stepI1, 0);
  mraa_gpio_write(m_stepI2, 0);
  mraa_gpio_write(m_stepI3, 0);
  mraa_gpio_write(m_stepI4, 0);
}
