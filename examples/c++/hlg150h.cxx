#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "hlg150h.hpp"

#define HLG150H_GPIO_RELAY 21
#define HLG150H_GPIO_PWM 22

void printState(upm::ILightController *lightController)
{
   if (lightController->isPowered())
   {
      std::cout << "Light is powered, brightness = " << lightController->getBrightness() << std::endl;
   }
   else
   {
      std::cout << "Light is not powered." << std::endl;
   }
}

int main( int argc, char **argv )
{
   int status = 0;
   upm::ILightController* lightController;

   try {
      lightController = new upm::HLG150H(HLG150H_GPIO_RELAY, HLG150H_GPIO_PWM);
      std::cout <<  "Existing state: "; printState(lightController);
      if (argc == 2)
      {
         std::string arg = argv[1];
         int brightness = ::atoi(argv[1]);
         if (brightness > 0) {
            lightController->setPowerOn();
            lightController->setBrightness(brightness);
         } else
            lightController->setPowerOff();
      }
      std::cout <<  "Now: ";printState(lightController);
      delete lightController;
   } catch (std::exception& e) {
      std::cout << "Error: " << e.what() << std::endl;
      status = 1;
   }

   return status;
}


