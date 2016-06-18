/*
 * Author: Henry Bruce <henry.bruce@intel.com>
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

#include <unistd.h>
#include <iostream>
#include "max44009.hpp"
#include "si1132.hpp"

#define EDISON_I2C_BUS 1 
#define FT4222_I2C_BUS 0

//! [Interesting]
// Simple example of using ILightSensor to determine 
// which sensor is present and return its name.
// ILightSensor is then used to get readings from sensor


upm::ILightSensor* getLightSensor()
{
   upm::ILightSensor* lightSensor = NULL;
   try {
      lightSensor = new upm::SI1132(mraa_get_sub_platform_id(FT4222_I2C_BUS));
      return lightSensor;
   } catch (std::exception& e) {
      std::cerr << "SI1132: " << e.what() << std::endl;      
   }
   try {
      lightSensor = new upm::MAX44009(EDISON_I2C_BUS);
      return lightSensor;
   } catch (std::exception& e) {
      std::cerr << "MAX44009: " << e.what() << std::endl;      
   }
   return lightSensor;   
}

int main ()
{
   upm::ILightSensor* lightSensor = getLightSensor();
   if (lightSensor == NULL) {
      std::cout << "Light sensor not detected" << std::endl;                        
      return 1;
   }
   std::cout << "Light sensor " << lightSensor->getModuleName() << " detected" << std::endl;
   while (true) {
      try {
         float value = lightSensor->getVisibleLux();
         std::cout << "Light level = " << value << " lux" << std::endl;
      } catch (std::exception& e) {
         std::cerr << e.what() << std::endl;
      }
      sleep(1);         
   }
   delete lightSensor;
   return 0;
}

//! [Interesting]      