/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
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

#include <uln200xa.h>

namespace upm {

  /**
   * @brief ULN200XA Stepper Driver
   * @defgroup uln200xa libupm-uln200xa
   * @ingroup seeed gpio motor
   */

  /**
   * @library uln200xa
   * @sensor uln200xa
   * @comname Darlington Stepper Driver
   * @altname ULN2001A ULN2002A ULN2003A ULN2004A
   * @type motor
   * @man seeed
   * @web http://www.seeedstudio.com/depot/Gear-Stepper-Motor-with-Driver-p-1685.html?cPath=39_40
   * @con gpio
   *
   * @brief UPM module for the ULN200XA Darlington Stepper Driver
   *
   * This module was developed on a ULN2003A Stepper Driver. It
   * should also support the ULN2001A, ULN2002A, and ULN2004A devices, when
   * using to drive the 28BYJ-48 unipolar stepper motor.
   *
   * Vcc goes to the 5V pin on your development board and the Vm pin should
   * be connected to an external 5V supply.
   *
   * @image html uln200xa.jpg
   * Example driving a stepper motor
   * @snippet uln200xa.cxx Interesting
   */


  class ULN200XA {
  public:

    /**
     * ULN200XA constructor
     *
     * @param stepsPerRev Number of steps per full revolution
     * @param i1 Digital pin to use for stepper input 1
     * @param i2 Digital pin to use for stepper input 2
     * @param i3 Digital pin to use for stepper input 3
     * @param i4 Digital pin to use for stepper input 4
     */
    ULN200XA(int stepsPerRev, unsigned int i1, unsigned int i2,
             unsigned int i3, unsigned int i4);

    /**
     * ULN200XA destructor
     */
    ~ULN200XA();

    /**
     * Sets the speed of the stepper motor in revolutions per minute (RPM)
     *
     * @param speed Speed to set the motor to, in RPM
     */
    void setSpeed(unsigned int speed);

    /**
     * Sets the direction of the motor, clockwise or counterclockwise
     *
     * @param dir Direction to set the motor to
     */
    void setDirection(ULN200XA_DIRECTION_T dir);

    /**
     * Steps the stepper motor a specified number of steps
     *
     * @param steps Number of steps to move the stepper motor
     */
    void stepperSteps(unsigned int steps);

    /**
     * Releases the stepper motor by removing power
     *
     */
    void release();

  protected:
    uln200xa_context m_uln200xa;

  private:
    /* Disable implicit copy and assignment operators */
    ULN200XA(const ULN200XA&) = delete;
    ULN200XA &operator=(const ULN200XA&) = delete;
  };
}
