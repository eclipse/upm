/*
 * Author: Zion Orent <zorent@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <mraa/aio.h>
#include "interfaces/iEmg.hpp"

namespace upm {
  /**
   * @brief EMG Muscle Signal Reader
   * @defgroup emg libupm-emg
   * @ingroup seeed analog electric
   */

  /**
   * @library emg
   * @sensor emg
   * @comname Electromyography (EMG) Sensor
   * @altname Grove EMG Sensor
   * @type electric
   * @man seeed
   * @con analog
   *
   * @brief API for the Grove EMG Muscle Signal Reader
   *
   * Grove EMG muscle signal reader gathers small muscle signals,
   * then processes them, and returns the result
   *
   * @image html emg.jpg
   * @snippet emg.cxx Interesting
   */
  class EMG : virtual public iEmg {
  public:
    /**
     * Grove EMG reader constructor
     *
     * @param pin Analog pin to use
     */
    EMG(int pin);
    /**
     * EMG destructor
     */
    ~EMG();

    /**
     * Calibrates the Grove EMG reader
     */
    virtual void calibrate();

    /**
     * Measures muscle signals from the reader
     *
     * @return Muscle output as analog voltage
     */
    virtual int value();

    virtual float getVolts();

  private:
    mraa_aio_context m_aio;
    /* Analog voltage reference */
    float m_aRef = 5.0;
    /* Scale */
    float m_scale = 1.0;
    /* Offset in sensor units */
    float m_offset = 0.0;
  };
}
