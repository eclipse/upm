/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <mraa/gpio.h>
#include <mraa/initio.hpp>
#include <interfaces/iHallEffect.hpp>

namespace upm {
  /**
   * @brief A110X Hall Effect Sensors
   * @defgroup a110x libupm-a110x
   * @ingroup seeed gpio electric robok
   */

  /**
   * @library a110x
   * @sensor a110x
   * @comname Hall Effect Sensor
   * @altname Grove Hall Sensor
   * @altid A1101, A1102, A1103, A1004, A1106
   * @type electric
   * @man seeed
   * @web http://www.allegromicro.com/en/Products/Magnetic-Digital-Position-Sensor-ICs/Hall-Effect-Unipolar-Switches/A1101-2-3-4-6.aspx
   * @con gpio
   * @kit robok
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
  class A110X : virtual public iHallEffect {
  public:
    /**
     * A110x digital sensor constructor
     *
     * @param pin Digital pin to use
     */
    A110X(int pin);
    /**
     * Instantiates A110x digital sensor based on a given string.
     *
     * @param initStr string containing specific information for A110X initialization.
     */
    A110X(std::string initStr);

    /**
     * A110X destructor
     */
    ~A110X();
    /**
     * Determines whether a magnetic field of south polarity has been detected
     *
     * @return True if magnetic field detected
     */
    virtual bool magnetDetected();

    /**
     * Installs an interrupt service routine (ISR) to be called when
     * the appropriate magnetic field is detected
     *
     * @param isr Pointer to a function to be called on interrupt
     * @param arg Pointer to an object to be supplied as an
     * argument to the ISR.
     */
    void installISR(void (*isr)(void *), void *arg);

    /**
     * Uninstalls the previously installed ISR
     *
     */
    void uninstallISR();

  private:

    bool m_isrInstalled;
    mraa::MraaIo mraaIo;
    mraa_gpio_context m_gpio;
  };
}


