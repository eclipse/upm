/*
 * Author: Jon Trulson <jtrulson@ics.com>
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

#include <mraa/pwm.h>

namespace upm {

  /**
   * @brief C++ API for the CJQ4435 MOSFET
   *
   * UPM module for the CJQ4435 MOSFET.  It was developed using the
   * Grove MOSFET module.  A MOSFET is like a switch, but it can
   * switch much faster than a mechanical relay.  Here, we implement
   * support via MRAA's PWM (Pulse Width Modulation) functions.
   *
   * Please note, that the available periods will vary depending on
   * the capabilities of your device.
   *
   * @ingroup gpio pwm
   * @defgroup cjq4435 libupm-cjq4435
   * @snippet cjq4435.cxx Interesting
   */
  class CJQ4435 {
  public:
    /**
     * CJQ4435 constructor
     *
     * @param pin digital pin to use - this pin must be PWM capable
     */
    CJQ4435(int pin);

    /**
     * CJQ4435 Destructor
     */
    ~CJQ4435();

    /**
     * set the period in microseconds
     *
     * @param us period in microseconds
     */
    void setPeriodUS(int us);

    /**
     * set the period in milliseconds
     *
     * @param ms period in milliseconds
     */
    void setPeriodMS(int ms);

    /**
     * set the period in seconds
     *
     * @param seconds period in seconds
     */
    void setPeriodSeconds(float seconds);

    /**
     * enable output
     *
     * @param enable enable PWM output if true, disable if false
     */
    void enable(bool enable);

    /**
     * set the duty cycle.  The duty cycle is a floating point number
     * between 0.0 (always off) to 1.0 (always on).  It represents how
     * much time as a percentage, per period, that the output will be
     * driven high.
     *
     * @param dutyCycle the duty cycle to use
     */
    void setDutyCycle(float dutyCycle);

    /**
     * a shortcut for turning the output to continuous on (high)
     */
    void on();

    /**
     * a shortcut for turning the output to continuous off (low)
     */
    void off();

  private:
    bool m_enabled;
    mraa_pwm_context m_pwm;
  };
}


