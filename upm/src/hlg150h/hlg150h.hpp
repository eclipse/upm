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

#include "interfaces/iLightController.hpp"
// #include "mraa/gpio.hpp"
#include "mraa/pwm.hpp"

namespace upm
{

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

class HLG150H : public upm::ILightController
{
public:
   HLG150H(int pinRelay, int pinPWM);
   ~HLG150H();

   virtual int getBrightness();
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



