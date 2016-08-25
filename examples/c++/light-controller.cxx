#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "lp8860.hpp"
#include "ds1808lc.hpp"
#include "hlg150h.hpp"

#define EDISON_I2C_BUS 1   // Edison I2C-1
#define GPIO_SI7005_CS 20 // Edison GP12
#define HLG150H_GPIO_RELAY 21
#define HLG150H_GPIO_PWM 22
#define LP8860_GPIO_PWR 45 // Edison GP45
#define DS1808_GPIO_PWR 15 // Edison GP165
#define DS1808_GPIO_EDISON_LIVE 36 // Edison GP14

//! [Interesting]
// Simple example of using ILightController to determine 
// which controller is present and return its name.
// ILightController is then used to get readings from sensor

upm::ILightController* getLightController()
{
   upm::ILightController* lightController = NULL;
   try {
      lightController = new upm::LP8860(LP8860_GPIO_PWR, EDISON_I2C_BUS);
      return lightController;
   } catch (std::exception& e) {
      std::cerr << "LP8860: " << e.what() << std::endl;      
   }
   try {
      lightController = new upm::DS1808LC(DS1808_GPIO_PWR, EDISON_I2C_BUS);
      return lightController;
   } catch (std::exception& e) {
      std::cerr << "DS1808LC: " << e.what() << std::endl;      
   }
   try {
      lightController = new upm::HLG150H(HLG150H_GPIO_RELAY, HLG150H_GPIO_PWM);
      return lightController;
   } catch (std::exception& e) {
      std::cerr << "HLG150H: " << e.what() << std::endl;      
   }
   return lightController;   
}


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
   // MraaUtils::setGpio(GPIO_SI7005_CS, 1);

   upm::ILightController *lightController = getLightController();
   if (lightController != NULL)
   {
      std::cout << "Detected light controller " << lightController->getModuleName() << std::endl;      
   }
   else
   {
      std::cerr << "Error. Unsupported platform." << std::endl;
      return 1;
   }

   try {
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


