/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdexcept>
#include <string>

#include "mraa-utils.hpp"
#include "mraa/gpio.hpp"

#define UPM_THROW(msg) throw std::runtime_error(std::string(__FUNCTION__) + ": " + (msg))

void MraaUtils::setGpio(int pin, int level)
{
   mraa::Gpio gpio(pin);
   gpio.dir(mraa::DIR_OUT);
   if (gpio.write(level) != mraa::SUCCESS)
      UPM_THROW("gpio write failed");
}


int MraaUtils::getGpio(int pin)
{
   mraa::Gpio gpio(pin);
   gpio.dir(mraa::DIR_IN);
   return gpio.read();
}


