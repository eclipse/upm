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

// #include "mraa/gpio.hpp"
#include "mraa/pwm.hpp"

namespace upm
{
    #define UPM_THROW(msg) throw std::runtime_error(std::string(__FUNCTION__) + ": " + (msg))

/**
 * @brief Meanwell HLG150H Lighting Power Supply Controller
 * @defgroup hlg150h libupm-hlg150h
 * @ingroup pwm light ilightcontroller
 */

/**
 * @library hlg150h
 * @sensor hlg150h
 * @comname 150W Constant Voltage/current LED Driver
 * @altname HLG150H
 * @type light
 * @con pwm
 * @web https://www.meanwell-web.com/en/product-info/ac-dc-power-supply/led-driver/101-180-w-led-driver/hlg-150h/product/HLG-150H-24B
 *
 * @brief API for HLG-150H Lighting Power Supply Controller
 *
 * The Mean Well
 * [HLG-150H](http://www.meanwell.com/webapp/product/search.aspx?prod=HLG-150H)
 * 150W Single Output Switching Power Supply
 *
 * @snippet hlg150h.cxx Interesting
 */

class HLG150H
{
public:
   HLG150H(int pinRelay, int pinPWM);
   ~HLG150H();

   int getBrightness();
   const char* getModuleName() { return "hlg150h"; }
   void setPowerOn();
   void setPowerOff();
   bool isPowered();
   void setBrightness(int dutyPercent);

private:
   /* Disable implicit copy and assignment operators */
   HLG150H(const HLG150H&) = delete;
   HLG150H &operator=(const HLG150H&) = delete;

   mraa::Result status;
   bool isPoweredShadow;
   int pinRelay;
   mraa::Pwm* pwmBrightness;
};

}



