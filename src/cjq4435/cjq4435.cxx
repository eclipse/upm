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

#include "cjq4435.h"

using namespace upm;
using namespace std;

CJQ4435::CJQ4435(int pin)
{
  if ( !(m_pwm = mraa_pwm_init(pin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_pwm_init() failed, invalid pin?");
      return;
    }

  m_enabled = false;
}

CJQ4435::~CJQ4435()
{
  if (m_enabled)
    mraa_pwm_enable(m_pwm, 0);

  mraa_pwm_close(m_pwm);
}

void CJQ4435::setPeriodUS(int us)
{
  if (mraa_pwm_period_us(m_pwm, us) != MRAA_SUCCESS)
    cerr << __FUNCTION__ << ": period specified is not supported" 
         << endl;
}

void CJQ4435::setPeriodMS(int ms)
{
  if (mraa_pwm_period_ms(m_pwm, ms) != MRAA_SUCCESS)
    cerr << __FUNCTION__ << ": period specified is not supported" 
         << endl;
}

void CJQ4435::setPeriodSeconds(float seconds)
{
  if (mraa_pwm_period(m_pwm, seconds) != MRAA_SUCCESS)
    cerr << __FUNCTION__ << ": period specified is not supported" 
         << endl;
}

void CJQ4435::enable(bool enable)
{
  m_enabled = enable;
  mraa_pwm_enable(m_pwm, ((enable) ? 1 : 0));
}

void CJQ4435::setDutyCycle(float dutyCycle)
{
  if (dutyCycle < 0.0)
    dutyCycle = 0.0;

  if (dutyCycle > 1.0)
    dutyCycle = 1.0;

  mraa_pwm_write(m_pwm, dutyCycle);
}

void CJQ4435::on()
{
  // set a 1 second period, with 100% duty cycle

  enable(false);
  setPeriodUS(1000);
  setDutyCycle(1.0);
  enable(true);
}

void CJQ4435::off()
{
  // set a 1 second period, with 0% duty cycle

  enable(false);
  setPeriodUS(1000);
  setDutyCycle(0.0);
  enable(true);
}

