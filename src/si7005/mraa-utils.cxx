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

#include "mraa-utils.hpp"
#include "mraa/gpio.hpp"

 #define UPM_THROW(msg) throw std::runtime_error(std::string(__FUNCTION__) + ": " + (msg))

void MraaUtils::setGpio(int pin, int level)
{
/*
   mraa_result_t status = MRAA_ERROR_NO_RESOURCES; 
   mraa_gpio_context gpio = mraa_gpio_init(pin);
   if (gpio != NULL) 
   {
      mraa_gpio_dir(gpio, MRAA_GPIO_OUT);
      status = mraa_gpio_write(gpio, level);
      mraa_gpio_close(gpio);
   } 
   return status;
*/
   mraa::Gpio gpio(pin);
   gpio.dir(mraa::DIR_OUT);
   if (gpio.write(level) != mraa::SUCCESS)
      UPM_THROW("gpio write failed");
}


int MraaUtils::getGpio(int pin)
{
/*
   mraa_result_t status = MRAA_ERROR_NO_RESOURCES; 
   mraa_gpio_context gpio = mraa_gpio_init(pin);
   if (gpio != NULL) 
   {
      status = mraa_gpio_dir(gpio, MRAA_GPIO_IN);
      int value = mraa_gpio_read(gpio);
      if (value != -1)
         *level = value;
      else
         status = MRAA_ERROR_NO_RESOURCES; 
      mraa_gpio_close(gpio);
   }
   return status;       
*/
   mraa::Gpio gpio(pin);
   gpio.dir(mraa::DIR_IN);
   return gpio.read();
}


