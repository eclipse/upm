#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include "hlg150h.hpp"
#include "mraa-utils.hpp"

#define PWM_PERIOD 3333

using namespace upm;

HLG150H::HLG150H(int pinRelay, int pinPWM)
{
   int dutyPercent = 0;
   status = mraa::SUCCESS;
   this->pinRelay = pinRelay;
   isPoweredShadow = false;
   pwmBrightness = new mraa::Pwm(pinPWM);
   status = pwmBrightness->enable(true);
   status = pwmBrightness->period_us(PWM_PERIOD);
   if (status != mraa::SUCCESS)
      UPM_THROW("pwm config failed.");
   dutyPercent = getBrightness();
   isPoweredShadow = dutyPercent > 10;
}

HLG150H::~HLG150H()
{
   delete pwmBrightness;
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
   int dutyUs = (PWM_PERIOD * dutyPercent) / 100;
   dutyUs = PWM_PERIOD - dutyUs;
   status = pwmBrightness->pulsewidth_us(dutyUs);
   // std::cout << "Brightness = " << dutyPercent << "%, duty = " << dutyUs << "us" << std::endl;
   if (status != mraa::SUCCESS)
      UPM_THROW("setBrightness failed");

}


int HLG150H::getBrightness()
{
   float duty = pwmBrightness->read();
   int dutyPercent = static_cast<int>(100.0 * (1.0 - duty) + 0.5);
   return dutyPercent;
}


