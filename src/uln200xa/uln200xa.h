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

#include <stdint.h>
#include <sys/time.h>

#include <mraa/gpio.h>
#include <mraa/pwm.h>

namespace upm {

  /**
   * @brief ULN200XA Stepper Driver library
   * @defgroup uln200xa libupm-uln200xa
   * @ingroup seeed gpio motor
   */

  /**
   * @library uln200xa
   * @sensor uln200xa
   * @comname ULN200XA Stepper Driver
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
   * @image html uln200xa.jpg
   * Example driving a stepper motor
   * @snippet uln200xa.cxx Interesting
   */


  class ULN200XA {
  public:

    /**
     * Enum to specify the direction of a motor
     */
    typedef enum {
      DIR_CW   = 0x01,
      DIR_CCW  = 0x02
    } ULN200XA_DIRECTION_T;

    /**
     * ULN200XA constructor
     *
     * @param stepsPerRev Number of steps per full revolution
     * @param i1 Digital pin to use for stepper input 1
     * @param i2 Digital pin to use for stepper input 2
     * @param i3 Digital pin to use for stepper input 3
     * @param i4 Digital pin to use for stepper input 4
     */
    ULN200XA(int stepsPerRev, int i1, int i2, int i3, int i4);

    /**
     * ULN200XA destructor
     */
    ~ULN200XA();

    /**
     * Returns the number of milliseconds elapsed since initClock()
     * was last called.
     *
     * @return Elapsed milliseconds
     */
    uint32_t getMillis();

    /**
     * Resets the clock
     *
     */
    void initClock();

    /**
     * Sets the speed of the stepper motor in revolutions per minute (RPM)
     *
     * @param speed Speed to set the motor to, in RPM
     */
    void setSpeed(int speed);

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

  private:
    struct timeval m_startTime;

    // stepper (4-wire)
    mraa_gpio_context m_stepI1;
    mraa_gpio_context m_stepI2;
    mraa_gpio_context m_stepI3;
    mraa_gpio_context m_stepI4;

    // steps per revolution
    int m_stepsPerRev;
    int m_currentStep;
    uint32_t m_stepDelay;

    /**
     * Steps the stepper motor one tick
     *
     */
    void stepperStep();

    /**
     * Defines the step direction: 1 = forward, -1 = backward
     *
     */
    int m_stepDirection;
  };
}


