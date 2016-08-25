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

#pragma once

#include "iModuleStatus.hpp"

namespace upm
{
/**
 * @brief ILightController Interface for Light Controllers
 */
 
/**
 *
 * @brief Interface for Light Controllers
 
 * This interface is used to represent light controllers

 * @snippet light-controllers.cxx Interesting 
 */

    class ILightController : public IModuleStatus
    {
    public:
      /**
       * Turn on power
       *
       * @throws std::runtime_error
       */
       virtual void setPowerOn() = 0;

      /**
       * Turn off power
       *
       * @throws std::runtime_error
       */
       virtual void setPowerOff() = 0;

      /**
       * Get power state
       *
       * @return true if powered, false otherwise
       *
       * @throws std::runtime_error
       */
       virtual bool isPowered() = 0;

      /**
       * Set brightness
       *
       * @param brightness as percentage
       *
       * @throws std::runtime_error
       */
       virtual void setBrightness(int percent) = 0;

      /**
       * Get brightness
       *
       * @return brightness as percentage
       *
       * @throws std::runtime_error
       */
       virtual int getBrightness() = 0;
       
       virtual ~ILightController() {}
    };

}



