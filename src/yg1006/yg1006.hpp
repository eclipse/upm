/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include "yg1006.h"

namespace upm {
    /**
     * @brief YG1006 Flame Sensor
     * @defgroup yg1006 libupm-yg1006
     * @ingroup seeed gpio light hak
     */

    /**
     * @library yg1006
     * @sensor yg1006
     * @comname Flame Sensor
     * @altname Grove Flame Sensor
     * @type light
     * @man seeed
     * @web http://www.seeedstudio.com/wiki/Grove_-_Flame_Sensor
     * @con gpio
     * @kit hak
     *
     * @brief API for the YG1006 Flame Sensor
     *
     *   UPM module for the YG1006 flame sensor. It detects a flame or any
     *   other light source in the 760-1,100 nm wavelength range.
     *
     * @image html yg1006.jpg
     * @snippet yg1006.cxx Interesting
     */
  class YG1006 {
  public:

    /**
     * YG1006 constructor
     *
     * @param pin Digital pin to use
     */
    YG1006(unsigned int pin);

    /**
     * YG1006 destructor
     */
    ~YG1006();

    /**
     * Determines whether a flame has been detected
     *
     * @return true if a flame or another comparable light source has
     * been detected
     */
    bool flameDetected();

  private:
    /* Disable implicit copy and assignment operators */
    YG1006(const YG1006&) = delete;
    YG1006 &operator=(const YG1006&) = delete;

    yg1006_context m_yg1006;
  };
}


