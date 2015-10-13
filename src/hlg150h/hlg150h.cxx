#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include "hlg150h.h"
#include "mraa-utils.h"

#define PWM_PERIOD 3333

using namespace upm;

HLG150H::HLG150H(int pinRelay, int pinPWM)
{
   int dutyPercent = 0;
   status = MRAA_SUCCESS;
   this->pinRelay = pinRelay;
   isPoweredShadow = false;
   pwmBrightness = mraa_pwm_init(pinPWM);
   if (pwmBrightness == NULL)
      UPM_THROW("mraa_pwm_init() failed");
   status = mraa_pwm_enable(pwmBrightness, 1);
   status = mraa_pwm_period_us(pwmBrightness, PWM_PERIOD);
   if (!isConfigured())
      UPM_THROW("pwm config failed.");
   dutyPercent = getBrightness();
   isPoweredShadow = dutyPercent > 10;
}

HLG150H::~HLG150H()
{
}

bool HLG150H::isConfigured()
{
   return status == MRAA_SUCCESS;
}


void HLG150H::setPowerOn()
{
   isPoweredShadow = true;
   MraaUtils::setGpio(pinRelay, 0);
}

void HLG150H::setPowerOff()
{
   isPoweredShadow = false;
   MraaUtils::setGpio(pinRelay, 1);
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


// If duty is less than 10% light will flicker
void HLG150H::setBrightness(int dutyPercent)
{
   if (dutyPercent < 10)
      dutyPercent = 10;
   if (isConfigured())
   {
      int dutyUs = (PWM_PERIOD * dutyPercent) / 100;
      dutyUs = PWM_PERIOD - dutyUs;
      status = mraa_pwm_pulsewidth_us(pwmBrightness, dutyUs);
      // std::cout << "Brightness = " << dutyPercent << "%, duty = " << dutyUs << "us" << std::endl;      
   }
   else
      UPM_THROW("setBrightness failed");

}


int HLG150H::getBrightness()
{
   if (isConfigured())
   {
      float duty = mraa_pwm_read(pwmBrightness);
      int dutyPercent = static_cast<int>(100.0 * (1.0 - duty) + 0.5);
      return dutyPercent;
   }
   else
      UPM_THROW("getBrightness failed");
}


