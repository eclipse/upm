/*
 * Author: Zion Orent <zorent@ics.com>
 *         Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <interfaces/iMotion.hpp>
#include <biss0001.h>
#include <mraa/initio.hpp>

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
       * Instantiates BISS0001 Motion Sensor object based on a given string.
       *
       * @param initStr string containing specific information for BISS0001 initialization.
       */
      BISS0001(std::string initStr);

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
      mraa::MraaIo mraaIo;
  };
}


