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

#include "l298.h"

using namespace upm;
using namespace std;

// constructor for the DC motor(s) mode
L298::L298(int pwmA, int dir1, int dir2)
{
  // No stepper in this mode
  m_stepper = false;

  // disable until complete
  m_motor = false;

  if ( !(m_pwm = mraa_pwm_init(pwmA)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_pwm_init() failed, invalid pin?");
      return;
    }

  if ( !(m_dir1 = mraa_gpio_init(dir1)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(dir1) failed, invalid pin?");
      mraa_pwm_close(m_pwm);
      return;
    }
  mraa_gpio_dir(m_dir1, MRAA_GPIO_OUT);

  if ( !(m_dir2 = mraa_gpio_init(dir2)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(dir2) failed, invalid pin?");
      mraa_pwm_close(m_pwm);
      mraa_gpio_close(m_dir1);
      return;
    }
  mraa_gpio_dir(m_dir2, MRAA_GPIO_OUT);

  setPeriodMS(L298_DEFAULT_PWM_PERIOD);
  setDirection(DIR_NONE);
  setSpeed(0);
  m_motor = true;
}

// constructor for the stepper mode
L298::L298(int stepsPerRev, int en, int i1, int i2, int i3, int i4)
{
  // no DC motors in this mode
  m_motor = false;

  // disable until complete
  m_stepper = false;

  m_stepsPerRev = stepsPerRev;
  m_currentStep = 0;
  m_stepDelay = 0;
  m_stepDirection = 1;          // default is forward

  // init the gpio's we will need
  if ( !(m_stepEnable = mraa_gpio_init(en)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(en) failed, invalid pin?");
      return;
    }
  mraa_gpio_dir(m_stepEnable, MRAA_GPIO_OUT);

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

  m_stepper = true;
}


void L298::initClock()
{
  gettimeofday(&m_startTime, NULL);
}

uint32_t L298::getMillis()
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


L298::~L298()
{
  if (m_stepper)
    {
      enable(false);
      mraa_gpio_close(m_stepEnable);
      mraa_gpio_close(m_stepI1);
      mraa_gpio_close(m_stepI2);
      mraa_gpio_close(m_stepI3);
      mraa_gpio_close(m_stepI4);
    }

  if (m_motor)
    {
      setDirection(DIR_NONE);
      setSpeed(0);
      enable(false);
      mraa_pwm_close(m_pwm);
      mraa_gpio_close(m_dir1);
      mraa_gpio_close(m_dir2);
    }
}

void L298::setPeriodMS(int ms)
{
  if (m_motor)
    {
      if (mraa_pwm_period_ms(m_pwm, ms) != MRAA_SUCCESS)
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_pwm_period_ms() failed");
    }
}

void L298::enable(bool enable)
{
  if (m_motor)
    {
      mraa_pwm_enable(m_pwm, ((enable) ? 1 : 0));
    }

  if (m_stepper)
    {
      mraa_gpio_write(m_stepEnable, ((enable) ? 1 : 0));
    }
}

void L298::setSpeed(int speed)
{
  if (m_motor)
    {
      if (speed < 0)
        speed = 0;

      if (speed > 100)
        speed = 100;

      float percent = float(speed) / 100.0;

      if (m_motor)
        {
          mraa_pwm_write(m_pwm, percent);
        }
    }

  if (m_stepper)
    {
      m_stepDelay = 60 * 1000 / m_stepsPerRev / speed;
    }
}

void L298::setDirection(L298_DIRECTION_T dir)
{
  if (m_motor)
    {
      if (dir & 0x01)
        mraa_gpio_write(m_dir1, 1);
      else
        mraa_gpio_write(m_dir1, 0);

      if (dir & 0x02)
        mraa_gpio_write(m_dir2, 1);
      else
        mraa_gpio_write(m_dir2, 0);
    }

  if (m_stepper)
    {
      switch (dir)
        {
        case DIR_CW:
          m_stepDirection = 1;
          break;
        case DIR_CCW:
          m_stepDirection = -1;
          break;
        default:                // default to 1 if DIR_NONE specified
          m_stepDirection = 1;
          break;
        }
    }


}

void L298::stepperStep()
{
  int step = m_currentStep % 4;

  //   Step I0 I1 I2 I3
  //     1  1  0  1  0
  //     2  0  1  1  0
  //     3  0  1  0  1
  //     4  1  0  0  1

  switch (step)
    {
    case 0:    // 1010
      mraa_gpio_write(m_stepI1, 1);
      mraa_gpio_write(m_stepI2, 0);
      mraa_gpio_write(m_stepI3, 1);
      mraa_gpio_write(m_stepI4, 0);
      break;
    case 1:    // 0110
      mraa_gpio_write(m_stepI1, 0);
      mraa_gpio_write(m_stepI2, 1);
      mraa_gpio_write(m_stepI3, 1);
      mraa_gpio_write(m_stepI4, 0);
      break;
    case 2:    //0101
      mraa_gpio_write(m_stepI1, 0);
      mraa_gpio_write(m_stepI2, 1);
      mraa_gpio_write(m_stepI3, 0);
      mraa_gpio_write(m_stepI4, 1);
      break;
    case 3:    //1001
      mraa_gpio_write(m_stepI1, 1);
      mraa_gpio_write(m_stepI2, 0);
      mraa_gpio_write(m_stepI3, 0);
      mraa_gpio_write(m_stepI4, 1);
      break;
    }
}

void L298::stepperSteps(unsigned int steps)
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
          // cerr << "STEPNUM: " << m_currentStep << endl;
        }
    }
}
