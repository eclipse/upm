/*
 * Author: Henry Bruce <henry.bruce@intel.com>
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
#include <unistd.h>
#include "upm/light-controller-pwm.h"
#include "mraa-utils.h"

#define PWM_PERIOD 3333


HLG150H::HLG150H(int pinRelay, int pinPWM)
{
   int dutyPercent = 0;
   status = MRAA_SUCCESS;
   this->pinRelay = pinRelay;
   isPoweredShadow = false;
   pwmBrightness = mraa_pwm_init(pinPWM);
   if (pwmBrightness != NULL)
   {
      status = mraa_pwm_enable(pwmBrightness, 1);
      status = mraa_pwm_period_us(pwmBrightness, PWM_PERIOD);
      isPoweredShadow = dutyPercent > 10;
      getBrightness(&dutyPercent);
   }
}

HLG150H::~HLG150H()
{
}

bool HLG150H::isOK()
{
   return status == MRAA_SUCCESS;
}


bool HLG150H::getBrightnessRange(int* percentMin, int* percentMax)
{
   *percentMin = 10;
   *percentMax = 100;
   return true;
}


bool HLG150H::setPowerOn()
{
   if (isOK())
   {
      status = MraaUtils::setGpio(pinRelay, 0);
      isPoweredShadow = true;
   }
   return isOK();  
}

bool HLG150H::setPowerOff()
{
   if (isOK())
   {
      status = MraaUtils::setGpio(pinRelay, 1);
      isPoweredShadow = false;
   }
   return isOK();  
}

bool HLG150H::isPowered()
{
   // Can't read GPIO state as setting in to input mode turns off relay
   // Instead we return a shadow variable
   /*
   int level;
   if (MraaUtils::getGpio(pinRelay, &level) == MRAA_SUCCESS)
      return level == 1;
   else
      return false;
   */
   return isPoweredShadow;
}


bool HLG150H::setBrightness(int dutyPercent)
{
   if (isOK())
   {
      int dutyUs = (PWM_PERIOD * dutyPercent) / 100;
      dutyUs = PWM_PERIOD - dutyUs;
      status = mraa_pwm_pulsewidth_us(pwmBrightness, dutyUs);
      // std::cout << "Brightness = " << dutyPercent << "%, duty = " << dutyUs << "us" << std::endl;      
   }
   return isOK();  
}


bool HLG150H::getBrightness(int* dutyPercent)
{
   if (isOK())
   {
      float duty = mraa_pwm_read(pwmBrightness);
      *dutyPercent = static_cast<int>(100.0 * (1.0 - duty) + 0.5);
   }
   return isOK();  
}


