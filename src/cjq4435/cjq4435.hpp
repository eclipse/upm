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
   * @brief CJQ4435 MOSFET library
   * @defgroup cjq4435 libupm-cjq4435
   * @ingroup seeed gpio pwm electric robok
   */

  /**
   * @library cjq4435
   * @sensor cjq4435
   * @comname Grove MOSFET
   * @altname CJQ4435
   * @type electric
   * @man seeed
   * @con gpio pwm
   * @kit robok
   *
   * @brief API for the CJQ4435 MOSFET
   *
   * UPM module for the CJQ4435 MOSFET. It was developed using the
   * Grove MOSFET module.  A MOSFET is like a switch, but it can
   * switch much faster than a mechanical relay.  Here, we implement
   * support via MRAA pulse width modulation (PWM) functions.
   * Note: available periods vary depending on
   * the capabilities of your device.
   *
   * @image html cjq4435.jpg
   * @snippet cjq4435.cxx Interesting
   */
  class CJQ4435 {
  public:
    /**
     * CJQ4435 constructor
     *
     * @param pin Digital pin to use; this pin must be PWM-capable
     */
    CJQ4435(int pin);

    /**
     * CJQ4435 destructor
     */
    ~CJQ4435();

    /**
     * Sets a period in microseconds
     *
     * @param us Period in microseconds
     */
    void setPeriodUS(int us);

    /**
     * Sets a period in milliseconds
     *
     * @param ms Period in milliseconds
     */
    void setPeriodMS(int ms);

    /**
     * Sets a period in seconds
     *
     * @param seconds Period in seconds
     */
    void setPeriodSeconds(float seconds);

    /**
     * Enables output
     *
     * @param enable Enables PWM output if true, disables otherwise
     */
    void enable(bool enable);

    /**
     * Sets a duty cycle. Duty cycle is a floating-point number
     * between 0.0 (always off) and 1.0 (always on). It represents a
     * proportion of time, per period, during which the output is
     * driven high.
     *
     * @param dutyCycle Duty cycle to use
     */
    void setDutyCycle(float dutyCycle);

    /**
     * Shortcut to turn the output to continuous on (high)
     */
    void on();

    /**
     * Shortcut to turn the output to continuous off (low)
     */
    void off();

  private:
    bool m_enabled;
    mraa_pwm_context m_pwm;
  };
}


