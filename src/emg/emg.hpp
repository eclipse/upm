/*
 * Author: Zion Orent <zorent@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

  private:
    mraa_aio_context m_aio;
  };
}
