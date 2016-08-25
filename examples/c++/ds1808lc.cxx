#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "ds1808lc.hpp"

#define EDISON_I2C_BUS 1            // Edison I2C-1
#define DS1808_GPIO_PWR 15          // Edison GP165

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
      lightController = new upm::DS1808LC(DS1808_GPIO_PWR, EDISON_I2C_BUS);
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
   } catch (std::exception& e) {
      std::cout << "Error: " << e.what() << std::endl;
      status = 1;
   }

   delete lightController;
   return status;
}


