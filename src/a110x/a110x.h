/*
 * Author: Jon Trulson <jtrulson@ics.com>
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

#include <string>
#include <mraa/gpio.h>

namespace upm {
  /**
   * @brief A110X Hall Effect library
   * @defgroup a110x libupm-a110x
   * @ingroup seeed gpio electric
   */

  /**
   * @library a110x
   * @sensor a110x
   * @comname A110X Hall Effect Sensor
   * @altname A1101, A1102, A1103, A1004, A1106
   * @altname Grove Hall Sensor
   * @type electric
   * @man seeed
   * @web http://www.allegromicro.com/en/Products/Magnetic-Digital-Position-Sensor-ICs/Hall-Effect-Unipolar-Switches/A1101-2-3-4-6.aspx
   * @con gpio
   *
   * @brief API for the A110X Hall Effect sensors
   *
   * UPM module for the A110X (A1101, A1102, A1103, A1104, and A1106)
   * Hall Effect sensors.  It outputs a digital signal indicating
   * whether it is detecting a magnetic field with south polarity
   * perpendicular to the sensor element.
   *
   * @image html a110x.jpg
   * An example showing a simple test for the presence of a field
   * @snippet a110x.cxx Interesting
   * An example demonstrating the use of an interrupt handler to count pulses
   * @snippet a110x-intr.cxx Interesting
   */
  class A110X {
  public:
    /**
     * A110x digital sensor constructor
     *
     * @param pin digital pin to use
     */
    A110X(int pin);
    /**
     * A110X Destructor
     */
    ~A110X();
    /**
     * Determine whether a magnetic field of south polarity has been detected
     *
     * @return True if magnetic field detected
     */
    bool magnetDetected();

    /**
     * Install an Interrupt Service Routine (ISR) to be called when
     * the appropriate magnetic field is detected.
     *
     * @param fptr function pointer to function to be called on interrupt
     * @param arg pointer to an object that will be supplied as an
     * arguement to the ISR.
     */
    void installISR(void (*isr)(void *), void *arg);

    /**
     * Uninstall the previously installed Interrupt Service Routine (ISR)
     *
     */
    void uninstallISR();

  private:
    bool m_isrInstalled;
    mraa_gpio_context m_gpio;
  };
}


