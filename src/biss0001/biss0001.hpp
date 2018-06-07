/*
 * Author: Zion Orent <zorent@ics.com>
 *         Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
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

#include <interfaces/iMotion.hpp>
#include <biss0001.h>

namespace upm {
    /**
     * @brief BISS0001 Motion Sensor
     * @defgroup biss0001 libupm-biss0001
     * @ingroup seeed gpio light tsk
     */

    /**
     * @library biss0001
     * @sensor biss0001
     * @comname Passive Infrared (PIR) Motion Sensor
     * @altname Grove PIR Motion Sensor
     * @type light
     * @man seeed
     * @web http://www.seeedstudio.com/depot/Grove-PIR-Motion-Sensor-p-802.html
     * @con gpio
     * @kit tsk
     *
     * @brief API for the BISS0001 Motion Sensor
     *
     * PIR sensors allow you to sense motion, almost always used to detect
     * whether a human has moved in or out of the sensors range. They are
     * small, inexpensive, low-power, easy to use and don't wear out. For that
     * reason they are commonly found in appliances and gadgets used in homes
     * or businesses. They are often referred to as PIR, "Passive Infrared",
     * "Pyroelectric", or "IR motion" sensors.
     *
     * @image html biss0001.jpg
     * @snippet biss0001.cxx Interesting
     */

  class BISS0001 : virtual public iMotion {
  public:
      /**
       * BISS0001 motion sensor constructor
       *
       * @param pin Digital pin to use
       */
      BISS0001(unsigned int pin);

      /**
       * BISS0001 destructor
       */
      ~BISS0001();

      /**
       * Gets the motion value from the sensor.
       *
       * @return true if motion was detected, false otherwise.
       */
      bool value();

      /**
       * Gets the motion value from the sensor.  This is a more
       * informative method name, but we want to keep compatibility
       * with the original for now. Implements iMotion interface.
       *
       * @return true if motion was detected, false otherwise.
       */
      virtual bool motionDetected();

  private:
      /* Disable implicit copy and assignment operators */
      BISS0001(const BISS0001&) = delete;
      BISS0001 &operator=(const BISS0001&) = delete;

      biss0001_context m_biss0001;
  };
}


