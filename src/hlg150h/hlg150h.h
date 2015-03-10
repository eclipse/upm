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

#include "upm/light-controller.h"
#include "mraa/gpio.h"
#include "mraa/pwm.h"

namespace upm
{

/**
 * @brief lp8860 Lighting power supply controller library
 * @defgroup hlg150h libupm-hlg150h
 */

/**
 * @brief C++ API for HLG-150H lighting power supply 
 *
 * The Mean Well
 * [HLG-150H](http://www.meanwell.com/webapp/product/search.aspx?prod=HLG-150H)
 * 150W Single Output Switching Power Supply
 *
 * @ingroup hlg150h pwm
 * @snippet hlg150h.cxx Interesting
 */

class HLG150H : public upm::ILightController
{
public:
   HLG150H(int pinRelay, int pinPWM);
   ~HLG150H();

protected:
   bool isOK();
   bool setPowerOn();
   bool setPowerOff();
   bool isPowered();
   bool setBrightness(int dutyPercent);
   bool getBrightness(int* dutyPercent);
   bool getBrightnessRange(int* percentMin, int* percentMax);

private:
   mraa_result_t status;
   bool isPoweredShadow;
   int pinRelay;
   mraa_pwm_context pwmBrightness;
};

}



